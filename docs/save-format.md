# Guardado

El formato `clave=valor` tiene `version=3`. Persiste configuración accesible/audio (subtítulos, contraste, movimiento reducido, flashes, prompts, escalas y canales), personaje, misión, objetivo, etapa, checkpoint, desbloqueo, salud, stamina y variables narrativas, además del estado académico heredado. La carga conserva compatibilidad con `version=2` usando valores seguros para los ajustes añadidos. Se escribe `*.tmp` y luego se renombra. Un archivo faltante, corrupto o con versión incompatible nunca se toma como una partida válida.
