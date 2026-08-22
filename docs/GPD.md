# CyberCBA 2077

## Game Production & Design Document

**Versión:** 0.1
**Estado:** Preproducción / Vertical Slice
**Motor / Framework:** Raylib
**Lenguaje:** C++17
**Build System:** CMake
**Plataforma inicial:** Desktop
**Duración objetivo de campaña:** 4–6 horas
**Cantidad objetivo:** Prólogo + 10–12 misiones
**Duración objetivo por misión:** 20–30 minutos

---

# 1. Propósito del documento

Este documento es la **fuente de verdad de CyberCBA 2077**.

Debe servir simultáneamente para:

* diseño del videojuego;
* producción;
* implementación en C++17;
* coordinación entre estudiantes;
* generación asistida de código mediante agentes de IA;
* revisión de arquitectura;
* definición del Vertical Slice;
* validación de features mediante criterios de aceptación.

CyberCBA 2077 **no es un videojuego educativo dentro de su ficción**.

El jugador no aprende estructuras de datos mediante puzzles pedagógicos.

El objetivo académico se encuentra en el **proceso de desarrollo del videojuego**: los estudiantes deberán implementar los sistemas necesarios utilizando C++, estructuras de datos propias, algoritmos, manejo de memoria y principios de ingeniería de software.

---

# 2. Contrato de interpretación para agentes de IA

Esta sección tiene prioridad sobre cualquier inferencia realizada por una IA.

## 2.1 Reglas generales

Un agente que implemente CyberCBA 2077:

### MUST

* respetar C++17;
* utilizar Raylib;
* utilizar CMake;
* respetar la arquitectura definida en este documento;
* separar lógica de juego, presentación, input y datos;
* utilizar estructuras de datos custom cuando corresponda;
* respetar ownership y lifetime de memoria;
* generar código legible y modular;
* implementar tests para lógica determinista;
* preservar las reglas narrativas y de gameplay definidas aquí;
* consultar este documento antes de introducir un nuevo sistema.

### MUST NOT

* introducir mecánicas no especificadas;
* convertir CyberCBA 2077 en un juego educativo explícito;
* utilizar contenedores STL;
* utilizar `std::unique_ptr`;
* utilizar `std::shared_ptr`;
* reemplazar estructuras custom por equivalentes de STL;
* introducir variables globales como solución arquitectónica;
* acoplar reglas de gameplay directamente al rendering;
* colocar toda la lógica en `main.cpp`;
* crear sistemas genéricos innecesariamente complejos;
* implementar features futuras durante el Vertical Slice;
* inventar personajes, facciones o eventos canónicos no definidos.

### SHOULD

* preferir soluciones simples;
* utilizar composición antes que herencia cuando sea razonable;
* mantener funciones pequeñas y con responsabilidad clara;
* evitar abstracciones prematuras;
* mantener APIs pequeñas;
* hacer explícitas las precondiciones e invariantes;
* favorecer comportamiento determinista en lógica testeable.

---

# 3. High Concept

**CyberCBA 2077** es un RPG narrativo de ciencia ficción ambientado en **NeoCBA**, una versión cyberpunk ficticia de Córdoba.

El jugador elige al comienzo entre dos protagonistas:

* **Emmanuel “Emma” Vázquez**, hacker;
* **Magali “Magga” Temerro**, especialista en combate táctico.

La campaña sigue una conspiración relacionada con el gobierno de NeoCBA, **La RED**, la resistencia conocida como **Las Luciérnagas**, una misión fallida que destruyó la relación entre Emma y Magga y una operación secreta para vender los recursos de NeoCBA a un poderoso magnate extranjero.

El juego combina:

* exploración e interacción;
* narrativa y decisiones;
* puzzles/hacking;
* combate táctico;
* progresión ligera;
* consecuencias narrativas controladas.

---

# 4. Visión del juego

CyberCBA 2077 debe sentirse como un videojuego completo construido con un **scope deliberadamente pequeño**.

La intención no es producir cientos de assets ni sistemas RPG complejos.

La producción debe obtener:

> **máxima variedad percibida utilizando una cantidad mínima de sistemas y assets.**

El diseño se apoya en:

* pantallas reutilizables;
* fondos reutilizables;
* sprites simples;
* retratos detallados;
* composición de escenas;
* variaciones mediante narrativa;
* decisiones;
* personajes;
* iluminación;
* props;
* estados del mundo.

---

# 5. Pilares de diseño

## 5.1 Dos protagonistas, dos maneras de resolver problemas

Emma y Magga no deben comportarse como skins intercambiables.

La elección inicial debe modificar:

* opciones disponibles;
* resolución de escenas;
* diálogos;
* estrategias;
* riesgos;
* pequeñas consecuencias narrativas.

Cada escena importante debe ofrecer **al menos dos vías de resolución significativas**.

No es necesario que ambas sean completamente exclusivas.

---

## 5.2 Decisiones antes que complejidad

El jugador debe tomar decisiones interesantes utilizando sistemas pequeños.

Preferimos:

> 4 acciones interesantes

antes que:

> 30 acciones redundantes.

Esto aplica especialmente a:

