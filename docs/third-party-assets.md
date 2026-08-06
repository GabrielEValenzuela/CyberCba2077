# Assets de terceros — CyberCBA 2077

**Removidos**: los packs de terceros evaluados (Pixel Cyberpunk Interior, CraftPix GUI, CraftPix 184808 icons, Cyberpunk_City_Ext_V1.0, Striker) fueron descartados y borrados del repositorio — ninguno llegó a estar wireado en el render en tiempo real (los 3 tiles de Pixel Cyberpunk Interior que se habían conectado como `WorldProp` decorativos fueron revertidos en una sesión anterior por verse como "collage", ver `docs/asset-integration-style.md`). El proyecto pasó a generar su arte estático con IA (`assets/generated/`, ver `docs/static-art-direction.md`) en vez de integrar packs de terceros. `assets/data/assets-manifest.json` (schemaVersion 7) ya no tiene entradas `environment.interior.*`.

Este documento es la fuente de verdad narrativa para la pantalla de créditos in-game, que debe referenciarlo en vez de duplicar la lista.

| Pack | Autor | Estado |
|---|---|---|
| Pixel Cyberpunk Interior | DyLESTorm (livingtheindie) | **Removido**. Estaba parcialmente integrado (6 tiles recortados, 3 wireados y luego revertidos); el pack y sus derivados (`assets/vendor/`, `assets/processed/environment/interior_*.png`) se borraron. |
| Free GUI for Cyberpunk Pixel Art | CraftPix.net | **Removido**. Verificado pero nunca integrado en escenas; borrado sin uso. |
| Free Cyberpunk Resource Pixel Art 32x32 Icons | CraftPix.net | **Removido**. Extraído pero ningún ícono individual llegó a seleccionarse/recortarse; borrado sin uso. |
| Cyberpunk_City_Ext_V1.0 | Desconocido | **Removido**. Nunca tuvo licencia verificable (ver historial de `assets/imports/rejected/PENDING_VERIFICATION.md` en commits previos); no se usó nunca en producción. |
| Striker (sprite strips) | Desconocido | **Removido**. Misma razón que el anterior — licencia no verificable, nunca usado. |
| Kenney (UI/prompts/iconos) | Kenney.nl | No descargado — sigue siendo un gap si se necesita en el futuro (CC0, https://kenney.nl/assets). |
| Assets narrativos hechos a mano/generados con IA | Proyecto CyberCBA 2077 | Vigente — `assets/processed/` (pipeline propio) y `assets/generated/` (IA), ver `docs/static-art-direction.md`. |
| DejaVu Sans Mono | Bitstream, Inc. / DejaVu contributors | **Integrado**: fuente de UI real, `AssetStore::font()` / `GameApp::drawText()`. Ver ADR `0015-ui-font.md`. |

## Nota sobre Thiings

Por directiva de `AGENTS.md` §6, Thiings solo puede usarse como referencia temporal para íconos de inventario y no se acreditó ni integró.

## ADRs relacionados

`docs/adrs/0007-primary-art-pack.md` documentó la elección original de Pixel Cyberpunk Interior como pack base — queda como registro histórico de esa decisión, superseded por el pivot a arte generado con IA (no se reescribe, ver ADR 0017 para la dirección de arte vigente).
