# Roadmap

- **v0.1.0 — La última transmisión:** pivot a RPG narrativo por turnos completo — selección de personaje, investigación del refugio, apagón, recuperación de energía (encuentro compartido — planeado, ver Pendientes), hacking de Emma y combate de Magga (`Screen::Encounter`, Phase 3), elección narrativa, ruta exterior, entrada al Neometro anunciada. Ver `docs/campaign-bible.md` para los hechos narrativos y `docs/architecture.md` para qué está realmente conectado a `GameApp` vs. solo en el dominio.
- **v0.2.0 — Neometro: El último convoy (planeado):** misión nueva sobre la misma fundación (`MissionGraph`/`DialogueGraph`/`EvidenceJournal`/`Encounter`), estructuras Queue/Stack sin STL.

La infraestructura general puede usar STL; las estructuras evaluadas de la materia no — ver `docs/academic-integration.md`.

## Cómo v0.2.0 se conecta sin reescribir la fundación

`MissionGraph`, `DialogueGraph`, `EvidenceJournal` y `Encounter` (Phase 1-3 de esta release) ya son genéricos — v0.2.0 solo necesita:

1. Un `NeometroContent.cpp` nuevo con su propio `buildNeometroMissionGraph()`/diálogo/evidencia/encuentros (mismo patrón que `PrologueContent.cpp`/`EncounterContent.cpp`, ver `docs/adding-a-mission.md`).
2. `Queue`/`Stack` implementadas sin STL (ver `docs/academic-integration.md`) detrás de `ITurnQueue`/`IActionHistory`, consumidas por la configuración de encuentro de esa misión (orden de turno de emergencias, historial de acciones reversible) — no por `Encounter` mismo, que sigue usando `std::vector` como infraestructura.
3. Extender `SaveService` a una versión nueva con los campos de esa misión, con migración explícita (mismo patrón que v3→v4→v5 en `docs/save-format.md`).
4. Nuevos nodos/pantallas siguiendo el patrón `Screen::Encounter`/`draw*()` de `GameApp`, sin tocar el motor genérico.

## Pendientes no bloqueantes hacia v0.2.0

- Encuentro compartido de recuperación de energía (spec §22) — hoy es una única acción de confirmación en el nodo `power_recovery` (`GameApp::selectNodeOption`, acción `@restore_power`), no un `Encounter` por turnos propio con tradeoffs reales.
- `security_control` es igualmente una única acción por protagonista, no un obstáculo con múltiples rutas tácticas (spec §29).
- Pantalla dedicada de Diario de evidencia (`EvidenceJournal` ya tiene los datos; hoy vive dentro de `drawObjectives()`, no en un `Screen` propio — ver `docs/investigation-system.md`). El mapa de misión por nodos ya existe como pantalla real (`Screen::World`, ADR `0014-node-based-world-screen.md`).
- Evidencia `insignia`/`unsent_message` sin hotspot que las descubra todavía (ver `docs/investigation-system.md`).
- No hay composición de escena narrativa con fondo/retrato ilustrado — el shell procedural existente cumple el rol de `Background` (ver `docs/static-art-direction.md`).
