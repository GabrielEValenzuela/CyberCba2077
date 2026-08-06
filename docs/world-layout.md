# Layout del prólogo

`WorldLayout` es el catálogo de props runtime de la v0.1.0. No crea un segundo motor de escenas: aporta datos testeables a `GameApp`, que ya es el renderizador y controlador de input activo. Cada `WorldProp` declara ID estable, ID/ruta de asset, zona, capa, pivot en mundo, tamaño, collider e interacción.

```text
Shelter: Low -> cama | Mid -> estantería, escritorio | Player | High -> foto, mapa, ventana, transmisor, puerta
Exterior: Low -> suelo | Mid -> refugio, barricada, terminal | Player | High -> farol, dron, Neometro
```

El refugio tiene spawn `(300,470)`, circulación entre cama, estantería y escritorio, y una ruta legible al transmisor. La interacción progresa `Shelter → Transmission`; una segunda interacción reconstruye la señal y desbloquea la puerta; la puerta realiza `Transmission → Route` y coloca al jugador en el exterior.

El exterior usa edificio del refugio, farol, barricada, terminal, dron y entrada Neometro. Emma interviene la terminal desde su prompt contextual; Magga sólo puede resolver el dron con Impacto táctico cerca del objetivo. Al llegar a la entrada, el flujo avanza `Convergence → Epilogue → Complete` mediante el mismo `MissionSystem`.

Los fondos, pisos, lluvia y luces son composición ambiental, no sustitutos de props. En producción no se dibujan colliders, pivotes, IDs ni radios de interacción. F3 los muestra sólo en el perfil development mediante el DevOverlay.

## Atmósfera y accesibilidad

El suelo usa placas industriales alternadas, juntas, cableado, paneles de acceso y humedad; el exterior agrega charcos, cables, fachadas lejanas y lluvia. `GameApp` mantiene el orden de render: shell/fondo, piso y decals, props bajos/medios con sombras de contacto, actor, props altos, iluminación aditiva, clima, UI contextual/HUD y DevOverlay.

El apagón del refugio se inicia al activar la señal: caída de tensión, parpadeo breve (sustituido por fade con `reduceFlashes`), luz de emergencia y foco ámbar del transmisor. La lluvia sólo se dibuja en `Exterior`; `rain_enabled`, `rain_density` y `rain_intensity` se declaran en los perfiles de configuración. Movimiento reducido limita la densidad a un tercio.

La entrada superpone señalética localizada por el juego sobre el texto de origen del PNG: `NEOMETRO`, `LÍNEA OESTE` y `FUERA DE SERVICIO`. No se presenta `LEVEL 1` como texto diegético.
