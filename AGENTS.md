# AGENTS.md — CyberCBA 2077

Reglas generales del repositorio para **cualquier agente de programación** (Claude Code, Codex, Copilot, u otro). Independiente de herramienta.

> Nota de precisión: "Exodus Systems" aparece únicamente como texto de sabor diegético en el Splash y como crédito en el Menú (`src/app/GameApp.cpp`, pantallas Splash/Menu — "AN EXODUS SYSTEMS PRODUCTION"). No hay ningún sistema, clase ni entidad narrativa con ese nombre en el código o los docs; no lo trates como un módulo o mecánica real.

## 1. Qué es CyberCBA 2077

RPG narrativo por turnos en C++17/raylib: exploración por nodos, investigación, diálogo y encuentros por turnos — ver `docs/campaign-bible.md` y ADR `0008-narrative-rpg-pivot.md`. Versión actual: **v0.1.0 — "La última transmisión"** (ver `vcpkg.json`, `README.md`). El prototipo top-down original (movimiento libre, colisión) fue reemplazado en `Screen::World` por un mapa de nodos real (ADR `0014-node-based-world-screen.md`); `WorldLayout` se preserva solo como fondo ilustrado procedural, sin colisión ni movimiento.

- Escenario: Neo-Córdoba 2077, "La ciudad que nos olvidó".
- El jugador elige entre **Emmanuel "Emma" Vázquez** (intrusión de enlace / hacking) o **Magalí "Magga" Temerro** (impacto táctico / combate). La elección cambia quién responde la transmisión, la ruta del prólogo y el encuentro por turnos que resuelve el obstáculo (`Screen::Encounter`); ambas rutas convergen en la entrada clausurada del Neometro.
- **Producto jugable vs. capa académica**: la capa académica (estructuras de datos, tests, benchmarks) queda fuera de la ficción y **no aparece en la UI del jugador**. `QueueChallenge` (`include/cybercba/QueueChallenge.hpp`, `src/domain/QueueChallenge.cpp`) es un dominio educativo de referencia — no es contenido de la campaña v0.1.0 (ver `docs/educational-design.md`, ADR `0003-domain-boundary.md`).
- Objetivo de v0.1.0: prólogo jugable completo (selección de personaje → intro → mapa de nodos del refugio → investigación → apagón → recuperación de energía → transmisión fragmentada → encuentro por turnos → elección narrativa → ruta exterior → control de seguridad → entrada Neometro → teaser de la próxima versión). v0.2.0 ("Neometro: El último convoy") es **planeado**, no implementado — incluirá las estructuras Queue/Stack académicas sin STL (`docs/roadmap.md`).

## 2. Regla de lectura previa

Antes de modificar código, todo agente debe leer:

- Este `AGENTS.md`.
- `README.md`.
- Los Markdown relevantes en `docs/` para el módulo afectado (ver tabla en §3).
- Los ADRs en `docs/adrs/` (`0001`–`0006`).
- Configuración y tests relacionados con el cambio.

Si el cambio altera arquitectura, comportamiento, controles, assets, formato de datos, comandos de build o flujo de misión, **la documentación correspondiente debe actualizarse en el mismo cambio**.

Documentos con contenido **desactualizado, marcado explícitamente como histórico, o scratch** — no tratarlos como fuente vigente sin verificar contra el código:

| Documento | Estado |
|---|---|
| `docs/ARQUITECTURA_UI.md` | Histórico — describe el `SceneManager` previo al vertical slice. Auto-declarado obsoleto en su primera línea. |
| `docs/RELEVAMIENTO.md` | Snapshot previo al vertical slice, superado por `docs/architecture.md`. |
| `docs/prompt.md`, `docs/prompt2.md` | Prompts de trabajo (scratch) para integración de assets, no documentación de arquitectura viva. |

## 3. Arquitectura

Fuente autoritativa: `docs/architecture.md` + ADRs. Estado real confirmado en `CMakeLists.txt` (qué se compila) y headers en `include/cybercba/`.

