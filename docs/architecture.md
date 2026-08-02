# Arquitectura

```text
GameApp (raylib, render virtual, navegación)
 ├─ InputRouter / AudioService / DevOverlay
 ├─ SaveService
 └─ GameSession
    ├─ CampaignProgress + NarrativeState + PlayerState
    ├─ MissionSystem / AbilitySystem
    └─ GameModel (dominio académico independiente)
```

`Campaign.cpp` concentra reglas puras y testeables: definiciones de personajes, capacidades, salud/stamina/invulnerabilidad y transición válida de la misión. `WorldLayout` concentra el catálogo runtime de props, capas, pivotes, colliders e interacciones del prólogo; `GameApp` lo traduce a render raylib mediante `AssetStore`, coordina pantallas/modales/HUD y mantiene sólo suelo, paredes, lluvia e iluminación ambiental como composición procedimental. `SaveService` es infraestructura versionada, con escritura temporal.

La UI usa una máquina de pantallas y modales para esta release; `SceneStack` sigue disponible como contrato de overlays para próximas escenas complejas. Ninguna escena contiene la implementación de hacking o combate. El recorrido y los componentes de la UI están documentados en [ui.md](ui.md).