* combate;
* puzzles;
* inventario;
* progresión;
* diálogos.

---

## 5.3 Consecuencias controladas

Las decisiones importan.

Sin embargo, no deben multiplicar exponencialmente la cantidad de contenido.

Las decisiones pueden modificar:

* diálogos posteriores;
* presencia de NPCs;
* recursos;
* ayuda disponible;
* dificultad;
* rutas locales;
* relaciones;
* pequeñas escenas;
* epílogo.

La campaña principal debe converger nuevamente después de las variaciones importantes.

---

## 5.4 Fail Forward

Una escena puede fallar.

Una campaña no debe quedar bloqueada por ese fallo.

El fracaso puede provocar:

* daño;
* pérdida de recursos;
* pérdida de una oportunidad;
* cierre de una ruta;
* empeoramiento de una relación;
* dificultad adicional;
* cambios narrativos.

Siempre debe existir una forma de continuar.

---

## 5.5 Producción reutilizable

No debe crearse un nuevo sistema para cada misión.

Los sistemas centrales deben combinarse de formas diferentes.

---

# 6. Estructura macro

El juego utiliza una estructura inspirada conceptualmente en los RPG clásicos de pocas pantallas sistémicas.

```text
INICIO
  ↓
ELECCIÓN DE PERSONAJE
  ↓
PRÓLOGO
  ↓
MAPA CENTRAL
  ↓
SELECCIÓN DE MISIÓN
  ↓
ESCENAS CONECTADAS
  ├── interacción
  ├── diálogo
  ├── decisión
  ├── puzzle / hacking
  └── combate
  ↓
CIERRE DE MISIÓN
  ↓
MEJORA DE HABILIDAD
  ↓
MAPA CENTRAL
```

---

# 7. Pantallas principales

Todo el juego debe construirse fundamentalmente mediante **cinco sistemas visuales reutilizables**.

## 7.1 Mapa Central

Responsabilidades:

* mostrar NeoCBA;
* mostrar misiones disponibles;
* mostrar misiones completadas;
* representar progresión narrativa;
* permitir seleccionar siguiente misión.

No es un mundo abierto.

---

## 7.2 Escena de misión

Pantalla principal del juego.

Contiene:

* fondo ilustrado;
* sprites pequeños;
* props;
* elementos interactivos;
* NPCs;
* diálogos;
* decisiones.

Los diálogos ocurren **dentro de esta misma pantalla**.

No existe una pantalla separada de visual novel.

---

## 7.3 Puzzle / Hacking

Sistema reutilizable para desafíos breves.

Duración objetivo:

**1–3 minutos.**

Deben existir únicamente **3–4 familias de puzzles base** durante toda la campaña.

La dificultad aumenta mediante:

* cantidad de elementos;
* restricciones;
* límite de movimientos;
* tiempo;
* información incompleta;
* combinaciones de reglas.

No debe inventarse un nuevo minijuego para cada misión.

---

## 7.4 Combate táctico

Sistema táctico sin grilla.

Duración objetivo:

**2–5 minutos.**

Debe utilizar pocas acciones significativas.

Ejemplos conceptuales:

* atacar;
* cubrirse;
* usar recurso;
* ejecutar maniobra;
* retirarse;
* aprovechar entorno.

El combate no utiliza movimiento manual sobre tablero.

---

## 7.5 Post-misión / Progresión

Al finalizar una misión:

1. se muestran consecuencias inmediatas;
2. se ejecuta una escena narrativa breve;
3. el jugador recibe **1 punto de habilidad**;
4. puede mejorar una habilidad;
5. regresa al mapa central.

---

# 8. Estructura de una misión

Plantilla recomendada:

```text
BRIEFING
   ↓
ESCENA 1
   ↓
ESCENA 2
   ↓
DECISIÓN / PUZZLE / COMBATE
   ↓
ESCENA 3
   ↓
CLÍMAX DE MISIÓN
   ↓
CONSECUENCIA
   ↓
CIERRE NARRATIVO
   ↓
MEJORA
   ↓
MAPA
```

Una misión normal debería contener aproximadamente **2–4 escenas conectadas**.

No todas deben contener combate y puzzle simultáneamente.

---

# 9. Protagonistas

## 9.1 Emmanuel “Emma” Vázquez

**Rol:** hacker.

Características visuales canónicas:

* masculino;
* cabello corto oscuro;
* barba recortada;
* piel humana natural;
* hoodie oscuro;
* apariencia observadora y contenida.

### Fantasía de gameplay

Emma modifica sistemas.

Sus soluciones favorecen:

* hacking;
* sabotaje;
* acceso remoto;
* análisis;
* ingeniería;
* manipulación electrónica;
* drones;
* infiltración técnica.

---

## 9.2 Magali “Magga” Temerro

**Rol:** especialista en combate táctico.

Características visuales canónicas:

* femenina;
* rubia;
* ojos verdes;
* piel humana natural;
* apariencia táctica y resiliente.

### Fantasía de gameplay

Magga controla situaciones físicas.

Sus soluciones favorecen:

