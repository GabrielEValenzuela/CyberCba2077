# CLAUDE.md — CyberCBA 2077

Antes de cualquier tarea, **leé `AGENTS.md`** — es la fuente general de reglas del repositorio (arquitectura, restricciones académicas, dirección artística, pipeline de assets, build, testing, worktree). Este archivo no repite ese contenido: define el flujo y comportamiento específico de Claude Code trabajando sobre este proyecto.

## Flujo obligatorio

1. Leer `AGENTS.md`.
2. Leer los Markdown relevantes en `docs/` para el área afectada (tabla de estados en `AGENTS.md` §2 indica qué está vigente vs. histórico/scratch).
3. Inspeccionar el código real afectado (no asumir desde la documentación).
4. Contrastar documentación vs. código — si difieren, el código manda; señalar la discrepancia.
5. Para tareas grandes: registrar un plan breve (usar `ExitPlanMode`/tareas) antes de implementar.
6. Implementar — no quedarse solo en el plan.
7. Mantener el proyecto compilable en todo momento (ver comandos reales en `AGENTS.md` §9).
8. Ejecutar validaciones reales (build, `ctest --test-dir build/dev --output-on-failure`) antes de reportar éxito.
9. Actualizar la documentación afectada en el mismo cambio.
10. Entregar el reporte final (formato en §7 de este archivo).

## Uso del contexto

- No asumas que un sistema existe porque se menciona en un doc de `docs/` — verificá contra `CMakeLists.txt` y el header/`.cpp` real. Ejemplo conocido: `SceneManager`/`IScene` existen como archivos pero **no se compilan** en `cyber_cba` — el flujo activo es `SceneStack`.
- Antes de crear un loader, manager, widget o servicio nuevo, buscá si ya existe uno equivalente (`AssetStore`, `AudioService`, `InputRouter`, `SceneStack`, `GameController`, `SaveService`). No dupliques.
- Localizá primero la interfaz/contrato existente (headers en `include/cybercba/`, `src/ui/IScene.hpp` vs `SceneStack.hpp`) antes de agregar una nueva.
- Verificá nombres de targets (`cyber_cba`, `cyber_cba_core`, `cyberpunk_model`, `unit_tests`, `benchmarks_run`) y rutas reales antes de referenciarlos en comandos o docs.

## Tareas de UI y gameplay

- Revisá capturas/referencias que el usuario aporte, y `docs/art-direction.md`, antes de tocar escenas o widgets.
- Diferenciá claramente greybox (rectángulos placeholder) de arte terminado — no declares una UI "completa" solo porque compila y navega.
- Verificá que la pantalla resultante se vea como parte de un videojuego, sin labels de debug visibles fuera del perfil desarrollo (ver `AGENTS.md` §8).
- Prestá atención a escala, composición, foco de cámara, colisiones e interacción real, no solo a que el código compile.
- Confirmá que los cambios respeten a Emma (cian, hacking) y Magga (ámbar/rojo apagado, combate) según `docs/character-design.md`.
- Probá perfil producción y perfil desarrollo por separado (`./scripts/build.sh --run` vs `./scripts/build.sh --dev --run`) — no asumas que lo que ves en dev es lo que verá el jugador final.

## Assets

No hagas:
- Generar arte falso con rectángulos si el asset real ya existe en `assets/` o en el manifest.
- Ignorar `assets/data/assets-manifest.json`.
- Aplicar filtros de suavizado (bilinear) — el proyecto usa `TEXTURE_FILTER_POINT` en todo el gameplay pixel art.
- Cargar la misma textura repetidas veces por fuera de `AssetStore`.
- Hardcodear pivotes en escenas cuando el manifest ya declara `pivot`.
- Incorporar recursos sin licencia verificada y registrada.
- Borrar assets originales/raw una vez procesados.

Preferí: metadata del manifest, pivotes declarativos, assets ya procesados en `assets/processed/` (o equivalentes en `assets/textures/`), fallbacks explícitos, y los loaders existentes (`AssetStore`, `AudioService`).

## C++

- C++17 (`CMakeLists.txt`), sin sanitizers locales configurados (sí en CI).
- Compilar con warnings visibles; no silenciarlos con `-w`.
- RAII, ownership claro, `const` correcto (ver `CONTRIBUTING.md`).
- `enum class` sobre enums crudos (patrón ya usado: `GameCommand`, `SaveLoadStatus`, `AbilityResult`).
- Tipos explícitos, headers mínimos, evitar macros.
- No mezclar raylib con dominio: dominio/aplicación (`src/domain/`, `src/application/`) deben seguir siendo testeables sin raylib, como ya lo son `Campaign.cpp`/`GameController.cpp`.
- No introducir STL en estructuras académicas nuevas (Queue/Stack/etc. sin STL — ver `AGENTS.md` §5); STL sí es válido en infraestructura/UI.
- No romper `GameModel` (`include/model/GameModel.hpp`) sin necesidad real — es un placeholder mínimo intencional, no lo "completes" salvo que la tarea lo pida explícitamente.

## Cambios amplios

- Dividir en incrementos verificables; no dejar el repo sin compilar entre pasos.
- Validar (build + `ctest`) después de cada bloque importante, no solo al final.
- Evitar reestructurar todo si la tarea no lo requiere.
- Si tomás una decisión arquitectónica relevante (nuevo patrón, cambio de contrato entre capas), registrala como ADR nuevo en `docs/adrs/000N-*.md`, siguiendo el formato de los 6 existentes.

## Worktree y seguridad

- Ejecutar `git status` antes de empezar cualquier tarea.
- Preservar cambios existentes; no usar `git reset --hard`, `git checkout .` ni comandos destructivos.
- No borrar archivos ajenos al alcance de la tarea.
- No hacer commit salvo pedido explícito del usuario.
- Informar inmediatamente si el worktree tiene archivos modificados previamente o conflictos antes de tocar nada.

## Reporte final esperado

```
Resumen
Implementación
Arquitectura
Archivos creados
Archivos modificados
Tests ejecutados
Resultado de build
Verificación manual
Limitaciones
Pendientes no bloqueantes
```

No afirmes que algo fue compilado, probado o verificado visualmente si no ocurrió realmente en la sesión.
