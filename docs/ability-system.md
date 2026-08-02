# Sistema de habilidades

`AbilitySystem` tiene operaciones puras: `hack`, `strike`, `applyDamage` y `update`. Hacking valida objetivo y cooldown; combate consume stamina, admite golpe fuerte y aplica cooldown. El jugador recibe invulnerabilidad breve al daño. Las escenas consumen `AbilityResult`, por lo que futuras cámaras, puertas, drones o mecanismos pueden conectar sus propios adaptadores sin duplicar por personaje.

Para agregar una habilidad, definir su especialidad/datos, exponer una operación testeable y hacer que los objetivos la soliciten mediante una interfaz de capacidad, nunca mediante ramas narrativas gigantes.
