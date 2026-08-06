# ADR 0013: Taxonomía de assets estáticos (Background/Portrait/CombatFigure/Icon/UIFrame/Effect/EvidenceImage)

El manifest (`assets/data/assets-manifest.json`) solo tenía categorías del prototipo top-down (`character`, `prop`, `environment`, `building`, `source`). El pivot narrativo necesita categorías propias para fondos ilustrados, retratos, figuras de combate estáticas, íconos, marcos de UI, efectos y evidencia — spec §15.

Se agrega `staticArtCategories` (schemaVersion 3→4) documentando las 7 categorías nuevas con tamaño/aspecto recomendado y estado real. **No se fabricó arte falso**: no existe ningún fondo ilustrado, retrato o figura de combate estática en el repositorio todavía, así que esas categorías quedan documentadas como `"status": "sin assets reales todavia"` en vez de tener entradas de catálogo inventadas.

Las únicas entradas nuevas en `assets[]` son 5 `evidenceImage` reales, porque `EvidenceJournal` (Phase 2) ya referencia esos 5 ids narrativos:
- `evidenceImage.photo` / `evidenceImage.network_map`: reutilizan honestamente los sprites de world-prop existentes (`prop.shelter.emma_magga_photo`, `prop.shelter.neo_cordoba_map_panel`) — son el mismo archivo, no una imagen de evidencia dedicada de mayor resolución.
- `evidenceImage.insignia`, `evidenceImage.unsent_message`, `evidenceImage.train41_record`: **gap real**, sin `processed`; `GameApp` los muestra hoy solo como texto (ver `docs/static-art-direction.md`).

Los packs de terceros verificados pero no integrados (`assets/vendor/craftpix-cyberpunk-gui`, `assets/vendor/craftpix-184808-icons`) quedan referenciados como candidatos para `icon`/`uiFrame` en `staticArtCategories`, sin decidir su integración en esta fase — ver `docs/third-party-assets.md` para el estado de licencias.

**Deuda preexistente encontrada y corregida en el mismo cambio**: `scripts/validate_assets.sh` verificaba `schemaVersion == 3` y un conteo fijo de 19 assets `processed`, pero el manifest ya tenía 25 (los 6 `environment.interior.*` del pack Pixel Cyberpunk Interior, integrados en una sesión anterior, nunca actualizaron el script — el `&&`/pipe con el que se probaba el script antes ocultaba el fallo real). Se actualizó a `schemaVersion == 4` y 27 (25 + los 2 `evidenceImage` que reutilizan sprites existentes), y se corrigió el chequeo de `source` para no exigir un archivo en disco cuando el manifest declara explícitamente el gap (`evidenceImage.insignia`/`unsent_message`/`train41_record`, sin campo `source`).
