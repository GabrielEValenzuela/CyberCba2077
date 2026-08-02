# Desarrollo

Producción usa `config/game.cfg`; desarrollo usa `config/development.cfg`. F3 muestra telemetría muestreada cada 500 ms. La ventana es redimensionable, mantiene 16:9 y aplica letterboxing. Configuración, modo desarrollo y ruta del guardado se cargan desde `clave=valor` con defaults seguros.

Los perfiles también aceptan `rain_enabled`, `rain_density` (1–120) y `rain_intensity` (0–1). En desarrollo, F3 además habilita el dibujo de pivotes, colliders, radios de interacción e IDs de los props del nivel; en producción esos datos no se renderizan.
