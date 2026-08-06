Perfecto. Te dejo un **segundo prompt para Codex**, enfocado en:

* integrar los assets procesados,
* reemplazar los rectángulos,
* armar el **refugio del prólogo**,
* armar la **zona exterior**,
* y montar la **entrada del Neometro** para la `v0.1.0`.

---

````md
# Task — Integrar assets reales al nivel 1 de CyberCBA 2077

Necesito que tomes los assets ya procesados del proyecto y los integres correctamente al gameplay de la `v0.1.0`, reemplazando el greybox actual.

El objetivo es que el prólogo deje de verse como rectángulos dibujados y pase a verse como un nivel jugable real con escenarios, props, colisiones, puntos de interacción y composición visual coherente.

---

## 0. Lectura obligatoria antes de empezar

Antes de modificar código:

1. Leé toda la documentación relevante:
   - `README.md`
   - `AGENTS.md`
   - `CLAUDE.md`
   - `docs/**/*.md`
   - `docs/art-direction.md`
   - documentación de assets
   - documentación del manifest
   - documentación de render/world/scene system
   - documentación de mission system
   - documentación de interaction system
   - ADRs relacionadas con UI, assets, resolución virtual y render

2. Inspeccioná el código real del proyecto y verificá:
   - cómo se cargan assets;
   - cómo se representan props;
   - cómo se renderizan escenas;
   - cómo se manejan colisiones;
   - cómo funcionan triggers;
   - cómo se definen interactuables;
   - cómo se instancia el prólogo.

3. No dupliques sistemas si ya existen:
   - loader de assets;
   - manifest;
   - SceneStack;
   - world objects;
   - collision system;
   - interaction prompts;
   - mission/objective system;
   - dialogue overlays;
   - dev tools.

4. Actualizá la documentación si la implementación final difiere del diseño actual.

---

## 1. Objetivo general

Transformar el prólogo actual en un vertical slice jugable con assets reales.

Esto implica:

- reemplazar los rectángulos placeholder del refugio;
- integrar personajes y props;
- construir una composición visual coherente;
- agregar colisiones y navegación;
- usar los assets del exterior;
- preparar la transición hacia la entrada del Neometro;
- mantener todo consistente con la narrativa de `La última transmisión`.

---

## 2. Assets a integrar

Usar los assets procesados ya disponibles.

### Personajes
- Emma
- Magga

### Refugio / interior
- cama
- escritorio
- transmisor de La Luciérnaga
- estantería
- puerta industrial
- ventana con lluvia
- generador
- mapa mural / mapa de Neo-Córdoba
- terminal interior / props de trabajo
- cualquier otro prop de safehouse ya procesado

### Exterior
- edificio / refugio exterior
- poste/lámpara de vigilancia
- barricadas / props de calle
- dron enemigo
- terminal exterior o acceso
- entrada abandonada del Neometro
- props ambientales adicionales

---

## 3. Sustituir greybox del refugio

La escena interior actual no puede seguir basada en:
- rectángulos puros;
- labels sobre objetos;
- marcos abstractos;
- objetos irreconocibles.

### Hacer lo siguiente

#### A. Construir el layout real del refugio
El refugio debe sentirse como una habitación habitable y reconocible.

Debe incluir visualmente:
- suelo;
- paredes;
- cama;
- escritorio / mesa de trabajo;
- estantería;
- transmisor;
- mapa;
- ventana;
- puerta;
- generador o fuente de energía;
- props menores.

#### B. Reemplazar cada placeholder por sprites reales
No mantener:
- rectángulos explicativos,
- rótulos permanentes como “DESCANSO” o “TRANSMISOR”,
- cajas de debug visibles en producción.

#### C. Composición
El refugio debe guiar visualmente hacia el transmisor cuando se active.

Usar:
- luz,
- contraste,
- posición,
- líneas de composición,
- props secundarios.

---

## 4. Integración de personaje dentro del refugio

El personaje no debe aparecer flotando ni superpuesto a un panel.

### Ajustar:
- posición inicial;
- punto de spawn;
- capa de dibujo;
- sorting;
- colisión con props;
- escala relativa respecto al entorno;
- navegación libre dentro del refugio.

