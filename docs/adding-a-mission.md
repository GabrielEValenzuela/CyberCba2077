# Agregar una misión

1. Crear definición y textos en `assets/data/missions`.
2. Añadir estados/condiciones y tests puros de dominio.
3. Registrar zonas, interactuables, diálogos y checkpoints.
4. Conectar capacidades mediante `AbilitySystem`, sin condicionar escenas por personaje.
5. Persistir cualquier estado nuevo con migración explícita de `SaveService`.
6. Exponer el capítulo bloqueado/disponible, probar guardado y actualizar roadmap.

Para Neometro, la misión consume los mismos jugador, mundo, objetivos y diálogos. Queue/Stack se implementan como dominios nuevos sin STL, no se reutiliza ni se oculta una solución de la v0.1.0.
