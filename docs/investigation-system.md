# Investigación

`EvidenceJournal` (`include/cybercba/Evidence.hpp`, `src/domain/Evidence.cpp`) separa el catálogo (`EvidenceItem{id, title, description, imageId, sourceScene, tags, unlocksFlag}`, registrado una vez vía `registerItem`) del descubrimiento (`discover(id)`, con prevención de duplicados — `discoveredCount()`/`discovered()`/`isDiscovered()`).

`GameSession::startPrologue()` puebla el catálogo real vía `buildPrologueEvidenceCatalog()` (`src/domain/PrologueContent.cpp`): `photo`, `insignia`, `network_map`, `unsent_message`, `train41_record` — ver `docs/campaign-bible.md` para qué revela cada una. El descubrimiento persiste en `SaveService` v5 (campo `evidence`, CSV de ids).

## Estado real en `GameApp` (Phase 2)

Solo dos evidencias tienen un hotspot investigable en el mundo hoy: `inspect_photo` y `inspect_map` (`GameApp::interact()`), que llaman `EvidenceJournal::discover(...)` y muestran el texto vía `DialogueGraph::resolveText(...)`. `train41_record` se descubre automáticamente al ganar el encuentro de hacking/combate de la etapa Route (`resolveEncounterOutcome()`). `insignia` y `unsent_message` están en el catálogo pero **no tienen todavía un hotspot que las descubra** — gap real, no oculto (coincide con el gap de `evidenceImage` documentado en `docs/static-art-direction.md`).

`drawObjectives()` lista dinámicamente la evidencia descubierta (`m_session.evidenceJournal().discovered()`); no existe todavía una pantalla dedicada de "Diario de evidencia" (`Screen::Evidence` del spec §34) — la lista vive dentro de la pantalla de objetivos existente.

El jugador no está obligado a inspeccionar toda la evidencia opcional; la información requerida para entender la trama llega igual por el camino principal (transmisión fragmentada/completa, resultado de los encuentros).