### Requisitos
- Emma y Magga deben verse correctos dentro del mismo espacio;
- ambos deben caber bien en puertas, pasillos y espacios de interacción;
- el refugio debe poder recorrerse con movimiento natural.

---

## 5. Props interactuables del refugio

Integrar los assets también a nivel funcional.

### Props principales obligatorios
- transmisor;
- mapa;
- fotografía / evidencia;
- mensaje / documento;
- puerta de salida.

### Cada prop debe tener
- sprite;
- posición;
- colisión si corresponde;
- trigger de interacción;
- prompt contextual;
- vínculo con misión/objetivos;
- soporte para teclado y gamepad.

### Ejemplos de prompt
- `[E] Examinar transmisor`
- `[E] Revisar mapa`
- `[E] Leer nota`
- `[E] Salir del refugio`

No usar el prompt genérico permanente:
- `[E] Interactuar`

---

## 6. Colisiones y navegación del refugio

Crear colisiones coherentes para:
- cama;
- escritorio;
- estantería;
- pared;
- puerta;
- transmisor si corresponde;
- props grandes.

### Requisitos
- el jugador no atraviesa muebles;
- puede circular naturalmente;
- existe una ruta clara hacia el transmisor;
- existe una ruta clara hacia la puerta de salida;
- las colisiones no bloquean interacciones válidas;
- DevOverlay puede seguir mostrando hitboxes en modo desarrollo.

No dibujar hitboxes en producción.

---

## 7. Integración del exterior — nivel 1 corto

Después del refugio, integrar la pequeña zona exterior usando assets reales.

### Esta zona debe incluir
- edificio/refugio exterior;
- calle o pasarela;
- iluminación urbana;
- poste/lámpara de vigilancia;
- barricadas o props de calle;
- señalética;
- lluvia / ambiente;
- terminal o punto de acceso si aplica;
- dron enemigo o amenaza;
- ruta hacia la entrada del Neometro.

### Objetivo
La zona no tiene que ser enorme, pero sí debe sentirse como:
- un espacio del mundo,
- no un pasillo vacío,
- ni un canvas con 2 sprites sueltos.

---

## 8. Entrada del Neometro

Integrar el asset de entrada abandonada del Neometro como cierre del prólogo.

### La escena debe comunicar
- lugar reconocible;
- acceso clausurado;
- símbolo o rastro de La Luciérnaga;
- importancia narrativa;
- continuación hacia el capítulo siguiente.

### Técnicamente
- el asset debe renderizarse con escala correcta;
- tener colisión;
- tener un trigger o zona final;
- disparar el cierre del prólogo;
- quedar listo para conectar con la `v0.2.0`.

---

## 9. Escena exterior: Emma y Magga

La zona exterior debe soportar ambas rutas.

### Emma
- puede usar su terminal / hacking en un punto del exterior;
- debe existir una interacción tecnológica visualmente respaldada por assets.

### Magga
- puede enfrentar obstáculo físico o amenaza;
- el entorno debe dar soporte a esa ruta.

No hace falta construir la misión completa otra vez en esta tarea, pero sí dejar los assets integrados en un espacio que permita esas acciones.

---

## 10. Tile / composición del nivel

Si el proyecto ya tiene sistema de tilemap o world chunks, usalo.

Si no existe todavía, implementá una solución razonable y contenida.

### El nivel debe tener capas
- fondo;
- suelo;
- props bajos;
- personaje;
- props altos / overlay;
- iluminación / FX si aplica.

Evitar una escena donde todo se dibuja en el mismo orden sin criterio.

---

## 11. Tamaño y escala de assets en mundo

Respetar el trabajo de normalización ya hecho.

### Asegurar
- Emma y Magga tienen escala coherente entre sí;
- props interiores tienen tamaño creíble;
- puerta, cama, escritorio y estantería no parecen juguetes ni gigantes;
- edificio exterior y entrada Neometro tienen presencia correcta;
- pivotes se usan correctamente.

No corregir tamaños “a ojo” con números mágicos dispersos por el código.
Centralizar en metadata o config donde sea posible.

---

## 12. Lighting / atmósfera básica

No hace falta hacer un sistema complejo, pero sí integrar visualmente los assets.