| Componente | Estado | Ubicación | Rol |
|---|---|---|---|
| `GameApp` | Implementado | `src/app/GameApp.{hpp,cpp}` | Orquestador real: render raylib, navegación entre pantallas (`Screen` enum: Splash/Menu/CharacterSelection/Intro/World/…), posee `InputRouter`, `AudioService`, `DevOverlay`, `SaveService`, `GameSession`. |
| `GameSession` | Implementado | `include/cybercba/GameSession.hpp`, `src/domain/GameSession.cpp` | Contenedor de `CampaignProgress`, `NarrativeState`, `PlayerState`, `AccessibilitySettings`, `AudioSettings`; expone `model()`, `progress()`, `startNewGame()`, `recordQueueResult()`. |
| `GameController` | Implementado | `include/cybercba/GameController.hpp`, `src/application/GameController.cpp` | Capa de aplicación: recibe `GameCommand` (enum: NewGame, ContinueGame, QueuePredict*, QueueExecute, QueueHint, QueueUndo, QueueRestart, QueueAdvanceDebrief) y despacha sobre `GameSession`. |
| `Campaign` (dominio) | Implementado | `include/cybercba/Campaign.hpp`, `src/domain/Campaign.cpp` | Reglas puras: `CharacterDefinition`, `PlayerState`, `NarrativeState`, `CampaignProgress`, `AbilitySystem` (hack/strike/applyDamage), `MissionState`/`PrologueStage`. Testeable sin raylib. |
| `QueueChallenge` | Implementado (educativo, fuera de ficción) | `include/cybercba/QueueChallenge.hpp`, `src/domain/QueueChallenge.cpp` | FIFO de 4 slots sobre `std::array` fijo (no STL dinámico), fases Intuition/Prediction/Guided/Independent/Debrief/Complete, undo por snapshots. |
| `SaveService` | Implementado | `include/cybercba/SaveService.hpp`, `src/infrastructure/SaveService.cpp` | Persistencia `key=value` versionada (`version=3`), escritura a `.tmp` + rename, estados `Loaded/Missing/Corrupt/UnsupportedVersion/IoError`. |
| `GameModel` | Implementado (placeholder mínimo) | `include/model/GameModel.hpp`, `src/model/GameModel.cpp` | `addCredits`/`credits()` — marcado explícitamente como placeholder en el propio header. |
| `SceneStack` | Deprecado (ADR `0011-deprecate-scenestack.md`) | `src/ui/SceneStack.{hpp,cpp}` | Compilado en `cyber_cba` pero **ningún símbolo es referenciado por `GameApp`/`main_ui.cpp`** — código muerto en la práctica, no el flujo activo real (ese es el `Screen` if/else dentro de `GameApp`). |
| `SceneManager` / `IScene` | Deprecado / no compilado | `src/ui/SceneManager.{hpp,cpp}`, `src/ui/IScene.hpp` | Existe en el árbol pero **no está en las fuentes de `cyber_cba` en `CMakeLists.txt`**. No lo referencies como el flujo activo. |
| `MissionGraph` | Implementado; poblado con el prólogo real, sin UI propia | `include/cybercba/MissionGraph.hpp`, `src/domain/MissionGraph.cpp` | Grafo de nodos de misión genérico (`MissionNode`, transiciones condicionadas por flags). `GameSession::startPrologue()` lo construye vía `PrologueContent`; persiste en `SaveService` v5. No reemplaza `MissionSystem`/`PrologueStage` como recorrido real de `GameApp` todavía — ver ADR `0009-node-based-mission-graph.md`. |
| `DialogueGraph` | Implementado; contenido real del prólogo, parcialmente conectado a `GameApp` | `include/cybercba/Dialogue.hpp`, `src/domain/Dialogue.cpp` | Líneas de diálogo con variantes por protagonista, resolución de hablante (`Protagonist`/`OtherProtagonist`/`Npc`/`Narrator`). `GameApp::interact()` la usa para `inspect_photo`/`inspect_map`; el resto del texto de `GameApp` (intro, transmisión) sigue siendo fijo. |
| `EvidenceJournal` | Implementado; catálogo real del prólogo, conectado a `GameApp::interact()`/`drawObjectives()` | `include/cybercba/Evidence.hpp`, `src/domain/Evidence.cpp` | Catálogo de evidencia, descubrimiento con prevención de duplicados. Persiste en `SaveService` v5. |
| `Encounter` | Implementado; con configuración real de hacking/combate del prólogo | `include/cybercba/Encounter.hpp`, `src/domain/Encounter.cpp` | Motor de turnos compartido por combate/hacking/recuperación de sistemas — ver ADR `0010-generic-encounter-system.md`. |
| `PrologueContent` | Implementado | `include/cybercba/PrologueContent.hpp`, `src/domain/PrologueContent.cpp` | Contenido real de "La última transmisión" (nodos, diálogo, evidencia) sobre los cuatro tipos anteriores. |
| `EncounterContent` | Implementado | `include/cybercba/EncounterContent.hpp`, `src/domain/EncounterContent.cpp` | Configuración real del hacking de Emma y el combate de Magga sobre `Encounter` — ver ADR `0012-encounter-screen-replaces-realtime-drone-hacking.md`. |
| `CharacterModule` / `CharacterRoster` | Implementado (andamiaje); contenido de los 9 personajes es TODO(student) | `include/cybercba/CharacterModule.hpp`, `src/domain/CharacterModule.cpp`, `include/cybercba/characters/*.hpp`, `src/domain/characters/*.cpp` | Contrato para que cada estudiante implemente uno de los 9 NPC/enemigos de v0.1.0 (Luma, Sistema, voces fabricadas del apagón, relé/dron de encuentro, vigilancia/patrulla de `security_control`, cameo de Tomo). Roster construido por `buildPrologueCharacterRoster()`, **no cableado todavía** a `MissionGraph`/`GameApp` — ver `docs/adding-a-character.md`. |
| `Screen::Encounter` (`GameApp`) | Implementado | `src/app/GameApp.{hpp,cpp}` (`drawEncounter`/`updateEncounter`/`startEncounter`) | Pantalla real de encuentro por turnos: orden de turno, lista de acciones con costo, integridad/recurso de jugador y enemigo, tensión. Reemplazó `Modal::Hacking` (barra de progreso) y el golpe único en tiempo real contra el dron. |
| `Screen::World` (`GameApp`) | Implementado — mapa de nodos, no mundo libre | `src/app/GameApp.{hpp,cpp}` (`nodeOptions`/`selectNodeOption`/`nodeBody`/`updateWorldNode`) | Reemplazó el movimiento WASD/colisión del prototipo — ver ADR `0014-node-based-world-screen.md`. El jugador elige destinos/acciones de `MissionGraph` desde una lista; el shell de `WorldLayout` queda como fondo ilustrado procedural, ya no colisiona ni requiere posición de jugador. |
| Escenas concretas | Implementado | `src/ui/scenes/{SplashScene,MainMenuScene,MapScene,HUDScene,InventoryScene}` | Compiladas en `cyber_cba`. |
| Widgets UI | Implementado | `src/ui/widgets/{NlmButton,NlmPanel,NlmText,DevOverlay}` | `DevOverlay` es exclusivo de perfil desarrollo. |
| Input | Implementado | `src/ui/InputRouter.{hpp,cpp}` | Enrutamiento de input hacia escena activa/comandos. |
| Assets | Implementado | `src/ui/AssetStore.{hpp,cpp}` | Carga fuentes/texturas, aplica `TEXTURE_FILTER_POINT`. |
| Audio | Implementado | `src/ui/AudioService.{hpp,cpp}` | Falla silenciosamente si falta un recurso. |
| Config | Implementado | `src/config/GameConfig.{hpp,cpp}`, `config/*.cfg` | `key=value`, perfiles producción/desarrollo (ver §7). |
| Misiones (`MissionSystem`) | Parcial | `docs/mission-system.md` | Stage machine `Shelter→Transmission→Route→Convergence→Epilogue→Complete` sobre `CampaignProgress`. `MissionDefinition` cargado desde `assets/data/missions/` es **planeado**. |
| Diálogos | Parcial | `docs/dialogue-system.md` | Depende de `selectedCharacter`/`otherCharacter`; formato de datos declarativo (speaker/text/conditions/options) es **planeado**. |
| Telemetría | Implementado (solo dev) | `src/ui/widgets/DevOverlay.cpp` | Muestreo cada 500ms, visible con F3 solo en perfil desarrollo. |

