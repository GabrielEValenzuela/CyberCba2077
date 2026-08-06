# Sistema de encuentros

Un encuentro es una instancia de `cybercba::Encounter` (ver `docs/turn-system.md`) configurada con participantes y acciones concretas. La configuración vive en `EncounterContent` (`include/cybercba/EncounterContent.hpp`, `src/domain/EncounterContent.cpp`), no en el motor — agregar un encuentro nuevo no requiere tocar `Encounter.{hpp,cpp}`.

## Encuentros reales de v0.1.0

- **Hacking de Emma** (`buildEmmaHackingEncounter`): Emma (100 integridad, 60 `link`) contra un "Rele de interferencia" (60 integridad). Acciones: Analizar (gratis), Aislar ruido, Redirigir señal, Ocultar rastro (aplica estado `hidden`), Extraer fragmento; el rele responde con `relay_pulse`.
- **Combate de Magga** (`buildMaggaCombatEncounter`): Magga (100 integridad, 60 `momentum`) contra un "Dron de seguridad" (50 integridad). Acciones: Golpe, Guardia (`guarding`), Esquivar (`evasive`), Impacto táctico (`guard_broken`); el dron alterna `charge_shot`/`rush` por paridad de ronda.

## Integración con `GameApp` (Phase 3)

`Screen::Encounter` (`src/app/GameApp.{hpp,cpp}`) es la UI real: `interact()` en los hotspots `use_terminal` (Emma, etapa Route) y `face_drone` (Magga, etapa Route) llama `startEncounter(...)`, que construye el `Encounter`, lo arranca y cambia de pantalla. `updateEncounter()` resuelve el turno del enemigo automáticamente (una acción fija por tipo de encuentro) y deja que el jugador navegue/confirme una acción en su turno; `Cancel` aborta el encuentro y vuelve a `Screen::World` sin resolverlo (equivalente a "Abort" del spec, manejado por `GameApp`, no por el motor genérico). `resolveEncounterOutcome()` conecta `Victory` con `MissionSystem::advance` + `EvidenceJournal::discover("train41_record")`, y `Defeat` con `Screen::Defeat`.

`drawEncounter()` muestra: ronda actual, tensión (si es mayor a 0), integridad/recurso de ambos participantes, de quién es el turno, y la lista de acciones con su costo — ver ADR `0012-encounter-screen-replaces-realtime-drone-hacking.md` para qué reemplazó (barra de progreso de hacking, golpe único en tiempo real).

## Agregar un encuentro nuevo

1. Definir participantes (`EncounterParticipant`) y acciones (`ActionDefinition`) en una función `buildXEncounter()` — seguir el patrón de `EncounterContent.cpp`.
2. Elegir `EncounterType` (`Combat`, `Hacking`, `Negotiation`, `Investigation`, `Survival`, `SystemRecovery`) — informativo, no cambia el comportamiento del motor.
3. Si el encuentro es jugable desde `GameApp`, agregar el disparador (hotspot/nodo de misión) que llama `startEncounter(...)` y, si el enemigo tiene más de una acción posible, extender la IA mínima de `updateEncounter()`.
4. Agregar tests de dominio (ver `tests/test_encounter_content.cpp`) antes de cablear la UI.