### Interior
- luz cálida suave en zona de cama;
- cian en terminales o transmisor;
- sombras simples;
- énfasis visual en el transmisor.

### Exterior
- lluvia;
- luz fría urbana;
- faroles / postes;
- brillo o contraste en la entrada del Neometro.

Mantener performance razonable.

---

## 13. Producción vs desarrollo

### En producción
No mostrar:
- rectángulos placeholder;
- labels de diseñador;
- hitboxes;
- triggers;
- nombres técnicos;
- bounds;
- wireframes.

### En desarrollo
Mantener herramientas para:
- mostrar colisiones;
- mostrar interactuables;
- mostrar pivotes si ya existe ese modo;
- inspeccionar world objects.

---

## 14. Manifest / world definitions

Actualizar el manifest y/o la definición del nivel.

Si existe un formato data-driven para el mundo, usarlo.

Por ejemplo, cada prop del refugio o exterior debería poder declararse mediante datos como:

```json
{
  "id": "safehouse_transmitter",
  "asset": "transmitter_luciernaga",
  "position": { "x": 820, "y": 410 },
  "collider": { "x": 0, "y": 0, "w": 64, "h": 48 },
  "pivot": { "x": 0.5, "y": 1.0 },
  "interactable": true,
  "interactionId": "inspect_transmitter",
  "layer": "props_mid"
}
````

Adaptalo a la arquitectura real del repo.

---

## 15. Qué debe desaparecer

Cuando termines, no deberían quedar visibles en producción cosas como:

* cajas vacías simulando muebles;
* marcos explicativos;
* textos fijos de “DESCANSO”, “MAPA”, “TRANSMISOR”, etc.;
* props representados por rectángulos sin identidad;
* escena interior basada en bordes abstractos.

---

## 16. Integración mínima de misión

No te quedes solo en lo visual.

Asegurate de que:

* el transmisor sigue conectado al objetivo;
* la puerta puede ser parte del flujo;
* el mapa puede dar contexto;
* la salida hacia exterior funciona;
* la llegada al Neometro funciona como cierre del prólogo.

---

## 17. Tests / validaciones

Agregar o actualizar tests donde tenga sentido para verificar:

* assets cargables desde el manifest;
* props del refugio instanciados correctamente;
* props exteriores instanciados correctamente;
* pivotes válidos;
* colisiones razonables;
* punto de spawn válido;
* trigger de salida del refugio;
* trigger de final del prólogo en el Neometro;
* producción no usa debug placeholders;
* DevOverlay sí puede mostrar bounds cuando corresponde.

No hace falta hacer tests visuales por screenshot si no encaja con el proyecto.

---

## 18. Verificación manual obligatoria

Recorrer manualmente, en la medida permitida por el entorno:

### Interior

* entrar al refugio;
* moverse;
* bordear cama, escritorio y estantería;
* interactuar con transmisor;
* interactuar con mapa;
* llegar a la puerta.

### Exterior

* salir del refugio;
* recorrer la zona;
* ver props exteriores reales;
* identificar ruta hacia el Neometro;
* llegar a la entrada.

### Final

* activar el cierre del prólogo.

Verificar con Emma y Magga.

---

## 19. Reporte final

Entregame un reporte con:

1. assets integrados;
2. props reemplazados;
3. estructura final del refugio;
4. estructura final del exterior;
5. cambios al manifest / defs;
6. cambios a escenas / world objects;
7. cambios a colisiones;
8. cambios a interacción;
9. tests agregados o ajustados;
10. verificación manual;
11. limitaciones reales;
12. qué assets siguen faltando para pulido futuro.

Incluí además una tabla como esta:

| Zona | Elemento | Antes | Después | Asset usado | Interactable | Collider |
| ---- | -------- | ----- | ------- | ----------- | ------------ | -------- |

---

## 20. Criterio de aceptación

La tarea está completa solo si:

* [ ] el refugio ya no se ve como greybox;
* [ ] los rectángulos fueron reemplazados por assets reales;
* [ ] el personaje se integra correctamente al entorno;
* [ ] el transmisor es reconocible;
* [ ] la cama es reconocible;
* [ ] el escritorio es reconocible;
* [ ] la puerta es reconocible;
* [ ] el mapa es reconocible;
* [ ] la escena exterior usa assets reales;
* [ ] la entrada del Neometro está integrada;
* [ ] las colisiones funcionan;
* [ ] las interacciones funcionan;
* [ ] el flujo del prólogo sigue funcionando;
* [ ] producción no muestra elementos de debug;
* [ ] documentación y manifest quedan alineados con la implementación.

No cierres la tarea solo porque “ya hay sprites en pantalla”.
Quiero una integración real al nivel jugable.

```

