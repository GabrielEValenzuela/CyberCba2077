# Pipeline de assets raster

## Objetivo y estructura

Los originales nunca se modifican: `assets/raw/` conserva una copia con nombre estable y `assets/processed/` contiene el PNG que el juego puede cargar. El manifest versionado en `assets/data/assets-manifest.json` enlaza ambas rutas, define canvas, bounds visuales y el punto de apoyo. Esta separación permite sustituir arte provisional sin cambiar coordenadas de gameplay.

```text
assets/data/*.png -> scripts/process_assets.sh -> assets/raw/ + assets/processed/
                                      -> assets/data/assets-manifest.json
```

La tarea inicial cubre 18 salidas runtime: Emma (idle/hacking), Magga (idle/impacto), ocho props del refugio, dos edificios y seis props exteriores. La fuente opaca antigua de Emma se conserva como `source.character.emma.idle_opaque_legacy`: no se usa en runtime porque el negro del fondo toca prendas negras y una extracción automática destruiría píxeles del personaje. La variante equivalente con alpha es la fuente de Emma idle.

## Procesar

Requiere ImageMagick 7 (`magick`):

```bash
./scripts/process_assets.sh
./scripts/validate_assets.sh
```

El script ubica el bounding box con alpha mayor a 3 %, pero aplica ese recorte sobre el alpha original. Así elimina padding y semitransparencia accidental distante sin convertir los bordes del sujeto en una máscara binaria. Después hace `resize` con `-filter point`, conserva proporción, centra el contenido dentro de un canvas con 16 px de margen y no introduce blur ni antialiasing nuevo.

## Canvas, pivotes y uso

- Personajes standing: `256x384`; pivot de base en `(128,368)`.
- Acciones: `384x256`; pivot de base en `(192,240)`.
- Props medianos: `256x256` o `256x384`; base inferior con margen de 16 px.
- Estructuras: `512x512`; base inferior en `(256,496)`.
- Props anchos: `384x256`; base inferior en `(192,240)`.

`bounds` del manifest es la caja resultante usando alpha > 3 % y sirve para placement/culling; no es una hitbox. Las colisiones continúan siendo datos de gameplay. `anchor` es la versión normalizada del mismo pivot y permite migrar a un atlas sin reinterpretar el origen.

## Validación

`validate_assets.sh` verifica para cada salida el canvas esperado, PNG RGBA y esquina transparente, y comprueba que cada ruta `processed` del manifest exista. La inspección visual obligatoria se realiza componiendo cada PNG sobre fondo claro, oscuro y una captura del mundo; se rechazan halos, color de fondo residual y reescalado suavizado. `AssetStore` aplica `TEXTURE_FILTER_POINT` al cargar cualquier textura.

Los recursos siguen marcados como `placeholder: true`: fueron preparados técnicamente, no declarados arte final ni recoloreados. Los props pictóricos no reemplazan los tiles top-down procedurales hasta que exista una integración de cámara y escala compatible.
