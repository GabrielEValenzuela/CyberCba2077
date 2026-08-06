# ADR 0014: `Screen::World` pasa a ser el mapa de nodos, no el mundo libre

Hasta este cambio, `Screen::World` era exactamente el prototipo top-down original: movimiento WASD, colisión contra `WorldLayout`, un sprite de jugador, y `interact()` disparando por proximidad a props (`inspect_photo`, `use_terminal`, `face_drone`, etc.). `MissionGraph` (Phase 1-2) existía, tenía contenido real (Phase 2) y persistía en `SaveService` (Phase 2), pero **no impulsaba ninguna pantalla** — era fundación sin UI. El resultado observable: el juego seguía viéndose y jugándose exactamente igual que antes del pivot, salvo por los dos encuentros por turnos (Phase 3). Este era el gap más importante señalado al final de Phase 3.

## Decisión

`Screen::World` se repropone por completo: en vez de mover un personaje con WASD por un tilemap, el jugador ve el nodo actual (`m_session.missionGraph().current()`), su texto (`nodeBody()`, resuelto contra `DialogueGraph`/texto propio) y una lista navegable de opciones (`nodeOptions()`) — destinos disponibles (`MissionGraph::availableTransitions()`) o una acción específica del nodo (restaurar energía, enfrentar el obstáculo, elegir una respuesta, despejar el control de seguridad). Confirmar una opción llama `selectNodeOption()`, que aplica los efectos de dominio (descubrir evidencia, setear flags, avanzar `PrologueStage`, iniciar un `Encounter`) y mueve el grafo (`MissionGraph::advanceTo`).

Se mantiene como fondo decorativo el "shell" ilustrado existente (`drawShelterShell`/`drawExteriorShell`/`drawWorldProps`/`drawWorldLighting`/`drawWeather`/`drawWorldSignage`) — sigue siendo arte procedural placeholder, pero ahora cumple exactamente el rol de `Background` que pide la spec del pivot (§15), sin necesidad de arte nuevo para este cambio.

## Qué se eliminó

`m_playerPosition`, el movimiento WASD, la colisión contra `WorldLayout::blocksMovement`, `nearbyWorldProp()`/`prompt()` (proximidad a props), `drawWorldDebug()` (colliders de debug, ya sin sentido sin movimiento), y la barra de salud/stamina de tiempo real en el HUD (dependía de `AbilitySystem::update`/`hack`/`strike`, que ya no se invoca desde `GameApp` — esas funciones siguen existiendo y testeadas en `Campaign.cpp`/`test_vertical_slice.cpp`, solo dejaron de tener un caller real en la UI). `WorldLayout.hpp/.cpp` **no se tocó** — sigue siendo infraestructura válida y testeada (`tests/test_world_layout.cpp`), solo dejó de ser consultada para colisión/proximidad.

## Mapeo de nodo → escenario

Los nodos del refugio (`refuge`, `rest_area`, `workshop`, `comms_room`, `archive`, `blackout`, `power_recovery`, `fragmented_transmission`) usan el shell de `WorldZone::Shelter`; el resto (`protagonist_encounter`, `narrative_choice`, `exterior_route`, `security_control`, `neometro_entrance`, `prologue_complete`) usa `WorldZone::Exterior` — mismo backdrop que antes tenía el jugador cruzando desde `exit_shelter`.

## Checkpoints

`GameApp::resetCheckpoint()` mapea cada string de checkpoint (`shelter`/`transmission`/`street`/`service_gate`/`neometro_gate`) a un nodo del grafo y a los flags que sus transiciones hacia adelante requieren, usando `MissionGraph::setStart` (que no valida transición, a diferencia de `advanceTo`) para saltar directo sin tener que rejugar el camino.

## Limitación conocida

`power_recovery` sigue siendo una única acción de confirmación ("Restaurar energía"), no el encuentro de recuperación de sistemas por turnos que describe la spec §22 — ver `docs/roadmap.md`, sigue siendo un pendiente no bloqueante. `security_control` es igualmente una acción única por protagonista, no un obstáculo con múltiples opciones tácticas.