* combate;
* movimiento táctico;
* intimidación;
* infiltración física;
* supervivencia;
* lectura del campo de batalla.

---

# 10. Elección inicial

El jugador elige **Emma o Magga al comenzar la campaña**.

La elección es permanente durante esa partida.

El personaje no elegido continúa existiendo como personaje central de la historia.

La estructura general de campaña permanece compartida, pero existen diferencias en:

* diálogos;
* opciones;
* resolución;
* riesgos;
* información obtenida;
* escenas específicas;
* perspectiva narrativa.

El objetivo no es producir dos videojuegos completamente diferentes.

---

# 11. Progresión

Cada personaje dispone de **10 habilidades durante una campaña**:

* 5 comunes;
* 5 exclusivas del protagonista seleccionado.

Por lo tanto existen **15 habilidades definidas en total**, pero únicamente 10 se presentan al jugador en cada partida.

Cada habilidad posee:

```text
Nivel I
Nivel II
Nivel III
```

Las mejoras deben desbloquear capacidades concretas.

Evitar:

> +7% daño

Preferir:

> permite intimidar guardias de rango medio.

---

# 12. Habilidades comunes

## Percepción

Detectar:

* detalles;
* trampas;
* rutas;
* contradicciones;
* elementos ocultos.

## Persuasión

Permite:

* negociar;
* convencer;
* reducir hostilidad;
* conseguir cooperación.

## Sigilo

Permite:

* reducir exposición;
* evitar determinadas confrontaciones;
* utilizar rutas discretas.

## Resistencia

Mejora:

* supervivencia;
* tolerancia a consecuencias físicas;
* recuperación frente a fallos.

## Improvisación

Permite:

* aprovechar recursos inesperados;
* convertir fallos en alternativas;
* resolver situaciones con recursos limitados.

---

# 13. Habilidades de Emma

## Hackeo

Acceso y manipulación de sistemas.

## Criptografía

Análisis de:

* mensajes;
* claves;
* transmisiones;
* información cifrada.

## Ingeniería

Interacción con:

* dispositivos;
* infraestructura;
* mecanismos;
* sistemas dañados.

## Drones

Control y aprovechamiento de dispositivos remotos.

## Intrusión

Acceso clandestino a sistemas protegidos y redes.

---

# 14. Habilidades de Magga

## Puntería

Resolución eficiente de enfrentamientos a distancia.

## Combate cercano

Control físico de situaciones a corta distancia.

## Táctica

Lectura del escenario y utilización estratégica del entorno.

## Intimidación

Obtención de cooperación mediante presencia y presión.

## Supervivencia

Adaptación a situaciones hostiles, recursos limitados y daño.

---

# 15. Recursos e inventario

CyberCBA 2077 **no utiliza un inventario RPG tradicional**.

Cada misión dispone aproximadamente de:

**2–4 recursos relevantes.**

Ejemplos:

* munición;
* botiquín;
* dispositivo;
* credencial;
* herramienta;
* carga EMP;
* información.

Los recursos existen para generar decisiones.

No existe:

* loot masivo;
* crafting complejo;
* cientos de objetos;
* gestión de peso;
* economía RPG extensa.

---

# 16. Combate

## Objetivo

Resolver una situación peligrosa mediante decisiones tácticas breves.

## No es

* un JRPG;
* un shooter;
* un tactical RPG sobre grilla;
* un sistema basado en grind.

## Acciones base esperadas

Una implementación concreta podrá utilizar aproximadamente:

```text
ATACAR
CUBRIRSE
MANIOBRA
USAR_RECURSO
RETIRARSE / ALTERNATIVA
```

Las acciones disponibles pueden depender de:

* personaje;
* habilidad;
* escenario;
* estado;
* recursos.

Magga debe poseer ventajas evidentes en combate.

Emma puede combatir, pero generalmente deberá:

* asumir mayor riesgo;
* utilizar dispositivos;
* hackear elementos del escenario;
* buscar una alternativa.

---

# 17. Puzzle y hacking

Los puzzles representan obstáculos reales dentro del mundo.

No son ejercicios académicos sobre estructuras de datos.

Ejemplos de ficción:

* abrir una puerta;
* descifrar una transmisión;
* redirigir energía;
* interferir vigilancia;
* reconstruir información;
* interceptar comunicaciones.

Deben mantenerse:

* breves;
* claros;
* visuales;
* reutilizables.

---

# 18. Diálogo

Los diálogos se muestran sobre la escena actual.

Componentes:

* retrato detallado;
* nombre;
* texto;
* opciones cuando corresponda.

No todos los diálogos deben ofrecer opciones.

Las elecciones aparecen únicamente cuando existe una diferencia significativa.

Puede combinarse:

* diálogo lineal;
* decisiones explícitas;
* decisiones mediante acciones.

---

# 19. Relaciones

No existen:

* barras de afinidad;
* números de confianza;
* puntos de romance;
* estadísticas visibles de relaciones.

El estado de una relación se comunica mediante:

* tono;
* diálogos;
* cooperación;
* disponibilidad;
* decisiones;
* acciones;
* escenas posteriores.

Internamente el juego puede almacenar flags o estados discretos necesarios para determinar consecuencias.

