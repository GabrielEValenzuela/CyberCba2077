# Pull Request — CyberCBA 2077

> [!IMPORTANT]
> Antes de pedir review:
> - leíste [`AGENTS.md`](../AGENTS.md) si es tu primera PR en el proyecto
> - linkeá la tarea/issue correspondiente
> - asignate esta PR
> - verificá que el cambio compile (`cmake --build build/dev`) y que los tests pasen (`ctest --test-dir build/dev`)
> - corriste `clang-format -i` sobre los archivos que tocaste
> - pedí aprobación de **Tester** y **Team Lead**
> - borrá todo este bloque antes de darle al botón de abrir PR!

---

## Tipo de cambio
<!-- Marcá al menos una opción para ayudar al reviewer a entender la naturaleza del cambio -->
- [ ] Feature
- [ ] Bugfix
- [ ] Refactor
- [ ] Tests
- [ ] Documentation (GPD/TAD/VS-001/ADR)
- [ ] Narrative content (escenas/diálogos de VS-001)
- [ ] Build / CI
- [ ] Chore

---

## Issue / tarea relacionada
<!-- Obligatorio: linkear la tarea o issue asociada.
Usar referencias como:
- Closes #12
- Fixes #8
- Related #21

Esto ayuda a la trazabilidad y a conectar código con requerimientos. -->
- Closes #
- Related #

---

## Resumen del cambio
<!-- Explicá brevemente qué hace esta PR.
Pensá esta sección como un TL;DR para que alguien entienda el objetivo sin leer todo el diff.
Ideal: 2 a 5 líneas. -->
Este PR:

-

---

## Problema / motivación
<!-- Explicá qué problema resuelve.
No describas todavía archivos ni detalles de implementación.
Respondé:
- ¿Qué fallaba o qué faltaba?
- ¿Qué comportamiento se esperaba (citá GPD/TAD-001/VS-001 si aplica)?
- ¿Por qué era necesario este cambio? -->
Contexto
-

Problema observado
-

Resultado esperado
-

---

## Solución implementada
<!-- Explicá el enfoque técnico elegido.
En C++ esto es especialmente importante:
- ¿qué clases, structs o módulos tocaste?
- ¿agregaste una nueva abstracción? ¿hacía falta (TAD-001 §4: cero abstracción especulativa)?
- ¿modificaste ownership, punteros, referencias, const-correctness, RAII?
- ¿cambió la complejidad o el ciclo de vida de objetos?

No repitas el diff línea por línea. Explicá la decisión de diseño. -->
Decisiones principales
-

-
-

Archivos o módulos clave
-

-
-

---

## Cambios incluidos
<!-- Lista concreta y escaneable.
Ejemplo:
- se agregó CombatActionType::UseResource
- se corrigió transición inválida en GameStateMachine
- se actualizaron tests de CombatSystem -->
-
-
-
-

---

## Consideraciones técnicas C++ (ver AGENTS.md / TAD-001)
<!-- Completar si aplica. Esta sección ayuda a revisar calidad técnica propia del lenguaje
y a chequear las reglas específicas de este proyecto, que no son las típicas de C++ moderno. -->
- [ ] No aplica
- [ ] **No introduje `std::vector`/`std::map`/`std::queue`/`std::stack`/`std::set`/etc. ni `std::unique_ptr`/`std::shared_ptr`** (TAD-001 §10-§11 — esto lo audita CI automáticamente, pero revisalo antes)
- [ ] Si toqué `include/cybercba/structures/`, no le agregué cuerpo a los métodos sin que se me pida explícitamente (son un ejercicio para el equipo, ver GPD §55)
- [ ] `cybercba_core` sigue sin incluir `<raylib.h>` ni llamar funciones de raylib directamente
- [ ] Se respetó const-correctness
- [ ] Se evitó copiar objetos innecesariamente
- [ ] Se revisó ownership de memoria/punteros (¿quién crea? ¿quién posee? ¿quién destruye?)
- [ ] Se evitó código duplicado
- [ ] Se mantuvo compatibilidad con la arquitectura existente (TAD-001 §5-§7)

