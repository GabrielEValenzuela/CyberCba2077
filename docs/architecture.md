# Arquitectura

```text
GameApp (raylib, render virtual, navegación)
 ├─ InputRouter / AudioService / DevOverlay
 ├─ SaveService
 └─ GameSession
    ├─ CampaignProgress + NarrativeState + PlayerState
    ├─ MissionSystem / AbilitySystem
    └─ GameModel (dominio académico independiente)
```

`Campaign.cpp` concentra reglas puras y testeables: definiciones de personajes, capacidades, salud/stamina/invulnerabilidad y transición válida de la misión. `WorldLayout` concentra el catálogo runtime de props, capas, pivotes, colliders e interacciones del prototipo top-down previo (ver ADR `0008-narrative-rpg-pivot.md` — deja de ser el flujo principal). `SaveService` es infraestructura versionada (v4), con escritura temporal.

La UI usa una máquina de pantallas y modales (`Screen`/`Modal` en `GameApp`) para esta release. `SceneStack`/`IScene`/`SceneManager` están **deprecados** — compilados pero sin ningún símbolo referenciado por `GameApp`/`main_ui.cpp` (ver ADR `0011-deprecate-scenestack.md`); no los trates como el mecanismo activo de transición. Ninguna escena contiene la implementación de hacking o combate. El recorrido y los componentes de la UI están documentados en [ui.md](ui.md).

## Fundación RPG narrativo (Phase 1) + contenido del prólogo (Phase 2)

Se agregó una capa de dominio reutilizable, raylib-free (ver ADR `0008-narrative-rpg-pivot.md`):

```text
cyber_cba_core (sin raylib)
 ├─ MissionGraph    — nodos + transiciones condicionadas por flags (ADR 0009)
 ├─ DialogueGraph   — líneas con variante por protagonista, hablante resuelto (Emma/Magga/Narrador/NPC)
 ├─ EvidenceJournal — catálogo de evidencia + descubrimiento sin duplicados
 ├─ Encounter       — motor de turnos genérico (combate/hacking/recuperación de sistemas) (ADR 0010)
 └─ PrologueContent — contenido real de "La última transmisión" sobre los cuatro tipos anteriores
```

`NarrativeState` se extendió de 3 a 5 campos (`trust`, `unresolvedHurt`, `recoveredTruth`, `caution`, `attachment`); `SaveService` está en formato v5 (nodo/flags de `MissionGraph` + evidencia descubierta) con compatibilidad de lectura para v2–v4 (campos nuevos quedan en su valor por defecto).

`GameSession::startPrologue()` construye el `MissionGraph`/`EvidenceJournal` de la campaña real vía `PrologueContent`. Desde ADR `0014-node-based-world-screen.md`, **`Screen::World` es el mapa de nodos real**: `GameApp::nodeOptions()`/`selectNodeOption()`/`nodeBody()` leen y avanzan `MissionGraph` y muestran texto de `DialogueGraph`, reemplazando el movimiento WASD/colisión que tenía el prototipo top-down. Investigación (`rest_area`/`workshop`/`comms_room`/`archive`) descubre evidencia al llegar al nodo (`GameApp::enterNode`).

### Librería de componentes UI reutilizables (`src/ui/widgets/`)

Empezó a extraerse de la lógica inline de `GameApp` una capa de widgets reutilizables entre pantallas (ver `docs/mission-screen-templates.md`):

- `EvidencePanel` — chip de evidencia (thumbnail + título); usado en `drawObjectives()` (bitácora/journal) y `drawEnd()` (resultados de misión).
- `StoryPanel` — template "story/prólogo" (6.1): label + retrato + texto; usado en `drawIntro()`.
- `ChoicePanel` — template "diálogo con elección" (6.3): hablante + texto + fila de respuestas; usado en el nodo `narrative_choice` de `drawWorld()`.
- `ActionMenu` — template "selección de acción" (6.2): lista vertical de botones; `GameApp::drawButton` delega en `ActionMenu::drawItem` (usado en menú, pausa, config, mundo).
- `NodeMapView` — template "mapa de nodos" (7.1): franja horizontal con nodos/conexiones/estado real, construida desde `MissionGraph::nodes()` (accessor nuevo).
- `TurnOrderPanel`/`ParticipantPanel`/`StatusPanel`/`EnemyIntentPanel` — template "encuentro por turnos" (7.2), usados en `drawEncounter()`; la intención enemiga (`EnemyIntentPanel`) reutiliza la misma IA determinista (`GameApp::nextEnemyActionId()`) que ejecuta `updateEncounter()`, sin duplicar la lógica de decisión.
- `MissionResultsView` — template "resultados de misión" (7.4), usado en `drawEnd()`; compone `EvidencePanel` para la evidencia recuperada.
- `TransmissionPanel` — panel de "transmisión fragmentada" (barra de señal + ruido procedural), usado en el nodo `fragmented_transmission` de `drawWorld()`.
- `DialoguePanel` — línea de diálogo sin elección (distinto de `ChoicePanel`), usado en `Modal::Dialogue`.
- `TextWrap` — wrap de texto compartido (antes duplicado dentro de `GameApp.cpp`), usado por los widgets de arriba y por `GameApp::drawWorld()`.

Con esta sesión, todos los templates de pantalla listados en `docs/mission-screen-templates.md` tienen al menos una implementación real wireada en `GameApp` — ver ese documento para el detalle fila por fila y los gaps de contenido/arte restantes (no de arquitectura).

**Actualización (ADR `0017-cinematic-ui-pivot.md`)**: el pivot pixel-art de ADR `0016` quedó superseded para el flujo narrativo — el filtrado pasa a bilinear por defecto (`AssetStore::texture`) y el layout usa `UiMetrics` para ocupar casi todo el canvas de 1280×720 en vez de cajas pequeñas centradas. Se agregaron `CurrentObjectivePanel`, `PortraitFrame` e `InteractionPanel`, wireados en `GameApp::drawWorld()`/`drawModal()`. Ver `docs/mission-screen-templates.md` para el detalle y los gaps de retrato/fondo cinemático pendientes.

## Encuentros por turnos (Phase 3)

`Screen::Encounter` (`src/app/GameApp.{hpp,cpp}`) es una pantalla real de encuentro por turnos sobre el motor `Encounter` de Phase 1, configurado con contenido real vía `EncounterContent` (`buildEmmaHackingEncounter()`, `buildMaggaCombatEncounter()`) — ver ADR `0012-encounter-screen-replaces-realtime-drone-hacking.md`. Reemplaza la barra de progreso de hacking y el golpe único en tiempo real contra el dron que tenía el prólogo original. La IA enemiga es mínima y determinista (suficiente para el prólogo, no un `EncounterAI` general); los efectos de estado se aplican/expiran correctamente pero todavía no tienen efecto mecánico (mitigación de daño, etc.) — son marcadores narrativos en esta fase.