No menciones componentes como implementados si no están compilados/enlazados — verificá siempre contra `CMakeLists.txt` primero.

## 4. Reglas de diseño

- UI sin lógica de dominio: escenas y widgets solo dibujan/leen estado y emiten comandos/acciones (ver `GameCommand` en `GameController.hpp`).
- Estado centralizado en `GameSession`; sin variables globales de gameplay.
- RAII, ownership explícito, `const` correcto (ver `CONTRIBUTING.md` §"C++ Development Guidelines").
- Evitar `new`/`delete` crudos innecesarios; evitar God Objects; evitar dependencias circulares; evitar valores mágicos.
- Preservar APIs públicas (headers en `include/cybercba/`) salvo justificación documentada.
- El proyecto debe permanecer compilable durante refactors — no dejar el árbol roto entre pasos.

## 5. Restricciones académicas

Las estructuras educativas (Queue, Stack, listas, árboles, heaps, grafos) **deben poder implementarse sin STL** — ver ADR `0003-domain-boundary.md` y el propio `QueueChallenge.cpp`, que usa `std::array<std::string,4>` fijo, no un contenedor dinámico STL.

- Infraestructura, UI, assets, persistencia y herramientas **sí pueden usar STL** libremente.
- Al agregar una estructura nueva para el "Neometro" (v0.2.0), no reutilizar los contenedores de v0.1.0 como solución oculta — cada estructura académica nueva debe implementarse desde cero por quien la asigne (ver `docs/adding-a-mission.md`).
- No incluir soluciones ocultas que reemplacen las implementaciones de estudiantes.

