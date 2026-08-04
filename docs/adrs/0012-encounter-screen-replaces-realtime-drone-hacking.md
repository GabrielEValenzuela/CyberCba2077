# ADR 0012: La pantalla de Encuentro reemplaza el hacking/combate en tiempo real del prólogo

El prólogo original resolvía el hacking de Emma con una barra de progreso (`Modal::Hacking`, mantener ENTER) y el combate de Magga con un golpe único en tiempo real contra el dron (`AbilitySystem::strike` disparado por `PrimaryAttack` + daño continuo por proximidad). Ninguno de los dos era un encuentro por turnos ni usaba la fundación de `Encounter` (Phase 1).

Se agrega `Screen::Encounter`, alimentada por `EncounterContent` (`include/cybercba/EncounterContent.hpp`, `src/domain/EncounterContent.cpp`): `buildEmmaHackingEncounter()` (Emma vs. "Rele de interferencia": Analizar/Aislar/Redirigir/Ocultar/Extraer) y `buildMaggaCombatEncounter()` (Magga vs. "Dron de seguridad": Golpe/Guardia/Esquivar/Impacto táctico), ambas configuraciones de datos sobre el motor genérico `Encounter` de Phase 1.

`GameApp::interact()` en los hotspots `use_terminal`/`face_drone` ahora llama `startEncounter(...)` en vez de abrir `Modal::Hacking` o resolver el combate en un solo frame. Se eliminó el daño por proximidad en tiempo real del dron (`AbilitySystem::applyDamage` en el loop de `Screen::World`) porque el combate ahora se resuelve enteramente dentro de `Screen::Encounter`.

**Limitaciones conocidas de esta integración** (documentadas, no ocultas):
- Solo hay un enemigo por encuentro en el prólogo, así que la UI no implementa selección de objetivo — es una simplificación válida para v0.1.0, no una limitación del motor genérico.
- Los efectos de estado (`guarding`, `evasive`, `guard_broken`, `hidden`) se aplican y expiran correctamente en el motor (`Encounter::performAction`/`tickStatuses`), pero **no tienen todavía efecto mecánico** (ej. `guarding` no reduce el daño recibido) — son marcadores narrativos/UI en esta fase; la lógica de mitigación queda para un incremento futuro si el diseño la requiere.
- La IA enemiga es determinista y mínima (el relé siempre usa `relay_pulse`; el dron alterna `charge_shot`/`rush` por paridad de ronda) — suficiente para el prólogo, no pensada como IA general de `EncounterAI`.