---

Si querés, el siguiente paso también te lo puedo dejar hecho:  
te preparo un **tercer prompt para Codex** enfocado en:

1. **tilemap / composición del cuarto**,  
2. **iluminación cyberpunk simple**,  
3. **montaje de la zona exterior con lluvia, faroles y señalética**,  
para que ya quede bastante más cerca de “parece un juego de verdad”.
```


¡Perfecto! Este tercer prompt se concentra en **composición del nivel, tilemap, iluminación, lluvia y ambientación**, para que los assets dejen de sentirse colocados sobre un fondo y formen un mundo coherente.

````md
# Task — Composición, tilemap e iluminación del prólogo de CyberCBA 2077

Trabajá sobre la integración visual existente de la `v0.1.0 — La última transmisión`.

Los assets ya fueron o serán:

- procesados con transparencia;
- normalizados;
- registrados en el manifest;
- integrados como props y estructuras del nivel.

El objetivo de esta tarea es convertir esos objetos aislados en dos espacios visualmente coherentes y jugables:

1. el interior del refugio;
2. la zona exterior que conduce al Neometro.

No alcanza con dibujar sprites sobre un fondo plano. El resultado debe parecer un nivel diseñado, con composición, profundidad, iluminación, atmósfera y lectura jugable.

---

# 0. Lectura obligatoria

Antes de modificar código:

1. Leé todos los archivos de información del proyecto:
   - `README.md`
   - `AGENTS.md`
   - `CLAUDE.md`
   - `.github/**/*.md`
   - `docs/**/*.md`
   - ADRs
   - documentos de arquitectura
   - documentación de render
   - documentación de assets
   - `docs/art-direction.md`
   - documentación de misiones y narrativa

2. Inspeccioná:
   - AssetStore;
   - manifest;
   - sistema de escenas;
   - sistema de mundo;
   - cámara;
   - render texture;
   - capas de dibujo;
   - colisiones;
   - iluminación existente;
   - configuración gráfica;
   - DevOverlay.

3. No dupliques sistemas existentes.

4. Actualizá la documentación cuando la implementación final cambie decisiones previas.

5. Preservá cambios ajenos del worktree.

No te detengas después de la auditoría. Implementá el alcance completo.

---

# 1. Objetivo visual

El prólogo debe transmitir:

- refugio clandestino y habitado;
- tecnología reutilizada;
- deterioro urbano;
- lluvia constante;
- contraste entre luces cálidas y cian;
- sensación de aislamiento;
- peligro creciente;
- una ciudad exterior mucho más grande que el protagonista;
- misterio alrededor de La Luciérnaga;
- atracción visual hacia el Neometro.

Debe dejar de sentirse como:

```text
fondo plano
+ sprites sueltos
+ HUD
````

Y pasar a sentirse como:

```text
espacio construido
+ composición jugable
+ profundidad
+ atmósfera
+ narrativa ambiental
```

---

# 2. Sistema de nivel

## Preferencia: contenido data-driven

Si existe un sistema de mapas o definiciones de mundo, utilizalo.

Si no existe, implementá una solución pequeña y extensible para declarar:

```text
ZoneDefinition
TileLayer
PropInstance
LightInstance
TriggerInstance
SpawnPoint
CollisionShape
AmbientEmitter
```

Ejemplo conceptual:

```json
{
  "id": "prologue_safehouse",
  "bounds": {
    "width": 1280,
    "height": 720
  },
  "layers": [
    {
      "id": "floor",
      "type": "tiles",
      "z": 0
    },
    {
      "id": "low_props",
      "type": "props",
      "z": 10
    },
    {
      "id": "actors",
      "type": "dynamic",
      "z": 20
    },
    {
      "id": "high_props",
      "type": "props",
      "z": 30
    },
    {
      "id": "lighting",
      "type": "effects",
      "z": 40
    }
  ]
}
```