## 6. Dirección artística

Resumen — fuente completa y autoritativa: **`docs/art-direction.md`**.

- Estilo: ilustración narrativa 2D semi-realista + pixel art top-down en gameplay.
- Gameplay: resolución virtual 1280×720, tiles 32×32px, personajes 48×64px, animación 8–12 FPS, filtrado **`TEXTURE_FILTER_POINT`** (aplicado en `AssetStore`).
- Paleta: base fría con cian de ambiente; acentos cian (Emma/hacking) y ámbar-rojo apagado (Magga/combate).
- Personajes humanos con tonos naturales (ver `docs/character-design.md`).
- Assets reemplazables — manifest obligatorio: `assets/data/assets-manifest.json` (esquema: `id`, `path`, `type`, dimensiones, `pivot`, `license`, `author`, `source`, `placeholder`).
- No incorporar assets externos sin licencia verificada y registrada en el manifest.
- **Thiings**: solo como referencia/placeholder temporal para objetos de inventario (terminales, radios, llaves, baterías, medkits); sus íconos deben redibujarse/adaptarse (paleta limitada, nearest-neighbor, contorno común) — no usarse directamente sin una decisión futura documentada y licencia verificada.
- No presentar rectángulos de greybox como arte terminado.

## 7. Pipeline de assets

Fuente completa: `docs/assets.md`, `docs/art-direction.md`.

- Rutas reales: `assets/{fonts,textures/{backgrounds,characters,maps,ui},audio/{music,ambience,ui},shaders,data/{campaigns,missions,localization}}`.
- Raw → processed: `scripts/process_assets.sh` toma `assets/raw/{characters,props,buildings,environment}` y genera PNGs en `assets/processed/...` con resize por point-sampling, preservando alpha.
- `scripts/validate_assets.sh` valida un catálogo fijo de PNGs procesados y sus dimensiones exactas (ej. `assets/processed/characters/emma_idle.png:256x384`).
- Manifest obligatorio (`assets/data/assets-manifest.json`): cada entrada define `id`, `path`, `type`, `pivot`, `license`, `author`, `source`, `placeholder`.
- `AssetStore` centraliza carga de fuentes/texturas y aplica `TEXTURE_FILTER_POINT`; no cargar la misma textura repetidas veces por fuera de este loader.
- Para integrar un asset nuevo: (1) colocarlo en `assets/raw/...`, (2) correr `scripts/process_assets.sh`, (3) validar con `scripts/validate_assets.sh`, (4) registrar entrada en `assets-manifest.json` con licencia/autor/fuente, (5) referenciarlo desde `AssetStore`/escena vía `id` de manifest, no hardcodeando paths.

## 8. Producción y desarrollo

Diferencias reales (`config/game.cfg` vs `config/development.cfg`):

| Clave | `game.cfg` (producción) | `development.cfg` (desarrollo) |
|---|---|---|
| `window_title` | CYBERCBA 2077 | CYBERCBA 2077 [DEV] |
| `splash_seconds` | 2.0 | 0.5 |
| `ui_mode` | production | development |
| `save_path` | `cybercba.save` | `cybercba-dev.save` |

- `DevOverlay` (F3), hitboxes/triggers de debug y telemetría interna solo deben estar visibles/activos en perfil desarrollo (`ui_mode=development`).
- Nada técnico (nombres de estructuras académicas, IDs internos, logs de debug) debe filtrarse a la experiencia de producción.

