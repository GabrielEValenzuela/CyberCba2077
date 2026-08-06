# UI de v0.1.0

La UI final de la release se concentra en `GameApp` como una máquina de pantallas y modales, conservando el dominio en `GameSession`, `MissionSystem` y `AbilitySystem`.

```text
Splash -> Menu -> Seleccion -> Introduccion -> Mundo -> Fin -> Capitulos
             |       |              |             |
             +-> Capitulos          +-> Pausa ----+-> Objetivos / Configuracion
             +-> Configuracion      +-> Dialogo / Intrusion / Derrota
             +-> Creditos
```

Todos los botones del menú principal y pausa tienen una ruta real. Las acciones destructivas (`Nueva partida`, reiniciar checkpoint, volver al menú y salir) se protegen mediante modal; el modal consume la entrada antes de actualizar la pantalla inferior. `Continuar` queda deshabilitado sin un guardado válido y Neometro se presenta como contenido bloqueado de v0.2.0.

## Componentes y tema

La presentación reutiliza `drawButton`, `drawOverlay`, `drawModal`, HUD, tarjetas de capítulo y tarjetas de selección. `NlmColors` mantiene el tema cian/ámbar y las métricas comunes usan la resolución virtual 1280x720. Los botones presentan borde, marcador lateral y contraste de foco; no dependen sólo del color.

## Controles y accesibilidad

Teclado, click de mouse y gamepad actualizan el dispositivo activo; los prompts alternan entre teclado y gamepad. La navegación por teclado/gamepad usa foco visible. Configuración persiste escala de UI, scanlines, flashes, canales de audio, subtítulos, alto contraste, movimiento reducido, prompts persistentes y velocidad de diálogo. El remapeo se declara explícitamente como próximo trabajo, sin botón engañoso.

## Integración jugable

El HUD muestra salud, recurso y objetivo. Emma abre la interfaz de intrusión y completa el enlace manteniendo confirmar; Magga recibe feedback de amenaza, puede usar Impacto táctico y puede llegar a derrota/reinicio. El diario se abre desde mundo o pausa. Guardar muestra un toast breve y las recuperaciones de checkpoint restauran estado de misión, salud y stamina.

El refugio y el exterior del prólogo usan sprites procesados declarados en `WorldLayout`: bajo → medio → jugador → alto → iluminación/FX. Los prompts aparecen únicamente junto a los interactuables y las colisiones/bounds sólo se dibujan con DevOverlay en perfil de desarrollo. Las rutas, metadata y el espejo declarativo del nivel permanecen en `assets/data/assets-manifest.json`.