Adaptá esto al código real. No introduzcas JSON si el proyecto ya tiene otra solución adecuada.

---

# 3. Capas de render

Implementá un orden claro de dibujo:

```text
Background
Floor
Floor decals
Low props
Shadows
Actors
Tall props / foreground occlusion
Particles
Lighting
Weather overlays
World UI
HUD
Scene overlays
DevOverlay
```

No dibujar todo en una única función sin estructura.

## Orden por profundidad

Para personajes y props que comparten espacio:

* ordenar por coordenada vertical de los pies o base;
* usar el pivot definido en el manifest;
* evitar que el jugador aparezca delante de un mueble cuando debería estar detrás;
* permitir que la parte alta de algunos props tape parcialmente al personaje.

Ejemplo:

```cpp
sortKey = worldPosition.y + pivotOffset.y;
```

No ordenar mediante coordenadas mágicas específicas para cada objeto.

---

# 4. Tilemap interior del refugio

Construí un tilemap o sistema modular para el refugio.

## Suelo

Crear un suelo reconocible:

* placas metálicas;
* hormigón industrial;
* baldosas técnicas;
* juntas;
* reparaciones;
* manchas;
* cables;
* pequeñas variaciones.

Evitar repetir un único tile de forma evidente.

Agregar variaciones:

```text
floor_clean
floor_worn
floor_cracked
floor_cable
floor_stain
floor_access_panel
```

Las variaciones pueden ser procedurales o assets simples originales.

## Paredes

Las paredes deben tener:

* espesor;
* caras visibles;
* esquinas;
* zócalos;
* conducciones;
* paneles;
* manchas;
* sombras de contacto.

No utilizar solamente un borde rectangular alrededor del escenario.

## Límites

Los límites jugables deben resultar evidentes por la arquitectura:

* paredes;
* muebles;
* puertas;
* columnas;
* desniveles.

No por líneas de debug visibles.

---

# 5. Composición del refugio

La habitación debe contar una historia y permitir una circulación clara.

## Distribución sugerida

### Zona de descanso

* cama;
* mesa pequeña;
* fotografía de Emma y Magga;
* pertenencias personales;
* luz cálida.

### Zona de trabajo

* escritorio;
* terminal;
* herramientas;
* cables;
* mapa mural;
* luz cian moderada.

### Zona del transmisor

* transmisor de La Luciérnaga;
* generador o alimentación;
* cables visibles;
* espacio suficiente para interactuar;
* foco narrativo durante el apagón.

### Salida

* puerta industrial;
* panel de acceso;
* iluminación que indique dirección;
* espacio de transición.

## Circulación

Debe existir:

* un recorrido natural desde el spawn;
* espacio alrededor de los interactuables;
* rutas sin pasillos excesivamente angostos;
* una lectura clara de dónde está la salida;
* margen para futuras animaciones y NPC.

---

# 6. Variantes por protagonista

El refugio puede compartir la misma estructura, pero debe mostrar pequeñas variaciones según el personaje elegido.

## Emma

Agregar o destacar:

* placas electrónicas;
* herramientas de precisión;
* cables;
* interfaces;
* pequeños monitores;
* dispositivo de hacking;
* luz cian más presente en la zona de trabajo.

## Magga

Agregar o destacar:

* guantes;
* vendas;
* equipo de entrenamiento;
* protección física;
* herramientas pesadas;
* marcas de impacto;
* luz ámbar más presente en una zona secundaria.

No crear dos niveles completamente distintos.

Usar props intercambiables o variaciones de decoración.

---

# 7. Iluminación 2D

Implementá un sistema de iluminación simple y eficiente.

No hace falta iluminación físicamente correcta.

## Componentes mínimos

```text
AmbientColor
PointLight
ConeLight
EmissiveSprite
ShadowOverlay
FlickerProfile
```

## Método sugerido

Podés usar:

1. una textura de oscuridad;
2. blend mode multiplicativo para oscurecer;
3. texturas radiales o conos para recuperar luz;
4. sprites emisivos dibujados aparte;
5. sombras simples bajo objetos.

Adaptalo a las posibilidades de raylib y al render actual.

## Luces del refugio

### Luz cálida

