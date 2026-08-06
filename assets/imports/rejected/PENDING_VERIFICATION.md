# Assets con licencia/autor sin verificar — NO integrar en producción

## Cyberpunk_City_Ext_V1.0.zip

- Extraído a: `assets/imports/extracted/cyberpunk-city-ext/`
- Contenido: `Cyberpunk_City_Character_01.png`, `Cyberpunk_City_Character_02.png`, `Cyberpunk_City_Doors_Windows_Signs.png`, `Cyberpunk_City_Props.png`, `Cyberpunk_City_Tiles_Fences.png`.
- No contiene LICENSE/README/CREDITS dentro del zip.
- Búsqueda web (WebSearch, agosto 2026) por el nombre exacto del paquete no devolvió una página de autor/itch.io/CraftPix identificable con certeza suficiente. Se encontró un post genérico de itch.io ("[Asset] Cyber City Cyberpunk Exterior Tileset and Characters") que podría o no corresponder al mismo autor/pack, pero no se pudo confirmar la correspondencia exacta de archivos ni los términos de licencia desde esta sesión.
- **Clasificación: Pending license verification.** No se usa como base exterior hasta confirmar autor y licencia oficial. Se recomienda verificar manualmente contra https://itch.io/t/995331/asset-cyber-city-cyberpunk-extetrior-tileset-and-characters y, si coincide, registrar SOURCE.md con el link y términos exactos.

## Striker.rar

- Extraído a: `assets/imports/extracted/striker/Striker/`
- Contenido: 9 sprite strips (`spr_Striker*_strip.png`), convención de nombres estilo GameMaker.
- Sin LICENSE/README/CREDITS en el archivo.
- WebSearch por nombres de archivo exactos y "Striker sprite pack" no identificó con confianza el autor/fuente original entre las fuentes prioritarias (itch.io oficial, Kenney, OpenGameArt, CraftPix).
- **Clasificación: Pending license verification.** NO integrado a ninguna escena de producción. Se mantiene únicamente en `assets/imports/extracted/striker/` para registro; no se copió a `assets/processed/` ni se referenció desde `AssetStore`/manifest.
- Acción recomendada para una sesión futura: búsqueda inversa de imagen (Google Images/TinEye) sobre `spr_StrikerIdle_strip.png`, o preguntar directamente al usuario la fuente de descarga original.

## Actualización (sesión de integración posterior)

Se reintentó la identificación de ambos packs vía WebSearch:

- **Cyberpunk_City_Ext_V1.0**: el mismo hilo de itch.io (`itch.io/t/995331/...`) sigue siendo el único resultado relacionado. Además apareció un pack de pago distinto, "Cyber City City Exterior Tileset" de `dragoonwys` (itch.io, USD 19.99), con nombre similar pero sin forma de confirmar que sus archivos internos coincidan con `Cyberpunk_City_Character_01.png`, `Cyberpunk_City_Doors_Windows_Signs.png`, `Cyberpunk_City_Props.png`, `Cyberpunk_City_Tiles_Fences.png` sin comprarlo/inspeccionarlo. **No se pudo confirmar autoría ni licencia.** Sigue en `pending_verification`, no integrado.
- **Striker.rar**: sin resultados nuevos relevantes; "spr_StrikerDash_strip" y variantes no arrojaron un autor/fuente identificable entre itch.io, Kenney, OpenGameArt o CraftPix. Sigue en `pending_verification`, no integrado.

Ambos packs permanecen fuera de `assets/processed/` y sin referencia en `assets-manifest.json`/`WorldLayout`. No se debe adivinar la licencia; se requiere confirmación directa del usuario o del autor original.
