# Pull Request — CyberCBA 2077

> [!IMPORTANT]
> Antes de abrir la PR: leíste [`AGENTS.md`](../AGENTS.md) (primera vez), linkeaste la issue,
> te asignaste la PR, corriste `clang-format -i` y `ctest --test-dir build/dev` pasa.
> Borrá este bloque antes de publicar.

**Tipo:** Feature / Bugfix / Refactor / Tests / Docs / Build-CI / Chore

**Closes #**

## Qué hace y por qué
<!-- 2-4 líneas: qué problema resuelve y qué decisión de diseño tomaste (ownership,
abstracciones nuevas, etc). No repitas el diff. -->

## Cambios clave
<!-- Lista escaneable de 3-6 bullets, no un changelog línea por línea. -->
-

## Cómo probarlo
```
cmake --build build/dev && ctest --test-dir build/dev --output-on-failure
```
<!-- Agregá un paso manual si aplica (ej. correr cybercba_app). -->

## Checklist C++
- [ ] Sin `std::vector/map/queue/stack/set` ni `unique_ptr/shared_ptr`
- [ ] Ownership de punteros claro (quién crea/posee/destruye)
- [ ] Const-correctness / sin copias innecesarias
- [ ] `cybercba_core` no incluye `<raylib.h>`

## Riesgos
- [ ] Cambio aislado / [ ] Puede afectar otros módulos — cuáles:

## Notas para el reviewer
<!-- Por dónde empezar a mirar, o dudas abiertas. Opcional. -->
