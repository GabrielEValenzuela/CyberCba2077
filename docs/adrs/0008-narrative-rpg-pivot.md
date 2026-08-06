# ADR 0008: Pivot a RPG narrativo por turnos

El prototipo top-down de movimiento libre no sostiene el diseño objetivo (investigación, diálogo ramificado, encuentros por turnos, misiones episódicas). Se pivota el producto a un RPG narrativo: exploración por nodos, escenas con fondo/retrato/texto, e investigación/combate/hacking resueltos con un motor de turnos genérico.

El prototipo top-down (`WorldLayout`, `AbilitySystem` en tiempo real, animación de 8 direcciones) no se elimina: queda como base existente, pero deja de ser el flujo principal a partir de esta decisión. Ver ADR `0011-deprecate-scenestack.md` para el estado de `SceneStack`/`IScene`, que son código muerto no relacionado con el prototipo top-down real (`GameApp.cpp`).

Esta fase (v0.1.0, incremento "Phase 1") agrega solo la capa de dominio reutilizable (`MissionGraph`, `Dialogue`, `Evidence`, `Encounter`) sin todavía conectar contenido de la campaña ni pantallas nuevas — ver `docs/roadmap.md`.
