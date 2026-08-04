# Arquitectura UI — CyberCBA 2077

> Este documento describe el MVP historico. La arquitectura implementada y vigente esta en [architecture.md](architecture.md); el `SceneManager` historico ya no es el orquestador del flujo jugable.

## Decisiones de diseño

El juego es un único ejecutable raylib, `cyber-cba`, con su propio `main_ui.cpp`; no conserva un ejecutable CLI.
`SceneManager` cumple el rol de controlador ligero: interpreta las transiciones solicitadas por cada escena, mantiene
el estado de navegación y es dueño del `GameModel` del MVP. Las escenas dibujan con raylib y no mutan el modelo.

No hay todavía estructuras de datos ni mapa reales. Por ello `MapScene` dibuja una topología de preview declarada
localmente y `InventoryScene` presenta una inspección de solo lectura de créditos. Esta decisión evita inventar un
grafo/lista en la capa de dominio. La integración posterior debe reemplazar solamente esas fuentes de lectura por
interfaces/adaptadores del modelo.

```
┌─────────────────────────────────────────────┐
│ RaylibView: SceneManager -> IScene           │  solo dibuja
│ Splash · Menu · Map + HUD · Inventory        │
└──────────────────┬──────────────────────────┘
                   │ observa / solicita transición
┌──────────────────▼──────────────────────────┐
│ GameController: SceneManager                 │  input y estado de escenas
└──────────────────┬──────────────────────────┘
                   │ lee
┌──────────────────▼──────────────────────────┐
│ GameModel: cyberpunk::GameModel              │  lógica existente
│ créditos (hoy); grafo e inventario (futuro)  │
└─────────────────────────────────────────────┘
```

## Clases nuevas

### `IScene`
- Responsabilidad: contrato uniforme de actualización, dibujo y transición.
- Métodos públicos: `update`, `draw`, `shouldTransition`, `nextScene`.
- Dependencias: ninguna.
- Archivo: `src/ui/IScene.hpp`.

### `SceneManager`
- Responsabilidad: registrar, seleccionar y actualizar escenas; superponer HUD en mapa.
- Métodos públicos: `init`, `shutdown`, `update`, `draw`, `shouldExit`.
- Dependencias: `IScene`, escenas, `GameModel`.
- Archivo: `src/ui/SceneManager.hpp`.

### `SplashScene`, `MainMenuScene`, `MapScene`, `HUDScene`, `InventoryScene`
- Responsabilidad: una pantalla cada una; HUD solo se superpone a mapa.
- Métodos públicos: contrato `IScene`; menú/mapa/inventario procesan sus teclas.
- Dependencias: raylib; HUD, mapa e inventario leen `GameModel` donde aplica.
- Archivos: `src/ui/scenes/`.

### `NlmButton`, `NlmPanel`, `NlmText`
- Responsabilidad: controles de presentación reutilizables sin lógica de dominio.
- Métodos públicos: dibujo y, en botón, detección de foco.
- Dependencias: raylib y `NlmColors`.
- Archivos: `src/ui/widgets/`.

## Estructura de directorios resultante

```
src/
├── main_ui.cpp
└── ui/
    ├── IScene.hpp
    ├── NlmColors.hpp
    ├── SceneManager.hpp/.cpp
    ├── scenes/{SplashScene,MainMenuScene,MapScene,HUDScene,InventoryScene}.hpp/.cpp
    └── widgets/{NlmButton,NlmPanel,NlmText}.hpp/.cpp
```

## Integración con código existente

`SceneManager` posee el `cyberpunk::GameModel` que el CLI ya usa. `HUDScene` e `InventoryScene` consultan
`credits()` sin modificar el modelo. No existe una representación de estaciones o inventario para integrar aún;
los puntos de extensión están documentados en `MapScene` e `InventoryScene`.

## CMake: cómo agregar raylib

`vcpkg.json` declara `raylib`. CMake usa `find_package(raylib CONFIG REQUIRED)`, crea el target `cyber_cba` con
nombre de salida `cyber-cba`, y lo enlaza con `raylib` y `cyberpunk_model`. El modelo se extrae a una librería
estática de CMake, sin cambiar su fuente, para que juego, tests y benchmark compartan exactamente la misma lógica.
La configuración se conserva externamente en `config/game.cfg`, se copia junto al binario durante la configuración
de CMake y puede sustituirse con `cyber-cba --config ruta/game.cfg`.

`ui_mode=production` es el valor de entrega: no crea ni dibuja la telemetría. Para desarrollo,
`ui_mode=development` crea `DevOverlay`, que muestra FPS de raylib y muestras de CPU, memoria y GPU. Las métricas
de CPU/memoria usan `/proc`; la GPU se lee de la interfaz estándar de algunos drivers Linux y se informa como
`unavailable` si no está expuesta, sin afectar al juego.

Los perfiles entregados son `config/game.cfg` (producción) y `config/development.cfg` (desarrollo). El primero se
copia junto al binario por CMake; el segundo se selecciona explícitamente con `--config`.

## Compilación y ejecución

Con `VCPKG_ROOT` definido (o `vcpkg` disponible en `PATH`), el script `scripts/build.sh` configura y compila el
target único del juego. Para lanzar el perfil de desarrollo: `./scripts/build.sh --dev --run`.

```cmake
find_package(raylib CONFIG REQUIRED)
add_executable(cyber_cba ...)
set_target_properties(cyber_cba PROPERTIES OUTPUT_NAME "cyber-cba")
target_link_libraries(cyber_cba PRIVATE cyberpunk_model raylib)
configure_file(config/game.cfg "${CMAKE_CURRENT_BINARY_DIR}/game.cfg" COPYONLY)
```

## Qué NO cambia

- `include/model/GameModel.hpp`
- `src/model/GameModel.cpp`
- `src/main.cpp`
- `tests/test_gamemodel.cpp`
- `benchmarks/bench_gamemodel.cpp`

La UI no introduce STL en clases de la materia ni agrega comportamiento al modelo.