Detalle adicional
-

---

## Cómo probar este cambio
<!-- Escribí pasos concretos y reproducibles.
Un reviewer o tester debería poder validar la PR sin preguntarte nada.
Incluir:
- comando para compilar/testear
- input esperado si aplica
- caso feliz
- caso borde / error si aplica -->
1. `export VCPKG_ROOT=...` y `cmake --preset dev`
2. `cmake --build build/dev`
3. `ctest --test-dir build/dev --output-on-failure`
4.

### Resultado esperado
<!-- Describí qué debería observar el tester -->
-

---

## Evidencia
<!-- Obligatorio cuando haya cambios visibles, gameplay, consola, logs o salidas relevantes.
Podés adjuntar screenshots, GIFs, outputs de consola, ctest, o sanitizers. -->
- [ ] No aplica
- [ ] Capturas adjuntas
- [ ] Logs / output de ctest adjunto
- [ ] Output de sanitizers (ASan/UBSan) adjunto
- [ ] Video/GIF adjunto

Evidencia
-

---

## Riesgos / impacto
<!-- Ayuda al reviewer a enfocar la revisión.
Marcá si el cambio puede romper otros módulos o alterar contratos existentes. -->
Impacto esperado:

- [ ] Cambio aislado
- [ ] Puede afectar otros módulos
- [ ] Puede modificar comportamiento existente
- [ ] Puede introducir regresiones si no se valida correctamente

Módulos potencialmente afectados
-

-
-

Breaking changes:

- [ ] No
- [ ] Sí

Si la respuesta es sí, explicar
-

---

## Testing realizado
<!-- Marcar lo que realmente se hizo.
No marcar cosas que no se validaron. -->
- [ ] Compila correctamente en mi entorno (`cmake --build build/dev`)
- [ ] `ctest --test-dir build/dev` pasa completo
- [ ] Se agregaron o actualizaron tests unitarios
- [ ] Corrí `clang-format -i` sobre los archivos tocados
- [ ] Probé bajo el preset `asan` si el cambio toca lógica de memoria/estructuras
- [ ] Se hizo validación manual (ej. corriendo `cybercba_app`)
- [ ] No aplica agregar tests

Detalle
-

---

## Checklist del autor
<!-- Esta sección es para disciplina profesional mínima antes de pedir review. -->
- [ ] Linkeé la issue / tarea correspondiente
- [ ] Me asigné esta PR
- [ ] El cambio tiene un objetivo claro y acotado
- [ ] No mezclé cambios no relacionados
- [ ] Revisé mi propio diff antes de pedir review
- [ ] El código compila y los tests pasan
- [ ] Actualicé tests si correspondía
- [ ] Actualicé documentación (GPD/TAD-001/VS-001/ADR) si correspondía
- [ ] No dejé código muerto, prints temporales o comentarios innecesarios
- [ ] Los nombres de variables, funciones y clases son claros (ver convenciones en ADR-001)
- [ ] La solución respeta la modularidad del proyecto

---

## Checklist de review
<!-- Esta sección ayuda a formalizar el flujo del equipo. -->
- [ ] Review técnica realizada
- [ ] Approval de Tester
- [ ] Approval de Team Lead

Reviewer(s):

- Tester:
- Team Lead:

---

## Notas para el reviewer
<!-- Usá esta sección para guiar la revisión y ahorrar tiempo.
Por ejemplo:
- empezar por src/combat/CombatSystem.cpp
- luego mirar tests/combat/CombatSystemTest.cpp
- tengo dudas sobre ownership en X -->
Orden sugerido de revisión:
1.
2.
3.

Puntos donde quiero feedback
-

-

Dudas abiertas
-