Estos valores no deben mostrarse como puntuación al jugador.

---

# 20. Narrativa

## 20.1 NeoCBA

NeoCBA es una Córdoba ficticia futurista controlada por un sistema gubernamental conocido como **La RED**.

La ciudad posee fuertes desigualdades.

Infraestructura y recursos son utilizados para favorecer sistemáticamente a los sectores más privilegiados.

---

# 21. La RED

La RED representa el aparato político y administrativo dominante de NeoCBA.

Sus líderes son:

* **Señor M**
* **su hermana — nombre TBD**

Ambos gobiernan NeoCBA y utilizan el sistema para proteger los intereses de los sectores privilegiados.

---

# 22. El Señor del Traje Naranja

Magnate extranjero extremadamente poderoso.

No gobierna territorios directamente.

**Compra a quienes los gobiernan.**

Su influencia se extiende mediante:

* empresas;
* contratos;
* intermediarios;
* infraestructura;
* adquisición de recursos;
* gobiernos dependientes.

Durante casi toda la campaña permanece invisible.

Su existencia se percibe mediante:

* documentos;
* conversaciones;
* intermediarios;
* contratos;
* rumores;
* consecuencias.

Su aparición física debe reservarse para una instancia cercana al final.

---

# 23. El plan

Señor M y su hermana preparan secretamente la **venta de NeoCBA al Señor del Traje Naranja**.

El acuerdo permitiría explotar y transferir recursos naturales y estratégicos.

La consecuencia prevista es que **más del 70% de la población perdería acceso efectivo a recursos fundamentales**.

Los sectores privilegiados conservarían acceso y beneficios.

---

# 24. Las Luciérnagas

Movimiento clandestino de resistencia.

Objetivos:

1. exponer públicamente el funcionamiento de La RED;
2. recuperar infraestructura y recursos para la población.

No buscan simplemente reemplazar a un gobierno por otro.

Su lucha incluye:

* información;
* infraestructura;
* comunicaciones;
* refugios;
* transporte;
* recursos;
* evidencia.

---

# 25. El falso líder

Emma y Magga tienen un **amigo de muchos años**.

Nombre:

**TBD**

Durante el comienzo de la historia parece ser:

> el líder de Las Luciérnagas.

En realidad es:

> la cara visible y principal líder operativo de la organización.

No conoce necesariamente toda la estructura.

Es una figura cercana, confiable y emocionalmente importante para ambos protagonistas.

---

# 26. El verdadero líder

Nombre:

**TBD**

Características:

* ex miembro de La RED;
* desertor;
* miembro de Las Luciérnagas;
* verdadero líder oculto de la organización;
* posee información crítica sobre el gobierno;
* conoce detalles del acuerdo con el Señor del Traje Naranja.

Su identidad como líder se mantiene inicialmente oculta incluso para muchos integrantes de Las Luciérnagas.

---

# 27. Prólogo — La misión fallida

El juego comienza **antes de la ruptura entre Emma y Magga**.

Son pareja.

Trabajan juntos para Las Luciérnagas.

Una operación importante es cuidadosamente planificada.

Durante la misión descubren que la operación ha sido comprometida.

### Conflicto

Emma está cerca de obtener información crítica.

Quiere continuar.

Magga considera que el equipo corre demasiado peligro.

Quiere abortar y evacuar.

Ambas posiciones son razonables.

Ninguno posee toda la información.

La decisión termina en desastre.

---

# 28. Consecuencias del prólogo

La misión provoca:

* desaparición/captura de un miembro importante;
* pérdida de información crítica;
* exposición parcial de Las Luciérnagas;
* paranoia;
* debilitamiento operativo;
* conflicto entre Emma y Magga.

El miembro desaparecido es posteriormente revelado como:

> el verdadero líder oculto de Las Luciérnagas.

---

# 29. Ruptura

Emma y Magga **no creen que el otro sea un traidor**.

Se culpan mutuamente por la decisión tomada durante la operación.

A pesar de seguir amándose, llegan a una conclusión:

> su relación está interfiriendo con el objetivo de derrotar a La RED.

Deciden separarse.

La ruptura nace del deber y del dolor, no de la desaparición del amor.

---

# 30. Salto temporal

Después del prólogo ocurre un salto temporal.

Duración exacta:

**TBD**

Emma y Magga permanecen distantes.

Ambos continúan luchando de diferentes maneras.

---

# 31. El mensaje

El personaje seleccionado recibe un mensaje cifrado.

El otro protagonista **no lo recibe**.

El remitente parece inicialmente desconocido.

El contenido revela que la misión original no falló únicamente por las decisiones de Emma y Magga.

Existió una intervención desde dentro.

Conceptualmente:

> alguien abrió la puerta desde adentro.

El mensaje contiene evidencia suficientemente específica para justificar una investigación.

---

# 32. Acto I — Investigación

El protagonista seleccionado comienza a investigar solo.

Durante aproximadamente **2–3 misiones**:

* sigue rastros;
* recupera información;
* encuentra contradicciones;
* descubre que la historia oficial no coincide con los hechos.

Finalmente descubre que:

