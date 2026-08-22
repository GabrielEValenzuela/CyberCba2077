# Créditos — tu "anuario"

La pantalla de créditos in-game (`GameStateId::Credits`, accesible desde el menú principal) lee automáticamente todos los archivos de esta carpeta y los muestra, ordenados alfabéticamente por nombre de archivo. No hay que tocar código C++ para aparecer ahí.

## Cómo agregar tu entrada

1. Forkeá o cloná el repo y creá una rama nueva (`git checkout -b creditos/tu-nombre`).
2. Agregá un archivo de texto acá: `credits/tu-nombre.txt` (usá tu nombre o usuario de GitHub como nombre de archivo, sin espacios — ej. `credits/juan-perez.txt`).
3. Escribí tu entrada adentro: 2 a 5 líneas, texto libre — tu nombre, qué hiciste en el proyecto, un comentario, lo que quieras dejar en el "anuario". Ver `_ejemplo.txt` en esta misma carpeta.
4. Guardá el archivo en **UTF-8** (tildes/ñ tienen que verse bien en el juego).
5. `git add credits/tu-nombre.txt`, `git commit`, `git push`, y abrí un Pull Request.

## Reglas

- Un archivo por persona. Si ya tenés uno, editalo en vez de crear otro.
- No se procesan `README.md` ni archivos que empiecen con `_` (esos son plantillas/documentación, no entradas).
- Sin líneas vacías al principio/final — se recortan automáticamente, pero mantené el archivo prolijo.
- Este mecanismo es intencionalmente simple: la idea es que cada persona practique el flujo real de git/GitHub (branch, commit, push, PR) agregando un archivo propio, no que edite un archivo compartido donde todos pisan los cambios de todos.

## Implementación

`cybercba::core::CreditsLoader::loadEntries("credits")` (`include/cybercba/core/CreditsLoader.hpp`, `src/core/CreditsLoader.cpp`) lee esta carpeta con `std::filesystem`, sin depender de raylib — testeado en `tests/core/CreditsLoaderTest.cpp`. `Game` la carga una vez en su constructor; `Renderer` la dibuja cuando el estado actual es `GameStateId::Credits`.
