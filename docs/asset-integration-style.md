# Organización de `assets/` y estilo de arte estático

Este documento cubría reglas para mezclar packs de terceros con el arte propio. Esos packs (Pixel Cyberpunk Interior, CraftPix GUI, CraftPix 184808, Cyberpunk_City_Ext, Striker) fueron **removidos** del repositorio (ninguno llegó a estar wireado en el render real) — ver `docs/third-party-assets.md` para el detalle de esa decisión. El proyecto genera su arte estático con IA en `assets/generated/`; este doc queda como referencia de organización de carpetas y de la política de filtrado vigente.

## Organización de `assets/`

`assets/` tenía ~130MB de zips/rar sueltos en la raíz y una imagen suelta sin ninguna referencia desde el código — ver `grep -rohE '"assets/[^"]*"' src/ include/` para la lista real de rutas que el juego usa. Los que correspondían a packs de terceros descartados se borraron; el resto (batches de runtime propios) se movió a `assets/archive/raw-downloads/` (preservado, no wireado) para que la raíz de `assets/` solo tenga carpetas con función clara:

| Carpeta | Uso real (wireado en código) |
|---|---|
| `assets/generated/` | Assets generados con IA, consumidos por `GameApp` — retratos, fondos, figuras, evidencia. `masters/`/`previews/` adentro son referencia/QA, no runtime. |
| `assets/processed/` | Pipeline propio (`scripts/process_assets.sh`) — props/edificios/personajes procesados. |
| `assets/audio/` | Efectos de UI (`AudioService`). |
| `assets/fonts/` | DejaVu Sans Mono (ADR 0015). |
| `assets/raw/`, `assets/imports/`, `assets/textures/` | Fuentes/staging del pipeline propio (`scripts/process_assets.sh`, `scripts/normalize_characters.py`). |
| `assets/manifests/`, `assets/prompts/`, `assets/requests/`, `assets/data/` | Metadata/manifests, no imágenes. |
| `assets/archive/raw-downloads/` | Batches de runtime propios ya superados por versiones más nuevas — preservados por si hace falta volver a algo puntual, no son parte del árbol activo. |

Antes de mover o borrar cualquier archivo de `assets/`, correr `grep -rohE '"assets/[^"]*"' src/ include/ | sort -u` y confirmar que la ruta no esté en esa lista.

## Filtrado

El arte estático cinematográfico de `assets/generated/` usa `TEXTURE_FILTER_BILINEAR` (default de `AssetStore::texture()`, ADR 0017); `TEXTURE_FILTER_POINT` se pide explícitamente sólo para pixel art intencional (ninguno wireado en el flujo activo hoy). No hay reglas de mezcla de packs de terceros porque no hay packs de terceros integrados actualmente.

## Historial

La integración parcial de Pixel Cyberpunk Interior (6 tiles recortados, 3 wireados como `WorldProp` y luego revertidos por verse como "collage") y el resto de los packs evaluados quedan documentados en `docs/third-party-assets.md` como registro histórico — los archivos ya no existen en el repositorio.
