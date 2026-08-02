# Misiones y objetivos

`CampaignProgress` contiene estado de misión, objetivo, etapa, checkpoint y desbloqueos. `MissionSystem` permite iniciar y avanzar sólo desde la etapa esperada: `Shelter → Transmission → Route → Convergence → Epilogue → Complete`. La v0.1.0 utiliza el sistema en cada transición y desbloquea el capítulo anunciado, no su contenido.

El siguiente paso es materializar `MissionDefinition` desde `assets/data/missions/`, manteniendo el mismo estado persistido y la misma API de condiciones.
