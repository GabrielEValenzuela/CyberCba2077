# CyberCBA 2077

> *NeoCBA, un futuro no muy lejano.* La Red gobierna la ciudad prometiendo orden. Un puñado de personas sabe que ese orden tiene precio, y que alguien ya firmó la venta.

**CyberCBA 2077** es un RPG narrativo de ciencia ficción ambientado en NeoCBA, una versión cyberpunk de Córdoba. Elegís a **Emmanuel "Emma" Vázquez**, hacker, o a **Magali "Magga" Temerro**, especialista en combate táctico — dos ex compañeros, ex pareja, separados por una misión que salió mal y por una decisión que ninguno de los dos terminó de perdonarle al otro.

Meses después, uno de los dos recibe un mensaje cifrado que dice lo que ninguno quería escuchar: la misión no falló sola. Alguien abrió la puerta desde adentro. Y el operativo que todos daban por perdido sigue vivo.

Lo que empieza como una búsqueda personal se convierte en la última oportunidad de frenar un acuerdo que va a dejar a más del 70% de NeoCBA sin acceso a lo esencial — mientras un magnate que nadie ha visto todavía observa desde algún lugar, comprando a quienes gobiernan.

## Por qué jugarlo

- **Dos protagonistas, dos formas reales de resolver problemas.** Emma hackea, sabotea, manipula sistemas. Magga lee el campo de batalla, intimida, dispara. La misma escena se resuelve distinto según quién sos — no son skins intercambiables.
- **Decisiones que importan, sin ramificarse al infinito.** Cada escena clave ofrece al menos dos caminos con consecuencias reales — recursos, aliados, diálogos futuros — que convergen de nuevo en la campaña principal en vez de explotar en mil finales inmanejables.
- **Ningún fallo te deja trabado.** Perder un combate o un puzzle no es un Game Over — es una historia distinta de cómo seguís adelante.
- **Combate táctico rápido y con criterio**, sin grilla ni grindeo: cubrirte, atacar, usar un recurso o intentar una maniobra arriesgada, en encuentros de 2 a 5 minutos.
- **Hacking y puzzles que se sienten parte del mundo**, no minijuegos desconectados de la ficción — redirigir energía, descifrar una transmisión, interferir vigilancia.

## El mundo

**NeoCBA** está gobernada por **La RED**, encabezada por el Señor M y su hermana, que usan el aparato del Estado para proteger a los sectores privilegiados. En las sombras opera **Las Luciérnagas**, la resistencia que busca exponer a La RED y devolverle infraestructura y recursos a la población. Y detrás de todo, invisible durante casi toda la campaña, el **Señor del Traje Naranja** — un magnate extranjero que no necesita gobernar territorios: le alcanza con comprar a quienes ya gobiernan.

Tu elección inicial, tus decisiones en el terreno y cómo reconstruís (o no) la relación entre Emma y Magga determinan cómo termina la historia — no con un medidor de relación a la vista, sino con las consecuencias de lo que hiciste.

*(La historia completa, escena por escena, está en [`docs/specs/VS-001.md`](docs/specs/VS-001.md) — sin spoilers acá arriba a propósito.)*

---

## El proyecto, técnicamente

CyberCBA 2077 es también un proyecto académico: el juego no es ficción educativa, pero **construirlo** es el ejercicio. El equipo implementa sus propias estructuras de datos (nada de `std::vector`/`std::map`/smart pointers estándar), maneja memoria con ownership explícito, y sigue una arquitectura donde la lógica de juego nunca depende de raylib — así se puede testear combate, diálogo y progresión sin abrir una ventana.

C++17 · raylib (vía vcpkg) · CMake · GoogleTest.

### Documentación

- [`docs/specs/VS-001.md`](docs/specs/VS-001.md) — spec del Vertical Slice: escenas, puzzles y combate exactos.
- [`docs/adr/`](docs/adr/) — Architecture Decision Records (convenciones de estilo, gestión de dependencias).
- [`INSTALL.md`](INSTALL.md) — dependencias y preparación de Ubuntu.
- [`COMPILE.md`](COMPILE.md) — configuración, compilación, pruebas y documentación de API.
- [`CHANGELOG.md`](CHANGELOG.md) — historial de releases.
- [`AGENTS.md`](AGENTS.md) / [`CLAUDE.md`](CLAUDE.md) — instrucciones para agentes de IA que trabajen en este repo.

### Compilar

Ver [INSTALL.md](INSTALL.md) y [COMPILE.md](COMPILE.md).

### Sumar tu nombre a los créditos

La pantalla de créditos in-game lee automáticamente todos los archivos en [`credits/`](credits/) — sin tocar código. Ver [`credits/README.md`](credits/README.md) para el flujo de un archivo por persona.

### Contribuir código

¿Primera vez en el repo? Empezá por [`AGENTS.md`](AGENTS.md). Los templates de Issues y Pull Requests en `.github/` están pensados para guiarte paso a paso.

## Licencia

Ver [`LICENSE`](LICENSE).
