# Sistema de turnos

`Encounter` (`include/cybercba/Encounter.hpp`, `src/domain/Encounter.cpp`) es el motor de turnos genérico compartido por combate, hacking y recuperación de sistemas — ver ADR `0010-generic-encounter-system.md`. Raylib-free, testeable sin ventana.

Ciclo real implementado: `start()` calcula el orden de turno por `initiative` descendente (`computeTurnOrder`). Cada `performAction(actorId, actionId, targetId)` valida que sea el turno del actor, que la acción exista, que el costo (`ActionCost{resource, amount}`) esté cubierto por `resourcePool` del actor, aplica `power` a la integridad del objetivo (clamp a `[0, maxIntegrity]`), marca `defeated` si llega a 0, aplica el `StatusEffect` si la acción lo define, y avanza el turno (`advanceTurn`) saltando participantes derrotados. Al completar una vuelta completa del orden de turno se incrementa `round()` y se descuentan los `remainingTurns` de cada estado activo (`tickStatuses`), removiendo los expirados.

`outcome()` se recalcula bajo demanda: `Defeat` si no queda ningún participante `isPlayerControlled` vivo, `Victory` si no queda ningún enemigo vivo, `InProgress` en otro caso. `tension()` es un contador aparte (`addTension`) que el contenido de cada encuentro puede usar para su propia semántica (ej. Trace en hacking) — el motor no le da significado propio.

**Limitación conocida (documentada, no oculta)**: los `StatusEffect` se aplican y expiran correctamente, pero no tienen todavía efecto mecánico automático (ej. `guarding` no reduce daño recibido) — son marcadores de datos; aplicar su efecto es responsabilidad de quien configure el encuentro (`EncounterContent`) o de una futura extensión del motor, no está implementado en v0.1.0.

Tests: `tests/test_encounter.cpp` (orden de turno, validación de costo, daño, expiración de estado, derrota/victoria, rechazo de acciones tras resolver el encuentro).
