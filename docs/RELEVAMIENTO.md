# Relevamiento Cyber CBA

> Relevamiento previo al vertical slice. El estado actual incorpora `GameSession`, `GameController`, `QueueChallenge`, `SaveService`, `InputRouter`, `SceneStack`, resolucion virtual y el flujo jugable documentado en [architecture.md](architecture.md).

## Árbol de directorios

```
.
├── assets/                         (vacío, solo .gitkeep)
├── benchmarks/bench_gamemodel.cpp
├── include/model/GameModel.hpp
├── src/main.cpp
├── src/model/GameModel.cpp
├── tests/test_gamemodel.cpp
├── CMakeLists.txt
├── CMakePresets.json
└── vcpkg.json
```

## Estructuras de datos

| Clase | Template | Métodos clave | Usa STL | Target CMake |
|-------|----------|---------------|---------|--------------|
| `cyberpunk::GameModel` | No | `addCredits`, `credits` | No; solo `<cstdint>` | Antes estaba compilada dentro de CLI, tests y benchmark; ahora se expone mediante `cyberpunk_model` para reutilización sin alterar su código. |

El único header público es `include/model/GameModel.hpp`. Es una clase de estado mínima con un único atributo
`std::int64_t mCredits`; no hay aún Queue, Stack, lista, árbol o grafo de la materia.

## Entidades del dominio

- `cyberpunk::GameModel`: único modelo existente; representa créditos.
- No se encontraron `Player`, `Station`, `Line`, `Map`, grafo, controlador ni sistema de entrada.

## Archivos de datos

No se encontraron archivos `.csv`, `.json` ni `.txt` de datos de juego. `vcpkg.json` y `CMakePresets.json`
son archivos de configuración, no datos del dominio. `assets/` no contiene recursos todavía.

## Loop principal

Existe un loop CLI muy simple en `src/main.cpp`: instancia `GameModel`, suma 50 créditos, lo imprime y termina.
No existe loop de juego ni entrada interactiva previa.

## Dependencias actuales (vcpkg.json)

- `gtest`
- `benchmark`
- `raylib` se agrega para el target UI opcional.

## Tests existentes

- `tests/test_gamemodel.cpp`: verifica que los créditos comienzan en cero y que `addCredits` acumula.
- `benchmarks/bench_gamemodel.cpp`: benchmark del modelo.

## Decisiones de arquitectura que esto implica

La UI no puede renderizar un grafo real ni iterar una estructura de inventario del curso porque esas estructuras no
existen en el commit relevado. Para no inventarlas ni tocar el modelo, el MVP muestra una topología de mapa marcada
como preview visual y usa el único dato real disponible (`GameModel::credits`) en HUD e inventario. Cuando se agregue
el grafo/inventario, `MapScene` e `InventoryScene` deberán recibir adaptadores de lectura del modelo; no se debe
modificar la UI para introducir STL en las estructuras de la materia.

Se crea la librería estática `cyberpunk_model` solo en CMake, reutilizando el mismo `GameModel.cpp`. CLI, tests,
benchmark y el juego la enlazan; no se modifica ningún archivo de lógica existente. El juego se compila siempre con
raylib como único ejecutable jugable: `cyber-cba`.