* el miembro desaparecido sigue vivo;
* él envió el mensaje cifrado;
* La RED está trasladándolo;
* fue anteriormente integrante de La RED;
* posee información crítica.

Esto provoca el reencuentro entre Emma y Magga.

---

# 33. Fin del Acto I

Emma y Magga llegan, por caminos diferentes, al mismo objetivo:

> rescatar al miembro desaparecido.

Deben colaborar nuevamente.

El reencuentro es incómodo.

No existe reconciliación romántica inmediata.

Durante el rescate descubren que el prisionero es:

> el verdadero líder oculto de Las Luciérnagas.

También comienzan a comprender la existencia del acuerdo para vender NeoCBA.

---

# 34. Acto II — Reconstrucción

La RED ha destruido o identificado gran parte de la infraestructura clandestina.

Las Luciérnagas deben reconstruirse.

El mapa central se vuelve parcialmente abierto.

Se presentan varias misiones disponibles simultáneamente.

El jugador puede decidir el orden.

Ejemplos conceptuales:

* recuperar comunicaciones;
* rehabilitar un refugio;
* recuperar contactos;
* obtener recursos;
* liberar infraestructura;
* recuperar inteligencia;
* volver a conectar células.

El orden puede modificar:

* diálogos;
* ayuda disponible;
* recursos;
* dificultad;
* contexto.

No altera radicalmente la campaña principal.

---

# 35. Reconstrucción emocional

Durante el Acto II también comienza lentamente la reconstrucción del vínculo Emma–Magga.

No existe reconciliación inmediata.

Debe mostrarse mediante:

* pequeños gestos;
* cooperación;
* conversaciones;
* vulnerabilidad;
* recuerdos;
* decisiones;
* protección mutua.

El romance permanece secundario al conflicto principal.

---

# 36. La traición

Hacia el final del Acto II se revela que:

> el amigo de toda la vida de Emma y Magga fue quien entregó información a La RED.

Es el personaje que inicialmente parecía ser el líder de Las Luciérnagas.

---

# 37. Motivación del traidor

No actúa principalmente por dinero o poder.

La RED amenaza a alguien que ama.

Además, comienza a convencerse de que enfrentarse directamente al acuerdo puede generar una catástrofe todavía mayor para NeoCBA.

Su traición surge de:

* miedo;
* amor;
* coerción;
* racionalización;
* desesperación.

Debe ser posible condenar sus acciones y al mismo tiempo comprender sus motivaciones.

---

# 38. Decisión sobre el traidor

Cuando se descubre la verdad, el jugador decide qué hacer con él.

Opciones conceptuales:

* perdonarlo;
* apartarlo de Las Luciérnagas;
* entregarlo.

La decisión modifica:

* su participación posterior;
* diálogos;
* algunas escenas;
* epílogo.

No crea un Acto III completamente diferente.

---

# 39. Acto III — La venta de NeoCBA

Después de reconstruir Las Luciérnagas, aparece una amenaza inmediata:

> el acuerdo para vender NeoCBA está a punto de firmarse.

El objetivo pasa a ser:

1. conseguir evidencia irrefutable;
2. sobrevivir a la respuesta de La RED;
3. difundir esa evidencia;
4. impedir que el acuerdo pueda cerrarse en secreto.

El Acto III debe sentirse más urgente que los anteriores.

---

# 40. Señor del Traje Naranja

Su aparición física ocurre cerca del final.

Hasta entonces debe existir principalmente mediante su influencia.

El encuentro físico debe sentirse significativo precisamente porque el jugador ha pasado gran parte del juego viendo únicamente sus consecuencias.

---

# 41. Emma y Magga — resolución

La reconciliación emocional ocurre durante el Acto III.

El resultado depende orgánicamente de las decisiones acumuladas.

Posibles estados:

### Reconciliación romántica

Emma y Magga vuelven a construir su relación como pareja.

### Reconciliación afectiva

Se perdonan y recuperan una relación profunda, pero no vuelven como pareja.

### Vínculo quebrado

Existe cierta comprensión, pero las heridas siguen siendo demasiado profundas.

No existe un medidor visible que determine estos resultados.

---

# 42. Final

Existe **un desenlace principal compartido**.

No deben construirse tres campañas finales completamente diferentes.

Las decisiones anteriores modifican el epílogo.

Pueden cambiar:

* supervivencia de personajes;
* destino del traidor;
* estado de Las Luciérnagas;
* consecuencias locales;
* vínculo Emma–Magga;
* futuro inmediato de NeoCBA.

---

# 43. Dirección visual

## Estilo

Híbrido.

Combina:

* fondos ilustrados;
* sprites pequeños;
* retratos detallados;
* UI reutilizable.

No se requiere animación compleja.

---

# 44. Escenas

Una escena se compone aproximadamente de:

```text
BACKGROUND
+ PROPS
+ CHARACTER SPRITES
+ UI
+ PORTRAITS
+ TEXT
+ OPTIONAL EFFECTS
```

Esto permite reutilizar el mismo fondo cambiando:

* personajes;
* iluminación;
* props;
* diálogos;
* estado;
* efectos.

