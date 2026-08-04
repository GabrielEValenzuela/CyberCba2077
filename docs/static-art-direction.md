# Dirección de arte estática (pivot RPG narrativo)

Ver `docs/art-direction.md` para la dirección visual del prototipo top-down (todavía preservada, ya no es el flujo principal — ADR `0008-narrative-rpg-pivot.md`). Este documento cubre las categorías estáticas nuevas del RPG narrativo, registradas en `staticArtCategories` dentro de `assets/data/assets-manifest.json` (schemaVersion 4) — ver ADR `0013-static-asset-categories.md`.

| Categoría | Formato recomendado | Uso | Estado real |
|---|---|---|---|
| `background` | 1920×1080, 16:9 | Fondo de escena narrativa (refugio, exterior, entrada Neometro) | **Piloto integrado**: `bg_refuge_rest_area` y `bg_neometro_entrance` (ver `assets/manifests/v0.1.0-prologue-runtime.json`) |
| `portrait` | 1024×1280, 4:5, fondo transparente | Diálogo, selección de personaje, transmisiones | **Integrado**: neutrales y 12 expresiones de Batch 02; las transmisiones usan sus retratos dedicados |
| `combatFigure` | 768×1024, 3:4, fondo transparente | Personajes/enemigos en `Screen::Encounter` | **Integrado**: figuras idle RGBA 1024×1536 y dron de seguridad RGBA 1200×1200 |
| `icon` | 256×256, 1:1 | Habilidades, estados, evidencia, nodos de misión | Packs verificados sin integrar: `assets/vendor/craftpix-184808-icons` (ver `docs/third-party-assets.md`) |
| `uiFrame` | variable | Marcos/paneles de UI | Pack verificado sin integrar: `assets/vendor/craftpix-cyberpunk-gui` |
| `effect` | variable | Partículas/transiciones de pantalla | Sin assets reales todavía |
| `evidenceImage` | 1200×900 | Imagen mostrada al descubrir una evidencia (`EvidencePanel`, `Screen::Objectives`) | **Integrado**: seis ilustraciones finales de Batch 02; el mensaje no enviado se elige según protagonista |
| `font` | TTF embebido bajo `assets/fonts/<pack>/` | Texto de UI (`GameApp::drawText`) | **Integrado**: DejaVu Sans Mono (regular+bold, licencia Bitstream Vera) reemplaza el bitmap font por defecto de raylib — ver ADR `0015-ui-font.md` |

El piloto runtime importado registra nueve assets, sus hashes, dimensiones, alpha, prompts, pivotes y safe areas en `assets/manifests/v0.1.0-prologue-runtime.json`; se valida con `python3 scripts/validate_assets.py`. Batch 02 añade 20 assets runtime, con hashes y lock de identidad de Magga, en `assets/manifests/v0.1.0-prologue-batch-02-runtime.json`; se valida con `python3 scripts/validate_batch_02_runtime.py`. `GameApp` consume ambos packs para selección, retratos de diálogo/transmisión, evidencia y encuentros. El catálogo principal `assets/data/assets-manifest.json` conserva los assets legacy top-down; no se mezclan sus rutas con el pack estático. Las categorías que todavía no tienen arte real (`icon`, `uiFrame`, `effect`) siguen usando UI procedimental/texto, no placeholders presentados como arte final.

## Pipeline de generación de assets estáticos

`scripts/generate_static_assets.py` (Pillow, determinista — mismo seed por asset en cada corrida, sin variación aleatoria entre ejecuciones) genera actualmente la categoría `evidenceImage` faltante:

```bash
python3 scripts/generate_static_assets.py --evidence   # genera insignia/unsent_message/train41_record
python3 scripts/generate_static_assets.py --all        # alias actual de --evidence (unica categoria soportada)
python3 scripts/generate_static_assets.py --validate   # verifica canvas 128x96 RGBA sin regenerar
```

Sigue las mismas reglas que `scripts/process_assets.sh`/`scripts/normalize_characters.py`: `PIL.Image` con paleta `cybercba_handheld` fija (near-black/dark-blue-gray/muted-cyan/warm-amber/light-gray/off-white), márgenes transparentes en las esquinas (requisito de `scripts/validate_assets.sh`), sin texto embebido (todo el texto lo dibuja `GameApp::drawText` sobre el asset, nunca al revés). No cubre todavía `background`/`portrait`/`combatFigure`/`icon`/`uiFrame` — quedan como pendiente explícito, no simulado.

## Resolución lógica y filtrado (actualizado por ADR 0017)

**Superseded parcialmente**: ADR `0016-logical-resolution-integer-scaling.md` fijaba una estética pixel-art de grilla lógica 320×180 con `TEXTURE_FILTER_POINT` global. El usuario aportó PoCs aprobadas de dirección cinemática (fondos ilustrados de alto detalle, retratos con iluminación suave) que contradicen esa dirección — ver ADR `0017-cinematic-ui-pivot.md`.

Estado actual:
- **Canvas**: se mantiene 1280×720 (`GameApp::W/H`) — ya es 16:9, cuenta como "equivalente documentado". Lo que cambió es que las pantallas ya no viven en cajas pequeñas centradas con márgenes negros grandes: usan `UiMetrics` (`src/ui/widgets/UiMetrics.hpp`) para ocupar casi todo el canvas.
- **Filtrado**: `AssetStore::texture()` aplica **bilinear por defecto** (parámetro `pointFilter=false`); `TEXTURE_FILTER_POINT` queda reservado para assets declarados explícitamente como pixel-art intencional (ninguno wireado en el flujo activo hoy). Ya no hay una regla global de point-filter para el gameplay narrativo.
- La referencia a "grilla lógica 320×180 a escala 4×" de ADR 0016 queda obsoleta para el flujo narrativo — no se aplica.
