# CYBERCBA 2077

## La ciudad que nos olvidó

CYBERCBA 2077 es un RPG narrativo por turnos ambientado en una Córdoba distópica — investigación, diálogo, relaciones y misiones episódicas resueltas con encuentros por turnos (ver ADR `0008-narrative-rpg-pivot.md` y `docs/campaign-bible.md`). **v0.1.0 — La última transmisión** es el prólogo jugable: el jugador elige a Emmanuel "Emma" Vázquez, especialista en intrusión de enlace, o a Magalí "Magga" Temerro, especialista en impacto táctico. La elección cambia quién responde la transmisión, los registros encontrados y el encuentro por turnos que resuelve el peligro (hacking para Emma, combate para Magga); ambas rutas convergen en la entrada clausurada del Neometro.

El prototipo top-down original (movimiento libre, colisión, animación de 8 direcciones) fue reemplazado: `Screen::World` es ahora un mapa de nodos real impulsado por `MissionGraph` — el jugador elige a dónde ir desde una lista de destinos en vez de caminar con WASD (ADR `0014-node-based-world-screen.md`). `WorldLayout` se preserva como infraestructura testeada, pero solo aporta el fondo ilustrado procedural detrás del panel de nodos, no colisión ni movimiento. Ver `docs/architecture.md` para el detalle.

La capa académica (estructuras, tests y benchmarks) queda fuera de la ficción: no aparece en la UI del jugador. `QueueChallenge` se conserva como dominio educativo de referencia, pero no es contenido de la campaña v0.1.0 — ver `docs/academic-integration.md`.

## Ejecutar

```bash
./scripts/build.sh --run
./scripts/build.sh --dev --run
ctest --test-dir build/dev --output-on-failure
```

`cyber-cba --config ruta/game.cfg` permite elegir otro perfil. La ventana usa una resolución virtual 1280×720 con letterboxing.

## Controles

- Flechas/WASD: navegar menús, nodos de misión y acciones de encuentro.
- Enter/Espacio: confirmar / viajar / actuar.
- En un encuentro por turnos: flechas eligen la acción, Enter la usa, Escape se retira.
- P o Escape: pausa. O: objetivos. F3: telemetría en perfil de desarrollo.

Consultá [arquitectura](docs/architecture.md), [biblia de campaña](docs/campaign-bible.md), [sistema de misiones](docs/mission-system.md), [sistema de encuentros](docs/encounter-system.md), [sistema de turnos](docs/turn-system.md), [diálogos](docs/dialogue-system.md), [investigación](docs/investigation-system.md), [UI](docs/ui.md), [diseño de juego](docs/game-design.md), [dirección artística](docs/art-direction.md), [dirección de arte estática](docs/static-art-direction.md), [pipeline de assets](docs/asset-pipeline.md), [integración académica](docs/academic-integration.md), [desarrollo](docs/development.md), [roadmap](docs/roadmap.md) y [cómo agregar una misión](docs/adding-a-mission.md).

## Arte, recursos y agradecimientos

El arte estático de CyberCBA 2077 (fondos, retratos, figuras, evidencia) se genera con IA en `assets/generated/` — ver [dirección de arte estática](docs/static-art-direction.md). Los packs de terceros evaluados en versiones anteriores (Pixel Cyberpunk Interior, CraftPix) fueron descartados por no llegar a integrarse en el render real; el detalle de esa decisión vive en [`docs/third-party-assets.md`](docs/third-party-assets.md).

- **DejaVu Sans Mono** — [dejavu-fonts.github.io](https://dejavu-fonts.github.io/) — fuente de UI real (licencia Bitstream Vera).

La lista completa de créditos vive en `docs/third-party-assets.md` — la pantalla de créditos in-game referencia ese documento en vez de duplicar la lista.

### Sumate a los créditos

La sección "EQUIPO // ANUARIO" de la pantalla de créditos in-game se arma leyendo `credits/*.txt` en tiempo de ejecución. Para aparecer ahí, agregá tu propio archivo con un Pull Request — ver [`credits/README.md`](credits/README.md) para el formato y el flujo de git/GitHub paso a paso.
