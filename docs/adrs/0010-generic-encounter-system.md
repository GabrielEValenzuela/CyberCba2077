# ADR 0010: Motor de encuentros por turnos genérico

Combate, hacking y recuperación de sistemas comparten la misma forma (participantes, orden de turno, acciones con costo/objetivo, efectos de estado, resultado) aunque su significado narrativo difiera. En vez de tres motores separados, se implementa un único `Encounter` (`include/cybercba/Encounter.hpp`, `src/domain/Encounter.cpp`) parametrizado por `EncounterType` y configurado con listas de `EncounterParticipant`/`ActionDefinition`.

`HackingEncounter`/`CombatEncounter`/`SystemRecoveryEncounter` (Phase 3) serán configuraciones de datos sobre este motor (nombres de recursos, acciones, participantes), no subclases con lógica propia. El motor no depende de raylib; toda la UI de encuentro (Phase 3) debe limitarse a leer `Encounter::participants()`/`turnOrder()`/`outcome()` y despachar `performAction`.

`ITurnQueue`/`IActionHistory` (spec §33) quedan como puntos de extensión documentados, no implementados en esta fase — ver `docs/academic-integration.md` (pendiente, Phase 5).
