# ADR 0015: Fuente de UI real (DejaVu Sans Mono) en vez del bitmap font por defecto de raylib

Todo el texto de `GameApp` (incluido el nuevo panel de nodos de `Screen::World`, ADR `0014`) se dibujaba con `DrawText`/`drawText`, que usa `GetFontDefault()` internamente: el bitmap font de 10px integrado en raylib, escalado. A tamaños grandes se ve borroso/pixelado de forma no intencional (no es un pixel font de la dirección artística — es un artefacto de escalar un font de 10px), lo que un usuario reportó explícitamente como difícil de leer.

`AssetStore::font(path, size)` (`src/ui/AssetStore.{hpp,cpp}`) ya existía con soporte para `LoadFontEx`, pero ningún código lo llamaba — infraestructura sin usar, como varios otros hallazgos de esta sesión.

## Decisión

Se agrega **DejaVu Sans Mono** (`assets/fonts/dejavu-sans-mono/`, licencia Bitstream Vera — permisiva, uso comercial permitido, ver `SOURCE.md`/`LICENSE.txt` en esa carpeta) como fuente de UI real. `GameApp` carga `DejaVuSansMono.ttf` a un tamaño base alto (64px) en el constructor vía `AssetStore::font()`, y `GameApp::drawText()` usa `DrawTextEx` con ese font en vez de `DrawText`. Si el archivo no carga (falta el binario, entorno sin assets), cae de nuevo a `DrawText`/`GetFontDefault()` — no rompe el arranque, solo se ve peor (mismo patrón de fallback silencioso que `AudioService`).

Monoespaciada por consistencia con la estética de terminal/HUD del proyecto (paneles, prompts, objetivos) — no es una decisión puramente estética, el espaciado fijo también hace más predecible el layout de texto (`wrapText`, botones de ancho fijo).

## Por qué no un font pixel-art

La dirección artística (`docs/art-direction.md`) reserva el estilo pixel-art estrictamente para el gameplay top-down (ahora deprecado como flujo principal, ADR `0008`). El texto de UI (paneles, diálogo, objetivos) no tiene ese requisito — la prioridad explícita del usuario fue legibilidad, no continuidad pixel-art. Un font pixel monoespaciado cyberpunk (ej. "Press Start 2P") sería una alternativa válida a futuro, pero no había ninguno disponible localmente con licencia verificable en esta sesión (ver limitación en el reporte); DejaVu Sans Mono es la opción real, legible y con licencia clara disponible ahora.

## `wrapText` y medición de texto

`wrapText()` (agregada para el fix anterior del texto desbordado) ahora mide con `MeasureTextEx` contra el font cargado cuando está disponible, en vez de `MeasureText` (que siempre mide contra el font por defecto) — si no, el ancho envuelto no coincidiría con el ancho real dibujado por `DrawTextEx`.