Cerca de:

* cama;
* lámpara;
* zona personal.

### Luz cian

Cerca de:

* terminal;
* transmisor;
* mapa;
* dispositivos.

### Luz ámbar

Cerca de:

* puerta;
* generador;
* advertencias.

No iluminar toda la habitación de manera uniforme.

Debe haber:

* áreas de interés;
* zonas de sombra;
* contraste legible;
* caminos visuales.

---

# 8. Secuencia de apagón

La iluminación debe participar de la narrativa.

Implementar una secuencia reutilizable:

```text
Normal
→ caída de tensión
→ parpadeo
→ apagón
→ luces de emergencia
→ transmisor activado
```

## Durante la caída

* reducir gradualmente la iluminación;
* reproducir uno o dos parpadeos controlados;
* apagar terminales;
* cambiar el audio;
* activar luces de emergencia;
* encender el transmisor como nuevo foco visual.

No utilizar flashes agresivos.

Respetar:

* reducción de flashes;
* movimiento reducido;
* accesibilidad.

Con reducción de flashes activa:

* reemplazar parpadeos por fades;
* limitar cambios bruscos;
* conservar claridad narrativa.

---

# 9. Sombras

Agregar sombras de contacto simples a:

* personajes;
* cama;
* mesa;
* estantería;
* transmisor;
* generador;
* edificios;
* dron;
* barricadas;
* poste.

Las sombras deben:

* respetar la base del objeto;
* ser discretas;
* reforzar que el objeto pertenece al mundo;
* evitar el aspecto de “sticker pegado”.

No usar una sombra rectangular genérica para todos los assets.

---

# 10. Zona exterior

Construí una zona breve pero diseñada entre el refugio y el Neometro.

No hacer un mapa enorme.

## Estructura sugerida

```text
Refugio
→ patio o callejón
→ calle deteriorada
→ control de seguridad
→ bifurcación Emma/Magga
→ aproximación al Neometro
→ entrada de estación
```

## Elementos

* exterior del refugio;
* calle o pasarela;
* edificios o siluetas de fondo;
* postes;
* cables;
* barricadas;
* terminal de acceso;
* señales urbanas;
* contenedores;
* charcos;
* residuos;
* tuberías;
* vegetación escasa o deteriorada;
* entrada del Neometro.

Los assets grandes no deben colocarse uno junto a otro sin transición.

Crear elementos intermedios que unifiquen el escenario.

---

# 11. Fondo urbano

Agregar profundidad mediante varias capas.

## Capas sugeridas

```text
Sky / atmospheric gradient
Distant buildings
Midground structures
Playable street
Foreground silhouettes
Rain
Fog
Lighting
```

Usar parallax muy sutil, cuando corresponda.

No mover el fondo de forma exagerada.

La ciudad debe parecer:

* grande;
* húmeda;
* parcialmente apagada;
* habitada a la distancia;
* peligrosa;
* tecnológicamente degradada.

---

# 12. Lluvia

Implementar un sistema de lluvia reutilizable y configurable.

## Debe incluir

* partículas de lluvia;
* dirección;
* velocidad;
* densidad;
* opacidad;
* viento moderado;
* impacto visual controlado.

## Configuración

```text
rain_enabled
rain_density
rain_intensity
rain_angle
reduced_motion
```

## Interior

La lluvia no debe atravesar el techo.

Puede verse:

* detrás de la ventana;
* mediante reflejos;
* mediante sonido;
* mediante gotas aisladas en zonas dañadas.

## Exterior

Debe verse directamente.

No saturar la pantalla con líneas blancas.

Utilizar tonos fríos y opacidad moderada.

---

# 13. Charcos y reflejos

Agregar detalles simples:

* charcos;
* reflejos de luces;
* pequeñas ondas;
* humedad;
* marcas de agua.

No hace falta implementar reflejos reales.

Podés usar:

* sprites;
* overlays;
* pequeñas animaciones;
* versiones oscuras y deformadas de luces.

Los charcos deben mejorar la ambientación sin confundir las colisiones.

---

# 14. Niebla y partículas

Agregar partículas ambientales discretas:

* vapor;
* polvo;
* humo;
* chispas;
* interferencia;
* gotas.

Usar emisores data-driven cuando sea razonable.