---

# 45. Personajes

Durante gameplay:

> sprites pequeños y simples.

Durante diálogos:

> retratos detallados.

No deben producirse animaciones extensas salvo que una necesidad concreta del gameplay las justifique.

---

# 46. Política de assets

Principio:

> No crear un asset único cuando una combinación de assets existentes pueda comunicar correctamente la escena.

Priorizar:

* reutilización;
* modularidad;
* variantes;
* recolocación de props;
* iluminación;
* overlays.

Evitar:

* ilustración exclusiva para cada diálogo;
* fondos únicos innecesarios;
* animaciones cosméticas costosas.

---

# 47. Vertical Slice

El Vertical Slice debe contener:

> **Prólogo completo + primera misión del Acto I**

Duración objetivo:

**30–45 minutos.**

Debe demostrar la experiencia final del juego.

No debe ser una demo tecnológica desconectada.

---

# 48. Vertical Slice — funcionalidades obligatorias

Debe incluir:

## Inicio

* pantalla inicial;
* elección Emma/Magga;
* inicio de campaña.

## Narrativa

* Emma y Magga todavía juntos;
* misión planificada;
* conflicto entre continuar y evacuar;
* fracaso;
* consecuencias;
* ruptura;
* salto temporal;
* mensaje cifrado.

## Mapa

* mapa central funcional;
* selección de la primera misión.

## Gameplay

* escenas conectadas;
* interacción;
* diálogo integrado;
* al menos una decisión;
* al menos un desafío de lógica/hacking;
* al menos un combate táctico;
* al menos dos vías de resolución en una escena importante.

## Consecuencias

* éxito o fallo local;
* continuidad garantizada;
* cambio narrativo visible.

## Progresión

* final de misión;
* 1 punto de habilidad;
* selección de mejora;
* retorno al mapa.

---

# 49. Vertical Slice — criterios de aceptación

El Vertical Slice se considera completo cuando:

* compila mediante CMake;
* ejecuta en desktop;
* permite completar el flujo completo;
* permite seleccionar Emma o Magga;
* ambas elecciones producen diferencias reales;
* no existen bloqueos de progreso;
* existe al menos un escenario de fail-forward;
* existe al menos una decisión narrativa;
* existe puzzle/hacking funcional;
* existe combate funcional;
* existe progresión funcional;
* existe navegación mediante mapa;
* la lógica crítica posee tests;
* no utiliza contenedores STL;
* no utiliza smart pointers estándar;
* no existen leaks conocidos en los flujos principales;
* no existen crashes durante una partida normal;
* rendering y lógica permanecen desacoplados.

---

# 50. Stack tecnológico

```text
Language: C++17
Framework: Raylib
Build: CMake
Tests: Framework TBD
Target: Desktop
```

---

# 51. Uso de STL

## Permitido

Utilidades estándar que no reemplacen las estructuras que deben desarrollar los estudiantes.

Ejemplo explícitamente permitido:

```cpp
std::string
```

También pueden utilizarse funcionalidades básicas del lenguaje y biblioteca estándar cuando no constituyan una estructura de datos equivalente a las requeridas académicamente.

## Prohibido

Contenedores STL.

Incluyendo, pero no limitado a:

```cpp
std::vector
std::list
std::forward_list
std::deque
std::queue
std::stack
std::map
std::unordered_map
std::set
std::unordered_set
```

---

# 52. Gestión de memoria

Prohibido:

```cpp
std::unique_ptr
std::shared_ptr
```

Permitido:

* raw pointers;
* wrappers custom desarrollados por estudiantes.

Todo objeto dinámico debe tener ownership explícito.

Para cada puntero relevante debe poder responderse:

1. ¿Quién crea el objeto?
2. ¿Quién posee el objeto?
3. ¿Quién puede observarlo?
4. ¿Quién lo destruye?
5. ¿Cuándo deja de ser válido?

---

# 53. Regla de ownership

Preferencia:

```text
OWNER
  ↓
creates
  ↓
OBJECT
  ↓
destroyed by OWNER
```

Los punteros no propietarios deben considerarse **observers**.

Un observer:

* no elimina el objeto;
* no transfiere ownership;
* no asume lifetime superior al owner.

---

# 54. Rule of Three / Five

Toda estructura custom que posea memoria dinámica debe definir correctamente, según corresponda:

* destructor;
* copy constructor;
* copy assignment;

y cuando se utilice movimiento:

* move constructor;
* move assignment.

No debe permitirse una copia superficial accidental de memoria poseída.

---

# 55. Estructuras de datos custom

Las estructuras necesarias para el juego deben ser implementadas por los estudiantes.

La API pública de cada estructura debe estar desacoplada de los sistemas que la utilizan.

Los sistemas de gameplay no deben depender de detalles internos como:

* nodos;
* índices internos;
* punteros internos.

Ejemplo conceptual:

```text
GameplaySystem
      ↓
Public ADT API
      ↓
Custom Data Structure
```

La selección exacta de estructuras académicas requeridas para cada milestone puede definirse en especificaciones de implementación posteriores.

---

# 56. Arquitectura

Objetivo:

