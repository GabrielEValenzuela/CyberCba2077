# DejaVu Sans Mono

- Autor original: Bitstream, Inc. (Bitstream Vera); modificaciones DejaVu por Stepan Roh y colaboradores (dominio público).
- Fuente oficial: https://dejavu-fonts.github.io/
- Archivos locales: `DejaVuSansMono.ttf` (regular), `DejaVuSansMono-Bold.ttf` (bold).
- Origen de esta copia: paquete del sistema `fonts-dejavu-mono` (Debian/Kali), verificado contra el proyecto oficial DejaVu.

## Licencia (`LICENSE.txt`, Bitstream Vera Fonts Copyright)

- Uso, copia, modificación, fusión, publicación, distribución y venta permitidos, sin regalías.
- Condición: mantener el aviso de copyright/marca y este permiso en todas las copias de las tipografías.
- Si se modifica el font, debe renombrarse (no puede seguir usando "Bitstream" o "Vera" en el nombre) — no aplica aquí, se usa sin modificar.
- No se puede vender el/los tipo(s) de letra por sí solo(s) como producto independiente (sí como parte de un paquete de software mayor, que es este caso).

## Rol asignado en CyberCBA 2077

Fuente de UI real, reemplaza el bitmap font por defecto de raylib (`DrawText`/`GetFontDefault()`) en todo el texto renderizado por `GameApp` — ver ADR `0015-ui-font.md`. Monoespaciada, coherente con la estética de terminal/HUD cyberpunk del proyecto y mucho más legible a los tamaños usados (13-68px) que el font de 10px por defecto de raylib escalado.

## Estado de integración

Cargada por `AssetStore::font()` (ya existía, sin uso previo) y consumida por `GameApp::drawText()` vía `DrawTextEx`. Registrada en `assets/data/assets-manifest.json` bajo una categoría `font` nueva dentro de `staticArtCategories` (schemaVersion 4→5).
