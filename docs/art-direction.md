# Dirección artística

## Principio visual

CYBERCBA 2077 combina ilustración narrativa 2D semirrealista con gameplay top-down en pixel art. La primera sostiene los momentos emocionales; la segunda mantiene el mundo jugable legible, consistente y viable de producir en raylib. La referencia visual entregada por el equipo orienta el tono, no es un asset que se pueda copiar: no se reproducen personajes, poses, composición ni detalles particulares.

La paleta base es fría, con iluminación ambiental cian y acentos ámbar o rojo apagado. El cyberpunk es urbano y funcional: ropa tecnológica sobria, expresiones naturales y accesorios que respondan a la narrativa.

## Ilustración narrativa

Se usa para la selección de personaje, diálogos, transmisiones, capítulos, recuerdos, pantallas de carga y escenas emocionales. Los retratos son de personas adultas, de proporciones humanas, con influencia moderada de cómic moderno y animación occidental. Deben tener sombras suaves, bordes definidos y fondos ambientales o desenfocados.

Las cinemáticas se construyen con ilustraciones estáticas: desplazamiento de cámara, parallax, lluvia, partículas, interferencia, pulsos de luz, fundidos y cambios puntuales de expresión. No requieren animación tradicional completa.

## Gameplay pixel art

| Elemento | Baseline |
| --- | --- |
| Resolución virtual | 1280 x 720 |
| Tiles | 32 x 32 px |
| Personajes | 48 x 64 px, o una proporción justificada |
| Animaciones | 8 a 12 FPS |
| Filtrado | `TEXTURE_FILTER_POINT` |

Los sprites se escalan sólo con factores enteros cuando sea posible. No se usa filtrado bilineal sobre pixel art ni se mezclan escalas arbitrarias. Las siluetas deben permitir reconocer entidades aun sin color.

El prólogo compone esos sprites con placas de suelo, humedad, sombras de contacto, luces puntuales cian/ámbar y lluvia exterior. La iluminación no sustituye la lectura de la silueta: concentra la atención en el transmisor, la terminal y el Neometro, y se atenúa con la configuración de accesibilidad.

## Protagonistas

### Emmanuel “Emma” Vázquez

Origen: Santa María. Especialidad: hacking. Acento: cian.

Emma usa ropa oscura funcional —hoodie o campera técnica—, guantes de interfaz y un dispositivo de muñeca o antebrazo. Su postura es contenida, observadora y precisa. La luz de interfaz, la pose defensiva y un pulso electrónico comunican su capacidad sin depender sólo del cian.

Para v0.1.0, el arte final debe poder sustituir placeholders con: `idle`, `walk_up`, `walk_down`, `walk_left`, `walk_right`, `interact`, `hack_start`, `hack_loop`, `hack_success` y `hurt`.

### Magalí “Magga” Temerro

Origen: San Justo. Especialidad: lucha. Acento: ámbar o rojo apagado.

Magga tiene cabello largo rubio oscuro/castaño claro y ojos verdes claramente visibles. Lleva ropa flexible y resistente, guantes o vendas, protecciones discretas y botas reforzadas. Su postura es firme y frontal; sus movimientos tienen mayor amplitud y peso. La guardia de combate y los impactos ámbar comunican su capacidad sin convertirla en una estética militar genérica.

Para v0.1.0, el arte final debe poder sustituir placeholders con: `idle`, `walk_up`, `walk_down`, `walk_left`, `walk_right`, `interact`, `attack_light`, `attack_heavy`, `dodge` y `hurt`.

## Assets y placeholders

Las cuatro representaciones previstas por protagonista son retratos, imagen de selección, sprite sheet y silueta. Para v0.1.0 bastan un retrato neutral, uno emocional, selección, `idle`, desplazamiento, habilidad y silueta. Mientras no exista arte final, se usan formas, partículas y siluetas originales; deben conservar identificador, pivote, caja, tamaño y API previstos para el reemplazo.

`assets/data/assets-manifest.json` es la fuente de metadatos de cada recurso. Registra fuente conservada, salida runtime, dimensiones, bounds y pivote, además de licencia, autor y estado de placeholder. Ninguna lógica de juego debe depender de un archivo de imagen particular. Los PNG de runtime se exportan con alpha, canvas y filtro point mediante el pipeline documentado en [asset-pipeline.md](asset-pipeline.md).

## Recursos externos y Thiings

Antes de incorporar un recurso externo se verifican sus términos vigentes y se registra autor, fuente y licencia en el manifest. Una descarga gratuita no implica uso comercial gratuito. No se redistribuyen recursos externos como colecciones independientes y los recursos provisorios se marcan como reemplazables.

Thiings puede servir de referencia o de placeholder temporal para objetos de inventario, terminales, radios, llaves, baterías y botiquines. Sus iconos 3D no se integran directamente al mundo pixel art: deben redibujarse o pasar por una adaptación con paleta limitada, nearest-neighbor, contorno común y corrección manual. Para v0.1.0 se prioriza arte procedural y propio.

## Criterios de aceptación

- Emma y Magga tienen siluetas distintas y comunican hacking/lucha mediante diseño y animación.
- La UI puede alojar retratos de alta resolución y el mundo conserva pixel art consistente.
- Los sprites conservan píxeles nítidos al escalarse.
- Todos los assets incorporados tienen metadata, procedencia y licencia registradas.
- Los placeholders se reemplazan sin cambios en la lógica del juego.