> separar lógica de gameplay de Raylib.

Arquitectura conceptual:

```text
┌────────────────────────────┐
│          GAME APP          │
└─────────────┬──────────────┘
              │
      ┌───────┴───────┐
      ↓               ↓
 GAME LOGIC        PRESENTATION
      │               │
      ↓               ↓
SYSTEMS / DATA      RAYLIB
```

Raylib no debe convertirse en el lugar donde vive la lógica del juego.

---

# 57. Game States

Estados principales sugeridos:

```text
MainMenu
CharacterSelection
CentralMap
MissionScene
Puzzle
Combat
PostMission
Pause
GameEnding
```

Las transiciones deben ser explícitas.

Evitar cadenas arbitrarias de `if` dispersas por el proyecto.

---

# 58. Separación de responsabilidades

## Input

Detecta acciones del jugador.

No resuelve reglas de gameplay.

## Game Logic

Decide qué ocurre.

Debe ser testeable sin renderizar.

## Rendering

Representa el estado.

No decide las reglas.

## Audio

Reproduce feedback sonoro.

No modifica reglas.

## Data

Describe:

* misiones;
* escenas;
* diálogos;
* habilidades;
* estados.

---

# 59. Estructura de proyecto sugerida

```text
CyberCBA2077/
│
├── CMakeLists.txt
│
├── assets/
│   ├── backgrounds/
│   ├── characters/
│   ├── portraits/
│   ├── props/
│   ├── ui/
│   ├── audio/
│   └── fonts/
│
├── include/
│   ├── core/
│   ├── game/
│   ├── states/
│   ├── systems/
│   ├── structures/
│   └── ui/
│
├── src/
│   ├── core/
│   ├── game/
│   ├── states/
│   ├── systems/
│   ├── structures/
│   └── ui/
│
├── tests/
│   ├── structures/
│   ├── combat/
│   ├── progression/
│   └── game_state/
│
└── docs/
    ├── GPD.md
    └── specs/
```

La estructura puede refinarse si existe una razón técnica documentada.

---

# 60. Convenciones C++

## Clases

```text
PascalCase
```

Ejemplo:

```cpp
MissionManager
CombatSystem
GameState
```

## Funciones y métodos

```text
camelCase
```

Ejemplo:

```cpp
startMission()
applyDamage()
changeState()
```

## Variables

```text
camelCase
```

## Constantes

Convención única definida por proyecto.

Recomendado:

```text
UPPER_SNAKE_CASE
```

## Archivos

Recomendado:

```text
PascalCase.hpp
PascalCase.cpp
```

Una clase principal por archivo cuando resulte razonable.

---

# 61. Clean Code

El proyecto debe priorizar:

* nombres descriptivos;
* funciones pequeñas;
* responsabilidades claras;
* baja duplicación;
* invariantes explícitas;
* errores detectables;
* interfaces pequeñas.

Evitar:

```text
GameManager
SuperManager
Utility
Helper
Common
Stuff
```

cuando estos nombres oculten responsabilidades ambiguas.

---

# 62. Funciones

Una función debe realizar una tarea conceptualmente clara.

Evitar funciones que simultáneamente:

* leen input;
* modifican gameplay;
* reproducen audio;
* renderizan;
* guardan partida.

Separar responsabilidades.

---

# 63. Clases

Una clase debe representar:

* una entidad;
* una responsabilidad;
* un servicio;
* un sistema claramente definido.

No crear clases solamente para “hacerlo orientado a objetos”.

---

# 64. Herencia

La herencia se utiliza cuando existe una relación semántica real.

No debe utilizarse como mecanismo predeterminado de reutilización.

Preferir composición cuando simplifique el diseño.

---

# 65. Globales

Evitar estado global mutable.

No crear managers globales como mecanismo principal de acceso.

Dependencias importantes deben ser explícitas.

---

# 66. Rendering

El rendering debe trabajar sobre estado ya calculado.

Ejemplo conceptual:

```text
Input
   ↓
Update
   ↓
Game State
   ↓
Render
```

No:

```text
Render
   ↓
decide gameplay
```

---

# 67. Testing

El Vertical Slice debe incorporar testing automatizado.

## Debe testearse

* estructuras custom;
* lógica de combate;
* progresión;
* transición de estados;
* reglas de misión;
* fail-forward;
* condiciones;
* lógica determinista.

## No es obligatorio testear automáticamente

* renderizado Raylib;
* alineación visual;
* animación;
* audio.

---

# 68. Testabilidad

La lógica debe poder ejecutarse sin abrir una ventana Raylib cuando sea razonable.

Ejemplo:

```text
CombatSystem
```

debe poder recibir un estado, una acción y producir un resultado sin depender directamente de:

```cpp
DrawTexture()
GetMousePosition()
BeginDrawing()
```

---

# 69. Definition of Done para una feature

Una feature se considera terminada solamente si:

* compila;
* cumple la especificación;
* respeta arquitectura;
* no rompe tests existentes;
* agrega tests cuando corresponde;
* no introduce contenedores STL;
* no introduce smart pointers estándar;
* ownership está documentado;
* no existe código muerto relevante;
* no contiene placeholders no documentados;
* está integrada al flujo real del juego.

