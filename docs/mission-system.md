# Misiones y objetivos

`CampaignProgress` contiene estado de misión, objetivo, etapa, checkpoint y desbloqueos. `MissionSystem` permite iniciar y avanzar sólo desde la etapa esperada: `Shelter → Transmission → Route → Convergence → Epilogue → Complete`. La v0.1.0 utiliza el sistema en cada transición y desbloquea el capítulo anunciado, no su contenido.

El siguiente paso es materializar `MissionDefinition` desde `assets/data/missions/`, manteniendo el mismo estado persistido y la misma API de condiciones.

## MissionGraph (Phase 2: contenido del prólogo conectado)

`MissionGraph` (`include/cybercba/MissionGraph.hpp`) es la estructura genérica de nodos/transiciones que reemplazará gradualmente el uso de `PrologueStage` como fuente del recorrido — ver ADR `0009-node-based-mission-graph.md`. `src/domain/PrologueContent.cpp` (`buildPrologueMissionGraph()`) puebla los nodos reales de "La última transmisión": `refuge` (con `rest_area`/`workshop`/`comms_room`/`archive` opcionales) → `blackout` → `power_recovery` (bloqueado por el flag `power_restored`) → `fragmented_transmission` → `protagonist_encounter` → `narrative_choice` → `exterior_route` → `security_control` → `neometro_entrance` → `prologue_complete`.

**Estado real**: `GameSession::startPrologue()` construye este grafo y lo persiste en `SaveService` (v5: `mission_node`, `mission_flags`). Desde ADR `0014-node-based-world-screen.md`, `Screen::World` es el mapa de nodos real — `GameApp::nodeOptions()`/`selectNodeOption()` leen y avanzan `MissionGraph` directamente. `MissionSystem`/`CampaignProgress`/`PrologueStage` no desaparecieron: siguen persistiendo el estado de guardado grueso (checkpoint, salud/stamina heredadas, capítulo desbloqueado) y se actualizan en paralelo en los mismos puntos de transición del grafo (ver `GameApp::selectNodeOption`), pero ya no son lo que el jugador ve — eso es `MissionGraph`.
