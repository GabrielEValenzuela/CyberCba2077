# Agregar una misión

Flujo real sobre la fundación del pivot narrativo (`MissionGraph`/`DialogueGraph`/`EvidenceJournal`/`Encounter`, Phase 1-3 — ver `docs/architecture.md`):

1. Definir metadata del capítulo (id, título, versión) — hoy vive implícita en `CampaignProgress`/`PrologueStage`; para una misión nueva, seguir el patrón de `buildPrologueMissionGraph()`.
2. Definir el grafo de misión: una función `buildXMissionGraph()` en un archivo de contenido propio (paralelo a `src/domain/PrologueContent.cpp`), con `MissionNode`/`MissionTransition` y flags de bloqueo — no reutilizar `PrologueStage` para una misión nueva.
3. Registrar fondos de escena (`assets/data/assets-manifest.json`, categoría `background` — ver `docs/static-art-direction.md`), con licencia/autor/fuente verificados.
4. Definir diálogo: `DialogueGraph` propio (o extender uno compartido), siguiendo el patrón `textForEmma`/`textForMagga`/`OtherProtagonist` de `PrologueContent.cpp`.
5. Definir evidencia de investigación: `EvidenceJournal` propio con `EvidenceItem` nuevos.
6. Definir encuentros: `buildXEncounter()` sobre el motor genérico `Encounter` (ver `docs/encounter-system.md`) — nunca un motor de turnos nuevo por misión.
7. Registrar acciones/habilidades nuevas como `ActionDefinition` dentro del `buildXEncounter()` correspondiente, no como ramas especiales en `GameApp`.
8. Conectar la integración de dominio específica de la misión (ej. v0.2.0: Queue/Stack detrás de `ITurnQueue`/`IActionHistory` — ver `docs/academic-integration.md`).
9. Agregar tests de dominio (grafo, diálogo, evidencia, encuentro) antes de cablear la UI — seguir el patrón de `tests/test_prologue_content.cpp`/`tests/test_encounter_content.cpp`.
10. Persistir cualquier estado nuevo con migración explícita de `SaveService` (bump de versión, campos nuevos con default seguro para saves anteriores — ver `docs/save-format.md`) y actualizar `docs/roadmap.md`.

La misión nueva **no debe** requerir cambiar `Encounter.{hpp,cpp}`, `MissionGraph.{hpp,cpp}`, `Dialogue.{hpp,cpp}` ni `Evidence.{hpp,cpp}` — esos son el motor genérico. Si una misión necesita cambiarlos, es señal de que falta una configuración de datos, no una excepción de motor.