---

# 70. Reglas para implementación mediante Claude u otra IA

Ante una tarea:

1. leer GPD;
2. identificar sistemas afectados;
3. inspeccionar código existente;
4. respetar APIs existentes;
5. proponer cambios mínimos;
6. implementar;
7. agregar/modificar tests;
8. compilar;
9. ejecutar tests;
10. reportar resultado.

El agente **no debe reescribir arquitectura completa** salvo requerimiento explícito.

---

# 71. Ante ambigüedad

Si algo no está definido:

### Primero

Buscar en:

```text
GPD
→ specs/
→ código existente
→ tests
```

### Si sigue sin estar definido

No convertir una suposición en canon.

Implementar la solución mínima reversible o marcar:

```text
TBD
```

según corresponda.

---

# 72. No objetivos

CyberCBA 2077 no pretende incluir en su versión inicial:

* mundo abierto;
* navegación continua por toda NeoCBA;
* cientos de NPCs;
* crafting;
* economía compleja;
* loot procedural;
* combate sobre grilla;
* árboles gigantes de habilidades;
* multiplayer;
* generación procedural de mapas;
* cinemáticas 3D;
* animaciones extensas;
* simulación física avanzada;
* sistema de romance numérico;
* múltiples campañas completamente independientes.

---

# 73. Scope de producción

La prioridad es:

```text
GAMEPLAY FUNCIONAL
        >
NARRATIVA FUNCIONAL
        >
CLARIDAD VISUAL
        >
PULIDO
        >
CANTIDAD DE CONTENIDO
```

Nunca debe sacrificarse estabilidad o claridad arquitectónica para incorporar más contenido.

---

# 74. Roadmap inicial

## Fase 0 — Preproducción

* GPD;
* arquitectura;
* especificaciones;
* wireframes;
* assets mínimos;
* prototype técnico.

## Fase 1 — Vertical Slice

* prólogo;
* primera misión;
* sistemas fundamentales;
* testing;
* flujo completo.

## Gate 1

Evaluar:

* ¿es divertido?
* ¿las decisiones funcionan?
* ¿Emma y Magga se sienten diferentes?
* ¿los sistemas son reutilizables?
* ¿el código puede continuar creciendo?
* ¿el costo de producir una nueva misión es razonable?

Sólo después se continúa.

## Fase 2 — Producción

Acto I completo.

## Fase 3

Acto II.

## Fase 4

Acto III.

## Fase 5

Integración, balance y pulido.

---

# 75. Principio rector

Ante cualquier decisión futura de diseño o implementación debe aplicarse esta pregunta:

> **¿Esta feature mejora significativamente CyberCBA 2077 utilizando una cantidad razonable de complejidad, código y assets?**

Si la respuesta es no:

> no debe incorporarse.

---

# 76. Estado actual de decisiones

## Cerrado

* RPG narrativo;
* mapa central;
* misiones por capítulos;
* escenas conectadas;
* Emma/Magga seleccionables;
* elección permanente por campaña;
* exploración/interacción;
* diálogo;
* decisiones;
* combate táctico;
* hacking/puzzles;
* fail-forward;
* consecuencias controladas;
* progresión;
* 10 habilidades por campaña;
* tres niveles por habilidad;
* un punto de mejora por misión;
* relaciones invisibles;
* reconstrucción progresiva Emma/Magga;
* estructura narrativa en tres actos;
* traición;
* falso líder;
* líder oculto;
* Señor M;
* hermana del Señor M;
* Señor del Traje Naranja;
* venta de NeoCBA;
* estilo híbrido;
* sprites pequeños;
* retratos detallados;
* C++17;
* Raylib;
* CMake;
* estructuras custom;
* raw pointers / wrappers custom;
* sin contenedores STL;
* sin smart pointers estándar;
* testing automatizado;
* Vertical Slice de prólogo + primera misión.

## TBD

* nombre del amigo/traidor;
* nombre del verdadero líder;
* nombre de la hermana del Señor M;
* duración exacta del salto temporal;
* identidad de la persona amenazada por La RED;
* nombres de misiones;
* nombres definitivos de distritos;
* diseño final de los 3–4 puzzles;
* reglas matemáticas exactas del combate;
* framework de testing;
* resolución interna objetivo;
* esquema definitivo de controles;
* formato de persistencia;
* estructura de guardado;
* listado definitivo de assets del Vertical Slice.

---

# 77. Próximo documento recomendado

El próximo artefacto de producción debe ser:

> **Vertical Slice Specification — VS-001**

Debe transformar este GPD en requisitos implementables para:

```text
Prólogo
+
Primera misión del Acto I
```

incluyendo:

* escenas exactas;
* diálogos;
* decisiones;
* estados;
* puzzles;
* combate;
* assets;
* estructuras de datos requeridas;
* clases;
* interfaces;
* criterios de aceptación;
* tests;
* Definition of Done.

Ese documento debe ser suficientemente preciso para que un agente como Claude pueda implementar el Vertical Slice sin tener que diseñar el juego mientras programa.