## 9. Build

Comandos reales (verificados contra `CMakeLists.txt`, `CMakePresets.json`, `scripts/build.sh`):

```bash
# Vía presets (usa VCPKG_ROOT):
cmake --preset dev && cmake --build build/dev
cmake --preset release && cmake --build build/release

# Vía script (build a ./build/, no build/dev):
./scripts/build.sh --run
./scripts/build.sh --dev --run       # usa config/development.cfg

# Tests (asume workflow de presets, build/dev):
ctest --test-dir build/dev --output-on-failure

# Ejecutar con config explícita:
cyber-cba --config ruta/game.cfg
```

⚠️ **Discrepancia real detectada**: `scripts/build.sh` configura y compila en `build/` plano (target `cyber_cba` únicamente), sin usar `CMakePresets.json`. El `ctest --test-dir build/dev` de README/`docs/testing.md` asume el workflow de presets. Si usás `build.sh`, los tests (`unit_tests`) no se generan en esa carpeta — usá el flujo de presets para tests/benchmarks.

Targets reales (`CMakeLists.txt`):
- `cyberpunk_model` (STATIC lib) — `GameModel`.
- `cyber_cba_core` (STATIC lib) — dominio/aplicación/infraestructura.
- `cyber_cba` (ejecutable, nombre de salida `cyber-cba`) — solo si `raylib_FOUND`.
- `unit_tests` — solo si GTest disponible.
- `benchmarks_run` — solo si Google Benchmark disponible.

Opciones CMake: `CYBERPUNK_BUILD_TESTS`, `CYBERPUNK_BUILD_BENCHMARKS`, `CYBERPUNK_ENABLE_COVERAGE` (ON por defecto, agrega `--coverage`), `CYBERPUNK_BUILD_GAME`.

No hay sanitizers configurados en `CMakeLists.txt` local; ASan+UBSan sí se usan en `.github/workflows/ci.yml` (solo CI).

## 10. Testing

- Framework: **GoogleTest** (`unit_tests`) y **Google Benchmark** (`benchmarks_run`).
- Ubicación: `tests/test_gamemodel.cpp`, `tests/test_vertical_slice.cpp`; benchmarks en `benchmarks/bench_gamemodel.cpp`.
- Ejecutar: `ctest --test-dir build/dev --output-on-failure` (requiere haber compilado con preset `dev`).
- Casos esperados: normales y de borde — ver ejemplos reales en `test_vertical_slice.cpp` (`QueueChallenge.PreservesFifoAndUndo`, `SaveService.HandlesMissingCorruptAndRoundTrip`, `Campaign.CharactersHaveExclusiveAbilities`, etc.).
- Evitar tests de screenshots como estrategia principal; extraer lógica de escenas hacia `Campaign`/`GameSession`/`GameController` (dominio/aplicación) para que sea testeable sin raylib — este es justamente el patrón ya usado en `Campaign.cpp`/`GameController.cpp`.
- Conservar los tests y benchmarks existentes; no eliminarlos sin reemplazo equivalente.
- CI (`.github/workflows/ci.yml`) exige `COVERAGE_THRESHOLD=100` (líneas y funciones) — tenerlo en cuenta al agregar código nuevo sin cobertura.

## 11. Modificación de archivos

- Revisar dependencias antes de editar (qué target compila el archivo, qué headers lo incluyen).
- No borrar trabajo ajeno ni descartar cambios del usuario.
- No hacer commits salvo solicitud explícita.
- No modificar archivos no relacionados con la tarea.
- Correr `git status` antes de empezar; si el worktree tiene cambios sin commitear, informarlo antes de proceder.
- Evitar refactors masivos innecesarios — cambios incrementales, validando compilación después de cada bloque relevante.

## 12. Entrega de tareas

Todo agente debe reportar al finalizar:

- Qué cambió, con lista de archivos creados y modificados.
- Decisiones tomadas y por qué.
- Comandos ejecutados (build/test/format).
- Resultado real de tests (no afirmar que algo compiló o pasó si no se ejecutó).
- Limitaciones y pendientes reales.
- Elementos no verificados explícitamente marcados como tal.

---

Ver también `CLAUDE.md` para el flujo específico de Claude Code sobre este repositorio.
