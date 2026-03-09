# Pull Request — CyberCba2077

> [!IMPORTANT]
> Antes de pedir review:
> - linkeá la tarea/issue correspondiente
> - asignate esta PR
> - verificá que el cambio compile
> - pedí aprobación de **Tester** y **Team Lead**
> - borra todo este bloque ante de darle al botón de abrir PR!

---

## Tipo de cambio
<!-- Marcá al menos una opción para ayudar al reviewer a entender la naturaleza del cambio -->
- [ ] Feature
- [ ] Bugfix
- [ ] Refactor
- [ ] Tests
- [ ] Documentation
- [ ] Performance
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
- ¿Qué fallaba?
- ¿Qué faltaba?
- ¿Qué comportamiento se esperaba?
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
- ¿agregaste una nueva abstracción?
- ¿modificaste ownership, punteros, referencias, const-correctness, RAII, STL containers?
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
- se agregó clase Inventory
- se corrigió validación de comandos
- se actualizaron tests del parser -->
-
-
-
-

---

## Consideraciones técnicas C++
<!-- Completar si aplica. Esta sección ayuda a revisar calidad técnica propia del lenguaje. -->
- [ ] No aplica
- [ ] Se respetó const-correctness
- [ ] Se evitó copiar objetos innecesariamente
- [ ] Se usó RAII / manejo seguro de recursos
- [ ] Se revisó ownership de memoria/punteros
- [ ] Se evitó código duplicado
- [ ] Se consideró complejidad temporal/espacial
- [ ] Se mantuvo compatibilidad con la arquitectura existente

Detalle adicional
-

---

## Cómo probar este cambio
<!-- Escribí pasos concretos y reproducibles.
Un reviewer o tester debería poder validar la PR sin preguntarte nada.
Incluir:
- comando para compilar
- comando para correr
- input esperado si aplica
- caso feliz
- caso borde / error si aplica -->
1.
2.
3.

### Resultado esperado
<!-- Describí qué debería observar el tester -->
-

---

## Evidencia
<!-- Obligatorio cuando haya cambios visibles, gameplay, consola, logs o salidas relevantes.
Podés adjuntar screenshots, GIFs, outputs de consola o logs. -->
- [ ] No aplica
- [ ] Capturas adjuntas
- [ ] Logs adjuntos
- [ ] Output de consola adjunto
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
- [ ] Compila correctamente en mi entorno
- [ ] Se agregaron o actualizaron tests unitarios
- [ ] Los tests existentes siguen pasando
- [ ] Se hizo validación manual
- [ ] Se probaron casos borde
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
- [ ] El código compila
- [ ] Actualicé tests si correspondía
- [ ] Actualicé documentación si correspondía
- [ ] No dejé código muerto, prints temporales o comentarios innecesarios
- [ ] Los nombres de variables, funciones y clases son claros
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
- empezar por src/engine/command_parser.cpp
- luego mirar tests/parser_test.cpp
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
