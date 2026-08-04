# ADR 0009: Grafo de misión basado en nodos

`MissionSystem` (dentro de `Campaign.hpp`) es una máquina de etapas lineal (`PrologueStage`), acoplada al prólogo de v0.1.0 y no reutilizable para futuras misiones con ramificaciones/nodos opcionales.

Se agrega `MissionGraph` (`include/cybercba/MissionGraph.hpp`, `src/domain/MissionGraph.cpp`) como estructura genérica: nodos con tipo (`Scene`, `Dialogue`, `Investigation`, `Encounter`, `Decision`, `Transition`), transiciones con condición opcional (`requiredFlag`), y flags de misión propios del grafo. No reemplaza `MissionSystem`/`PrologueStage` en esta fase — conviven; `MissionGraph` es la base para el contenido de la campaña (Phase 2) y para misiones futuras (v0.2.0+) sin heredar el acoplamiento lineal actual.

Es infraestructura, no una estructura académica: usa `std::vector` libremente (ver AGENTS.md §5).
