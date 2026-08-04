# Integración académica

`docs/educational-design.md` describe `QueueChallenge`, el dominio educativo actual (FIFO sobre `std::array` fijo, sin STL dinámico) — permanece fuera de la ficción, no es contenido de campaña. Este documento cubre cómo las estructuras académicas futuras (Queue/Stack en v0.2.0, listas/árboles/heaps/grafos en releases posteriores — ver `docs/roadmap.md`) se conectan al motor sin convertirse en dependencias ocultas del gameplay real.

## Frontera

- Infraestructura, UI, dominio narrativo (`MissionGraph`, `DialogueGraph`, `EvidenceJournal`, `Encounter`) y persistencia **pueden usar STL** libremente — son parte de la ingeniería del juego, no del contenido evaluado.
- Las estructuras académicas asignadas por materia (Queue, Stack, listas, árboles, heaps, grafos) **deben implementarse sin STL** — ver ADR `0003-domain-boundary.md` y `AGENTS.md` §5. Cada release nueva implementa su estructura desde cero; no se reutiliza ni se oculta la solución de una release anterior.

## Interfaces de extensión (documentadas, no implementadas todavía)

Spec §33 pide interfaces explícitas para que el motor pueda recibir implementaciones de estudiantes sin que el gameplay dependa de una implementación oculta de reemplazo:

```cpp
ITurnQueue        // orden/gestión de turnos pendientes (candidato: v0.2.0, Queue)
IActionHistory    // historial de acciones reversible (candidato: v0.2.0, Stack)
IInventoryChain   // cadena de inventario dinámica (candidato: v0.3.0, listas)
ICitizenIndex     // índice de ciudadanos/archivo (candidato: v0.4.0, árboles)
IPriorityQueue    // prioridad/triage (candidato: v0.5.0, heap/AVL)
ICityGraph        // conexiones de distrito (candidato: v0.6.0, grafos)
IRouteFinder      // ruta con costo (candidato: v0.7.0, grafos con peso)
```

Ninguna de estas interfaces existe todavía en el código — quedan documentadas como puntos de extensión futuros, no como trabajo pendiente oculto. Cuando se implemente una, debe: (1) vivir en `include/cybercba/`, sin dependencia de raylib; (2) ser consumida por el motor genérico existente (`Encounter`, `MissionGraph`, etc.) a través de la interfaz, no de un tipo concreto; (3) tener una implementación de referencia sin STL provista por quien curse esa estructura, sin fallback oculto que haga funcionar el juego si la implementación del estudiante falta o está incompleta.

## Ejemplo concreto: v0.2.0 y `ITurnQueue`/`IActionHistory`

Ver "Preparación para futuras misiones" en `docs/roadmap.md` y `docs/adding-a-mission.md` para cómo Neometro: El último convoy conecta Queue/Stack sin reescribir `MissionGraph`/`Encounter`.
