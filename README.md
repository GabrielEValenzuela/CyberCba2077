# CYBERCBA 2077

## La ciudad que nos olvidó

**v0.1.0 — La última transmisión** es un prólogo 2D top-down jugable en raylib. El jugador elige a Emmanuel “Emma” Vázquez, especialista en intrusión de sistemas, o a Magalí “Magga” Temerro, especialista en combate de proximidad. La elección cambia la ruta del prólogo, los registros encontrados y la forma de superar el peligro; ambas convergen en la entrada clausurada del Neometro.

La capa académica (estructuras, tests y benchmarks) queda fuera de la ficción: no aparece en la UI del jugador. `QueueChallenge` se conserva como dominio educativo de referencia, pero no es contenido de la campaña v0.1.0.

## Ejecutar

```bash
./scripts/build.sh --run
./scripts/build.sh --dev --run
ctest --test-dir build/dev --output-on-failure
```

`cyber-cba --config ruta/game.cfg` permite elegir otro perfil. La ventana usa una resolución virtual 1280×720 con letterboxing.

## Controles

- WASD: moverse; flechas: navegar menús.
- Enter/Espacio: confirmar; E: interactuar / hackear.
- X: Impacto táctico de Magga; P o Escape: pausa.
- F3: telemetría en perfil de desarrollo.

Consultá [arquitectura](docs/architecture.md), [UI](docs/ui.md), [diseño de juego](docs/game-design.md), [dirección artística](docs/art-direction.md), [pipeline de assets](docs/asset-pipeline.md), [desarrollo](docs/development.md) y [cómo agregar una misión](docs/adding-a-mission.md).