Ejemplos:

```text
steam_vent
electrical_sparks
transmitter_interference
metro_fog
```

No crear un sistema distinto para cada escena.

---

# 15. Señalética

Incorporar señalética coherente:

* Línea Oeste;
* acceso restringido;
* salida;
* advertencias;
* numeración de sectores;
* marcas de La Luciérnaga.

No depender de texto generado dentro de los PNG cuando:

* sea ilegible;
* esté en otro idioma;
* tenga errores;
* diga `LEVEL 1`;
* incluya texto ajeno al diseño narrativo.

Cuando corresponda:

* ocultar o editar visualmente el texto incrustado;
* superponer señalética renderizada por el juego;
* mantener textos centralizados y localizables.

La entrada debe decir correctamente:

```text
NEOMETRO
LÍNEA OESTE
FUERA DE SERVICIO
```

No usar `LEVEL 1` como texto diegético salvo que exista una razón narrativa.

---

# 16. Ruta de Emma

Construir una ruta tecnológica breve.

Debe utilizar:

* terminal exterior;
* sensor;
* cámara;
* panel;
* puerta o barrera.

La composición visual debe comunicar:

* qué elemento puede hackearse;
* qué sistema controla;
* qué cambia al completarlo.

El hacking puede:

* apagar una cámara;
* desactivar la barricada;
* abrir un acceso;
* desviar un dron;
* cortar una alarma.

No marcar objetos hackeables permanentemente con marcos enormes.

Usar:

* iluminación;
* pequeño icono contextual;
* prompt;
* pulso al entrar en rango.

---

# 17. Ruta de Magga

Construir una ruta física breve.

Debe utilizar:

* barricada;
* dron;
* obstáculo;
* acceso forzado.

La composición debe permitir:

* espacio para esquivar;
* lectura del ataque del dron;
* margen de cámara;
* colisiones claras;
* ruta alternativa después del enfrentamiento.

No colocar el combate en un pasillo excesivamente estrecho.

---

# 18. Entrada del Neometro

La entrada debe ser el foco final del nivel.

## Composición

* visible parcial o totalmente desde cierta distancia;
* luz cian tenue desde el interior;
* luces ámbar de advertencia;
* acceso dañado;
* humedad;
* cables;
* símbolo reciente de La Luciérnaga;
* sonido de tren lejano.

## Llegada

Al acercarse:

* reducir música;
* aumentar sonido subterráneo;
* destacar el símbolo;
* mostrar una transmisión final;
* bloquear temporalmente el movimiento cuando corresponda;
* iniciar cierre del prólogo.

No finalizar apenas el jugador toca un collider invisible sin feedback.

---

# 19. Cámara

Ajustar la cámara para:

* mostrar suficientemente al personaje;
* permitir leer props y amenazas;
* evitar exceso de espacio vacío;
* anticipar puntos importantes;
* no cortar edificios grandes;
* acompañar el combate.

## Funciones sugeridas

```text
follow player
soft damping
look ahead
temporary focus target
camera bounds
small configurable shake
```

La vibración debe respetar accesibilidad.

No usar movimientos bruscos para interacciones menores.

---

# 20. Escala visual

Revisar todas las escalas en conjunto.

## Personajes

Emma y Magga deben:

* tener altura visual equivalente;
* leerse sobre el suelo;
* no ser demasiado pequeños;
* no parecer gigantes frente a puertas.

## Props

La cama, puertas, terminales y muebles deben respetar proporciones humanas.

## Estructuras

El refugio y el Neometro deben tener presencia, pero no ocupar todo el viewport sin intención.

Centralizar escalas mediante:

* manifest;
* world definition;
* presets;
* categorías.

No dispersar escalas mágicas por las escenas.

---

# 21. Performance

El sistema debe funcionar fluidamente.

Evitar:

* crear texturas cada frame;
* cargar assets repetidamente;
* reconstruir mapas completos por frame;
* ordenar miles de objetos innecesariamente;
* emitir partículas sin límite;
* calcular luces costosas fuera de cámara.

Agregar límites configurables a:

* partículas;
* lluvia;
* luces visibles;
* efectos.

Mantener `TEXTURE_FILTER_POINT` para pixel art.

---

# 22. Configuración gráfica

Integrar opciones existentes:

* intensidad de lluvia;
* movimiento reducido;
* flashes reducidos;
* scanlines;
* vibración de cámara;
* escala de UI;
* calidad de partículas, si corresponde.

Los cambios deben aplicarse sin reiniciar cuando sea seguro.

Persistir configuración.

---

# 23. DevOverlay

Agregar herramientas de diagnóstico para desarrollo:

* capas;
* pivotes;
* sort key;
* luces;
* emisores;
* límites de cámara;
* zonas;
* triggers;
* colisiones;
* conteo de partículas;
* asset ID;
* coordenadas.

Atajos o categorías pueden integrarse con el overlay actual.

No mostrar esto en producción.

---

# 24. Tests

Agregar tests para lógica extraída:

* orden de capas;
* sort key por base;
* carga de definición de zona;
* pivotes válidos;
* luces con configuración válida;
* secuencia de apagón;
* reducción de flashes;
* configuración de lluvia;
* spawn válido;
* camera bounds;
* ruta Emma disponible;
* ruta Magga disponible;
* convergencia en Neometro;
* trigger final del prólogo.

No usar comparación de screenshots como estrategia principal.

---

# 25. Verificación visual obligatoria

Capturar o revisar, en la medida permitida:

## Refugio

1. sin HUD;
2. iluminación normal;
3. apagón;
4. transmisor activo;
5. Emma;
6. Magga;
7. DevOverlay con capas y colisiones.

## Exterior

1. salida del refugio;
2. lluvia;
3. ruta de Emma;
4. ruta de Magga;
5. combate con dron;
6. terminal exterior;
7. entrada del Neometro.

## Resoluciones

* 1280×720;
* 1920×1080;
* ultrawide;
* ventana redimensionada.

No declarar verificaciones que el entorno no permitió realizar.

---

# 26. Criterios de aceptación

La tarea no está completa hasta que:

* [ ] El refugio posee suelo y paredes reconocibles.
* [ ] Existen capas de profundidad.
* [ ] El jugador se oculta correctamente detrás de props altos.
* [ ] Los assets no parecen stickers flotantes.
* [ ] Existen sombras de contacto.
* [ ] La iluminación guía la atención.
* [ ] El apagón cambia visualmente el escenario.
* [ ] El transmisor se vuelve el foco durante la señal.
* [ ] Existe una zona exterior diseñada.
* [ ] Existe lluvia configurable.
* [ ] La lluvia no atraviesa el interior.
* [ ] Existen charcos o humedad.
* [ ] La ciudad tiene capas de fondo.
* [ ] Emma posee una ruta tecnológica clara.
* [ ] Magga posee una ruta física clara.
* [ ] Ambas rutas convergen.
* [ ] La entrada del Neometro es el foco final.
* [ ] La señalética es correcta y legible.
* [ ] No quedan rectángulos de greybox visibles en producción.
* [ ] No quedan textos de assets incorrectos como `LEVEL 1`.
* [ ] La cámara acompaña correctamente.
* [ ] El rendimiento es razonable.
* [ ] Las opciones de accesibilidad afectan los efectos.
* [ ] La documentación refleja la implementación.

---

# 27. Reporte final

Entregá:

## Arquitectura visual

* capas;
* tilemap;
* sorting;
* iluminación;
* partículas;
* cámara.

## Refugio

* composición;
* zonas;
* luces;
* props;
* narrativa ambiental.

## Exterior

* recorrido;
* capas urbanas;
* lluvia;
* props;
* rutas de personajes.

## Neometro

* composición final;
* trigger;
* señalética;
* conexión con la próxima release.

## Archivos

* creados;
* modificados;
* datos;
* manifest;
* documentación.

## Tests

* comandos;
* resultados;
* limitaciones.

## Evidencia

* capturas o descripción de recorridos realmente verificados.

## Pendientes

Solamente mejoras visuales no bloqueantes.

No consideres completa esta tarea porque los assets ya se renderizan.

La aceptación depende de que formen un nivel visualmente coherente, navegable, atmosférico y narrativo.

```

Este bloque debería lograr el salto decisivo: de **“sprites importados”** a **“un escenario con composición, profundidad y atmósfera”**.
```
