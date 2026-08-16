# Squad Tasks — CyberCBA 2077

**Estado:** Draft — training assignment
**Audiencia:** estudiantes del curso, organizados en 4 escuadras de 4 personas
**Documentos superiores:** `AGENTS.md`, `docs/GPD.md`, `docs/TAD-001.md`,
`docs/adr/ADR-001-cpp-style-guide-merge.md`, `docs/specs/VS-001.md`,
`CHANGELOG.md` (sección "Known limitations")

---

## 0. Propósito

Este documento asigna trabajo real de gameplay a 4 escuadras de 4
desarrolladores cada una (16 personas). Cada escuadra construye **una
feature nueva y acotada** que extiende de forma plausible el Vertical Slice
(VS-001) o cierra una limitación documentada en `CHANGELOG.md`. No son
ejercicios sintéticos: cada una agrega algo que el juego descrito en
`docs/GPD.md` necesita, en un momento en que el esqueleto arquitectónico
(`GameStateMachine`, `MissionSystem`, `CombatSystem`, `DialogueSystem`,
`ProgressionSystem`, las estrategias de NPC) ya existe y está testeado.

**No toquen `include/cybercba/structures/`.** `DynamicArray`, `Queue`,
`Stack` y `DirectedGraph` son un ejercicio de estructuras de datos
reservado aparte para el equipo — están deliberadamente sin cuerpo
(`AGENTS.md`, VS-001 §12.2). Ninguna de las 4 features de abajo requiere
implementarlas; si su diseño empieza a depender de una de ellas, es señal
de que se desviaron del scope — vuelvan a leer la sección de la feature.

### Cómo se forman las escuadras

4 personas por escuadra. Repártanse el trabajo real (ver "Suggested task
split" en cada sección) — no es "una persona diseña, tres codean". Cada
squad debe poder explicar en review **por qué** cada uno de los 4
requisitos técnicos (OOP / polimorfismo / punteros / recursión) existe en
su feature, no solo mostrar que están presentes.

### Qué significa "terminado"

- Compila con `cmake --preset dev && cmake --build build/dev`.
- `ctest --test-dir build/dev --output-on-failure` en verde, incluyendo
  tests nuevos para la lógica agregada (TAD-001 §31/§32: testear lógica
  determinista sin ventana).
- Sigue ADR-001 (`m_` en miembros, guards `#ifndef`, Allman, 120 columnas,
  etc.) y TAD-001 (dirección de dependencia §6, ownership §12-14, sin
  contenedores STL ni `std::unique_ptr`/`std::shared_ptr`, `cybercba_core`
  sin `<raylib.h>`).
- `clang-format -i` corrido sobre los archivos tocados antes de abrir PR.
- PR abierta con el template de `.github/PULL_REQUEST_TEMPLATE.md`
  completo (tipo de cambio, issue linkeada, resumen, checklist tildado).
- Si algo queda genuinamente ambiguo, no se inventa canon (TAD-001 §71):
  se pregunta en Discussions o se implementa la solución mínima reversible
  dejando explícito por qué.

---

## Squad A — Mission Unlock Map (`missions/`, `game/`)

### Feature y pitch

Hoy el Mapa Central de VS-001 expone un único nodo de misión siempre
disponible (VS-001 §4: "un grafo trivial de 1 nodo alcanzable"). GPD
describe una campaña de 10-12 misiones (§0) que el mapa central deberá
exponer más adelante, con misiones que se desbloquean según progreso
(flags narrativos, skills, misiones previas completadas). Squad A
construye el sistema de **desbloqueo de misiones** que hace eso posible:
una cadena de nodos de misión, cada uno con condiciones de desbloqueo, y
la lógica que, dado el `CampaignState` actual, determina qué misiones son
seleccionables.

### Por qué esta feature, por qué ahora

VS-001 §4 dice explícitamente que el mapa central de un solo nodo es una
simplificación deliberada "para no sobreproducir contenido" en el VS, no
una limitación permanente — la campaña completa (GPD) necesita el sistema
real antes de poder autorar la misión 2. `CentralMap` en `Game`
(`include/cybercba/game/Game.hpp`) hoy solo transiciona de estado; no hay
ningún sistema que decida *qué* está disponible. Esta feature no reemplaza
`DirectedGraph` (esa estructura sigue reservada y genérica) — construye una
estructura específica de dominio, pequeña, para nodos de misión con sus
condiciones de desbloqueo, que es exactamente el tipo de sistema real que
GPD anticipa y que hoy no existe en ningún lado del código.

### Concrete requirements

- Nuevo tipo `MissionNode` (`include/cybercba/missions/MissionNode.hpp`):
  datos inmutables de una misión del mapa (id, nombre, puntero a lista de
  nodos prerrequisito) + una condición de desbloqueo asociada.
- Interfaz `IUnlockCondition` con `bool isSatisfied(const
  game::CampaignState&) const`. Al menos 2 implementaciones concretas,
  p. ej. `PriorMissionCompletedCondition` (requiere que otro `MissionNode`
  esté marcado completado) y `FlagCondition` (requiere un flag narrativo
  específico en `CampaignState`, p. ej. `intelComplete`).
- Un `MissionMapSystem` (`include/cybercba/missions/MissionMapSystem.hpp`)
  que, dado un `CampaignState` y una lista de `MissionNode`, calcula el
  conjunto de misiones actualmente seleccionables.
- La resolución de si un nodo está desbloqueado debe considerar **cadenas
  de prerrequisitos** (misión C requiere B, B requiere A) — no solo el
  caso de un único prerrequisito directo.
- No se autora contenido narrativo de misiones 2-3 (fuera de scope, VS-001
  §16) — la feature es el sistema, con datos de prueba mínimos (2-3 nodos
  ficticios en los tests) para demostrarlo.

### Dónde aparecen OOP / polimorfismo / punteros / recursión

- **OOP:** `MissionMapSystem` tiene una responsabilidad única (decidir qué
  está desbloqueado), separada de `MissionSystem` (orquesta escenas dentro
  de una misión ya elegida) y de `Game` (dueño del `CentralMap` state).
- **Polimorfismo:** `IUnlockCondition` con `PriorMissionCompletedCondition`
  / `FlagCondition` (mismo espíritu que `IPersuasionStrategy`) — así se
  agregan condiciones nuevas (p. ej. "requiere skill de nivel II") sin
  tocar `MissionMapSystem`.
- **Punteros:** `MissionNode` referencia sus prerrequisitos vía
  `const MissionNode* const* pPrerequisites` (array de punteros
  observadores — el nodo no crea ni destruye los nodos prerrequisito, solo
  los observa). Definan explícitamente en el header quién es owner del
  array de `MissionNode` completo (sugerencia: almacenamiento estático,
  igual que `SceneDefinition`/`MissionDefinition` hoy).
- **Recursión:** `isUnlocked(node)` debe evaluar la cadena de
  prerrequisitos recursivamente (A depende de B depende de C...) — esto es
  honestamente recursivo (es un recorrido de grafo dirigido acíclico) y
  además es exactamente el tipo de algoritmo que se reutiliza en Squad B/D
  desde ángulos distintos, así que documenten bien la condición de corte
  (nodo sin prerrequisitos) y el caso de ciclo (debe fallar de forma
  segura, no colgar el programa — decidan y documenten el comportamiento).

### Suggested task split (4 devs)

1. **Dev 1:** `IUnlockCondition` + `FlagCondition` + tests unitarios.
2. **Dev 2:** `PriorMissionCompletedCondition` + `MissionNode` + tests;
   define el contrato de ownership del array de prerrequisitos.
3. **Dev 3:** `MissionMapSystem::isUnlocked` recursivo + detección de ciclo
   + tests (incluyendo un caso de cadena de 3 niveles y un caso de ciclo).
4. **Dev 4:** integración mínima en `Game`/`CentralMap` (exponer qué
   misiones están seleccionables al estado actual) + verificación manual
   corriendo `cybercba_app`.

### Acceptance criteria

- [ ] `IUnlockCondition` y 2+ implementaciones, con tests para cada una.
- [ ] `MissionMapSystem::isUnlocked` resuelve cadenas de 3+ niveles
      correctamente (test dedicado) y no crashea/cuelga ante un ciclo.
- [ ] Sin contenedores STL, sin smart pointers.
- [ ] `clang-format` sin diffs.
- [ ] Tests nuevos corren bajo `ctest --preset dev`.
- [ ] No se modifica ni se referencia el cuerpo de `DirectedGraph`.

### Suggested starter files

Leer primero: `include/cybercba/missions/MissionSystem.hpp`,
`include/cybercba/game/CampaignState.hpp`, `include/cybercba/game/Game.hpp`,
VS-001 §4 y §16.
Crear: `include/cybercba/missions/MissionNode.hpp`,
`include/cybercba/missions/IUnlockCondition.hpp`,
`include/cybercba/missions/MissionMapSystem.hpp`, y sus `.cpp` en
`src/missions/`, más `tests/missions/MissionMapSystemTests.cpp`.

---

## Squad B — Guard Squad Alert Propagation (`combat/`, `missions/`)

### Feature y pitch

Hoy `alarmTriggered` es un único flag booleano global (VS-001 §3.2/§3.4):
si se dispara, todos los guardias del combate siguiente empiezan
"alertados" (`AlertedGuardStrategy`, +daño). No hay ningún concepto de que
la alerta se propague entre guardias individuales — es todo o nada. Squad
B construye un sistema de **propagación de alerta entre guardias**: un
guardia detectado avisa a los guardias cercanos, que a su vez avisan a los
suyos, con el nivel de alerta decayendo o transformando la estrategia de
combate de cada uno según qué tan lejos está de la fuente de la alarma.

### Por qué esta feature, por qué ahora

`GuardBehaviorStrategy` (`include/cybercba/combat/GuardBehaviorStrategy.hpp`)
ya tiene el patrón Strategy con `StandardGuardStrategy`/
`AlertedGuardStrategy` seleccionado una vez al iniciar combate (VS-001
§9.5.2) — pero la selección hoy es "todo el escuadrón igual". CHANGELOG
señala que el contenido narrativo real (P2/P4/M2/M3) todavía no está
autorado; cuando se autore, el diseño de VS-001 §3.2 ("alarma silenciosa")
va a necesitar más granularidad que un booleano si se quiere que sigilo
parcial tenga sentido. Esta feature construye esa granularidad ahora,
como sistema aislado y testeable, antes de que el contenido narrativo la
necesite.

### Concrete requirements

- Nuevo tipo `GuardSquadMember` (`include/cybercba/combat/GuardSquad.hpp`):
  un guardia con un nivel de alerta (`enum class AlertLevel { Idle, Alert,
  FullAlert }`) y punteros observadores a guardias "vecinos" (a quiénes
  puede alertar).
- Interfaz `IGuardBehaviorStrategy` ya existente se reutiliza; agregar una
  tercera implementación `EscalatingGuardStrategy` cuyo daño depende del
  `AlertLevel` del guardia (no solo Standard/Alerted binario).
- `GuardSquad` (posee el array de `GuardSquadMember`) con un método
  `propagateAlert(std::size_t sourceIndex, AlertLevel level)` que
  actualiza recursivamente el nivel de alerta de los vecinos, decayendo un
  nivel por cada salto (un guardia a 2 saltos del origen queda en `Alert`,
  no en `FullAlert`, salvo que ya estuviera más alerta por otra vía).
- `CombatSystem`/`MissionSystem` deben poder consultar, por guardia, qué
  `IGuardBehaviorStrategy` corresponde según su `AlertLevel` actual (sin
  romper el contrato existente de `CombatSystem::resolveAction`).

### Dónde aparecen OOP / polimorfismo / punteros / recursión

- **OOP:** `GuardSquad` tiene responsabilidad única (topología + estado de
  alerta del escuadrón), separada de `CombatSystem` (resolución de rondas,
  que sigue sin conocer la topología, solo qué estrategia usar por
  guardia).
- **Polimorfismo:** `EscalatingGuardStrategy` como tercera implementación
  de `IGuardBehaviorStrategy`, mismo contrato que `StandardGuardStrategy`/
  `AlertedGuardStrategy` (`decideAction(const CombatState&) const`).
- **Punteros:** cada `GuardSquadMember` tiene un array de punteros
  observadores a sus vecinos (`const GuardSquadMember* const* pNeighbors`)
  — el escuadrón entero vive dentro de `GuardSquad` (owner, mismo patrón
  que TAD-001 §12 con `Game`/`currentState_`); ningún miembro individual
  hace `new`/`delete`. Documenten explícitamente en el header quién
  construye `GuardSquad` y cuándo se destruye (sugerencia: por-combate,
  vive dentro del `CombatState` de esa escena o es pasado por
  `MissionSystem`).
- **Recursión:** `propagateAlert` es honestamente recursiva — es una
  propagación de onda sobre un grafo con decaimiento por profundidad,
  parada natural cuando el nivel a propagar ya no mejora el nivel actual
  del vecino (evita recorrer el mismo nodo infinitamente incluso con
  ciclos en la topología de vecinos). Documenten el caso base con
  claridad.

### Suggested task split (4 devs)

1. **Dev 1:** `GuardSquadMember` + `GuardSquad` (topología, ownership) +
   tests de construcción/consulta de vecinos.
2. **Dev 2:** `EscalatingGuardStrategy` + tests (incluye los ya existentes
   `StandardGuardStrategy`/`AlertedGuardStrategy` como referencia de
   estilo).
3. **Dev 3:** `propagateAlert` recursivo + tests (caso lineal, caso con
   ciclo en la topología, caso de nivel que no debe bajar).
4. **Dev 4:** integración con `CombatSystem`/`MissionSystem` (cómo se
   selecciona la estrategia por guardia según su `AlertLevel` sin romper
   `resolveAction`) + verificación manual.

### Acceptance criteria

- [ ] `GuardSquad::propagateAlert` tiene test para cadena lineal, para
      ciclo, y para "no downgrade" de un guardia ya más alerta.
- [ ] `EscalatingGuardStrategy` testeada igual que las estrategias
      existentes (mismo estilo que `tests/combat/`).
- [ ] Ownership de `GuardSquad`/`GuardSquadMember` documentado en el header
      (quién crea, quién destruye, quién solo observa).
- [ ] No rompe los 38 tests existentes.
- [ ] Sin STL, sin smart pointers, `clang-format` limpio.

### Suggested starter files

Leer primero: `include/cybercba/combat/GuardBehaviorStrategy.hpp`,
`src/combat/GuardBehaviorStrategy.cpp`,
`include/cybercba/combat/CombatSystem.hpp`,
`include/cybercba/combat/CombatState.hpp`, VS-001 §7 y §9.5.2.
Crear: `include/cybercba/combat/GuardSquad.hpp`,
`src/combat/GuardSquad.cpp`, extender
`include/cybercba/combat/GuardBehaviorStrategy.hpp`/`.cpp` con
`EscalatingGuardStrategy`, y `tests/combat/GuardSquadTests.cpp`.

---

## Squad C — Branching Dialogue Consequences (`missions/`)

### Feature y pitch

`DialogueSystem` hoy avanza una `DialogueScript` línea por línea, con
elecciones puntuales que saltan a `nextLineIndex` (un solo nivel de
ramificación por línea). VS-001 §3.3 ya describe una decisión con
sub-ramas condicionadas por el personaje jugado (Emma vs. Magga) y por
elecciones previas dentro de la misma escena (p. ej. la elección de
cobertura de Magga en P3 depende de que se haya elegido "Confiar en Emma"
antes). Squad C construye un evaluador de **condiciones narrativas
anidadas**: dado el árbol de posibles elecciones de una escena y el estado
acumulado de flags/decisiones previas, determina qué rama de diálogo
corresponde mostrar, sin si-else dispersos por todo el código de escena.

### Por qué esta feature, por qué ahora

CHANGELOG dice explícitamente que el contenido narrativo real de P1-P6/
M1-M4 todavía no está autorado — cuando se autore, alguien va a tener que
codificar la lógica de "esta línea depende de esta elección anterior, que
depende de otra". Sin un sistema para eso, esa lógica termina como
condicionales anidados dentro de `Game`/`MissionSystem`, exactamente lo
que TAD-001 §64 pide evitar. Esta feature construye el evaluador *antes*
de que el contenido narrativo real lo necesite, como sistema aislado y
testeable con datos de prueba.

### Concrete requirements

- Interfaz `INarrativeCondition` (`include/cybercba/missions/
  NarrativeCondition.hpp`) con `bool isSatisfied(const
  game::CampaignState&) const`. Al menos 2 implementaciones: p. ej.
  `FlagEqualsCondition` (compara un flag de `CampaignState` con un valor
  esperado) y `AllOfCondition`/`AnyOfCondition` (compone otras condiciones
  — este es el punto donde aparece la anidación real).
- Un `DialogueBranchNode`: una línea de diálogo candidata + su
  `INarrativeCondition` asociada + punteros a nodos hijos (sub-ramas que
  solo aplican si este nodo se activó).
- Un `NarrativeConsequenceResolver` (o nombre similar) que, dado un árbol
  de `DialogueBranchNode` y el `CampaignState` actual, devuelve la
  secuencia de líneas efectivamente aplicables (recorriendo el árbol,
  evaluando condiciones, sin evaluar sub-ramas de un nodo que no se
  activó).
- No reemplaza `DialogueSystem` (que sigue resolviendo la reproducción
  línea a línea de un `DialogueScript` ya lineal) — este sistema decide
  *qué* `DialogueScript`/líneas usar antes de dárselas a `DialogueSystem`,
  a partir del estado acumulado.

### Dónde aparecen OOP / polimorfismo / punteros / recursión

- **OOP:** `NarrativeConsequenceResolver` con responsabilidad única
  (resolver qué rama narrativa aplica), separado limpiamente de
  `DialogueSystem` (reproducción) y `MissionSystem` (orquestación de
  escenas).
- **Polimorfismo:** `INarrativeCondition` con `FlagEqualsCondition` /
  `AllOfCondition` (mismo espíritu Strategy que `IPersuasionStrategy`) —
  agregar un tipo de condición nuevo (p. ej. "requiere skill nivel II") no
  toca el resolver.
- **Punteros:** `DialogueBranchNode` referencia a sus hijos vía array de
  punteros observadores (`const DialogueBranchNode* const* pChildren`); el
  árbol completo vive en almacenamiento estático de la definición de
  escena (mismo patrón que `SceneDefinition`/`DialogueScript` hoy — dato
  inmutable, no se crea con `new` por sesión). Documenten explícitamente
  que el resolver nunca posee los nodos, solo los recorre.
- **Recursión:** evaluar el árbol de ramas es honestamente recursivo — un
  nodo activo habilita evaluar sus hijos, cada hijo puede tener sus
  propios hijos condicionados. Es exactamente el caso "evaluar árbol de
  diálogo/decisión anidado" mencionado como motivador del lenguaje del
  curso. Caso base: nodo sin hijos o nodo cuya condición no se satisface
  (no se desciende más).

### Suggested task split (4 devs)

1. **Dev 1:** `INarrativeCondition` + `FlagEqualsCondition` + tests.
2. **Dev 2:** `AllOfCondition`/`AnyOfCondition` (composición) +
   `DialogueBranchNode` + tests, define el contrato de ownership del árbol.
3. **Dev 3:** `NarrativeConsequenceResolver` recursivo + tests (árbol de
   3+ niveles, rama que no se activa no debe evaluarse — verificar con un
   condition-spy que cuenta llamadas).
4. **Dev 4:** integración de ejemplo con `MissionSystem`/`DialogueSystem`
   (un árbol de prueba pequeño que reproduzca el caso de P3 descrito en
   VS-001 §3.3) + verificación manual.

### Acceptance criteria

- [ ] `INarrativeCondition` y 2+ implementaciones (incluyendo al menos una
      composición: `AllOf`/`AnyOf`) con tests.
- [ ] `NarrativeConsequenceResolver` testeado sobre árbol de 3+ niveles;
      test explícito de que ramas no activadas no se evalúan.
- [ ] Ownership del árbol documentado (quién lo posee, resolver no
      destruye nada).
- [ ] Sin STL, sin smart pointers, `clang-format` limpio.
- [ ] No rompe tests existentes de `DialogueSystem`/`MissionSystem`.

### Suggested starter files

Leer primero: `include/cybercba/missions/DialogueSystem.hpp`,
`include/cybercba/missions/PersuasionStrategy.hpp`,
`include/cybercba/game/CampaignState.hpp`, VS-001 §3.3 y §9.5.
Crear: `include/cybercba/missions/NarrativeCondition.hpp`,
`include/cybercba/missions/DialogueBranchNode.hpp`,
`include/cybercba/missions/NarrativeConsequenceResolver.hpp` + sus `.cpp`
en `src/missions/`, y `tests/missions/NarrativeConsequenceResolverTests.cpp`.

---

## Squad D — Skill Prerequisites & Level III Effects (`progression/`)

### Feature y pitch

`SkillDefinition` ya modela 15 habilidades con descripciones de Nivel I,
II y III (`include/cybercba/progression/SkillDefinition.hpp`), pero
VS-001 §9 dice explícitamente que solo el paso Nivel I → Nivel II está
mecánicamente forzado en el slice, y marca el resto como
`TODO(VS-002)` (efecto mecánico de Nivel III, política TAD-001 §61). Squad
D construye la lógica de progresión que falta: qué efecto mecánico aplica
cada nivel de una skill, y — para las skills que lo tengan sentido
narrativo (p. ej. una skill exclusiva que depende de tener otra en Nivel
II antes) — la validación de **prerrequisitos entre skills**.

### Por qué esta feature, por qué ahora

Es un TODO explícito ya escrito en la spec (VS-001 §9), no una feature
inventada: "el resto puede existir como datos seleccionables sin efecto
mecánico adicional todavía (marcar con TODO(VS-002))". `ProgressionSystem`
hoy (`include/cybercba/progression/ProgressionSystem.hpp`) sabe otorgar
puntos y subir de nivel una skill, pero no sabe *qué hace* cada nivel más
allá de las 2 skills con lógica ya conectada (Persuasión/Intimidación en
M1, Percepción en P3 rama Magga, según VS-001 §9). Esta feature generaliza
eso a un sistema real en vez de casos especiales cableados a mano.

### Concrete requirements

- Interfaz `ISkillEffect` (`include/cybercba/progression/
  SkillEffect.hpp`): `void applyTo(game::CampaignState&) const` (o el
  punto de aplicación que decidan, documentado). Al menos 2
  implementaciones concretas para 2 skills distintas del catálogo
  existente (p. ej. un efecto de resistencia extra en combate para una
  skill de Magga, un efecto de recurso extra para una skill de Emma).
- Extender el modelo de skill con una noción de **prerrequisito**: una
  skill puede requerir que otra skill (o la misma, en un nivel anterior)
  esté en cierto nivel antes de poder subir de nivel. No todas las skills
  tienen prerrequisitos — solo agréguenlos donde tenga sentido narrativo
  con las 15 skills ya definidas.
- `ProgressionSystem::canLevelUp(skillId)` (o nombre similar) debe validar
  la cadena de prerrequisitos completa antes de permitir subir de nivel —
  si la skill requerida a su vez tiene su propio prerrequisito no
  satisfecho, la validación debe fallar (no solo mirar un nivel de
  distancia).
- No se cablea contenido narrativo nuevo de misiones — el catálogo de las
  15 skills ya existe (`SkillDefinition`); esta feature es la lógica que
  falta sobre datos que ya están.

### Dónde aparecen OOP / polimorfismo / punteros / recursión

- **OOP:** `ISkillEffect`/`ProgressionSystem` mantienen la separación ya
  existente entre catálogo de datos (`SkillDefinition`, inmutable) y
  sistema con comportamiento (`ProgressionSystem`) — el efecto mecánico de
  cada skill vive en su propia clase con una sola responsabilidad, no
  dentro de un switch gigante en `ProgressionSystem`.
- **Polimorfismo:** `ISkillEffect` con 2+ implementaciones concretas,
  mismo espíritu que `IPersuasionStrategy`/`IGuardBehaviorStrategy` —
  agregar el efecto de una skill nueva no toca `ProgressionSystem`.
- **Punteros:** cada definición de prerrequisito referencia a otra skill
  vía un puntero observador (p. ej. `const SkillDefinition*
  pRequiredSkill`, `nullptr` si no tiene prerrequisito) — las
  `SkillDefinition` siguen siendo dueñas de sí mismas en el catálogo
  estático existente (`skillDefinition(SkillId)`); el prerrequisito nunca
  posee ni destruye la skill que referencia. Documenten esto explícito en
  el header nuevo.
- **Recursión:** `canLevelUp` debe seguir la cadena de prerrequisitos
  recursivamente (la skill X requiere Y en nivel II, que a su vez podría
  requerir Z) — es honestamente recursivo por la misma razón que en Squad
  A/B: es un recorrido de dependencias, y hay que decidir y testear qué
  pasa ante un ciclo de prerrequisitos (debería ser imposible de crear
  con datos válidos, pero el código debe no colgarse si ocurre por error
  de datos).

### Suggested task split (4 devs)

1. **Dev 1:** `ISkillEffect` + primera implementación concreta + tests.
2. **Dev 2:** segunda implementación concreta de `ISkillEffect` + el
   modelo de prerrequisito sobre `SkillDefinition`/`SkillId` + tests.
3. **Dev 3:** `ProgressionSystem::canLevelUp` recursivo + tests (cadena de
   2+ niveles de prerrequisito, caso de ciclo).
4. **Dev 4:** integración con el flujo de mejora de habilidad existente en
   `ProgressionSystem`/`Game` (pantalla de mejora post-misión, VS-001 §9)
   + verificación manual.

### Acceptance criteria

- [ ] `ISkillEffect` con 2+ implementaciones, testeadas.
- [ ] `canLevelUp` testeado con cadena de prerrequisitos de 2+ niveles y
      con un caso de ciclo que no debe colgar el programa.
- [ ] Ownership de las referencias entre `SkillDefinition` documentado.
- [ ] No rompe los tests existentes de `ProgressionSystem`.
- [ ] Sin STL, sin smart pointers, `clang-format` limpio.

### Suggested starter files

Leer primero: `include/cybercba/progression/SkillDefinition.hpp`,
`include/cybercba/progression/ProgressionSystem.hpp`,
`include/cybercba/progression/SkillId.hpp`, VS-001 §9.
Crear: `include/cybercba/progression/SkillEffect.hpp`,
extender `include/cybercba/progression/SkillDefinition.hpp` (o un nuevo
`SkillPrerequisite.hpp`) con el modelo de prerrequisito, y
`tests/progression/SkillPrerequisiteTests.cpp` /
`tests/progression/SkillEffectTests.cpp`.

---

## Expectativas comunes a las 4 escuadras

- **Ramas:** no se encontró un job de CI (`validate-branch-name` u
  similar) en `.github/` que fije una convención de nombres de rama en
  este repo — si el equipo adopta una, documéntenla acá. Mientras tanto,
  usen algo descriptivo y consistente, p. ej.
  `feature/<squad>-<nombre-corto>` (ej. `feature/squad-a-mission-unlock`).
- **Issues:** abran una issue por feature usando las plantillas de
  `.github/ISSUE_TEMPLATE/` (`feature_request.yml` para la feature en sí;
  `bug_report.yml` si encuentran algo roto en el código existente
  mientras trabajan) antes de empezar a codear, y linkeen la PR a esa
  issue (`Closes #N`) como pide el PR template.
- **PRs:** usen `.github/PULL_REQUEST_TEMPLATE.md` completo — tipo de
  cambio, issue relacionada, resumen, y el checklist de pre-review
  (`AGENTS.md` leído, `cmake --build build/dev` compila, `ctest` en
  verde, `clang-format -i` corrido, aprobación de Tester y Team Lead).
- **Bloqueos:** si algo es ambiguo entre lo que dice VS-001/GPD y lo que
  hace falta para la feature, no inventen canon (TAD-001 §71) — pregunten
  en Discussions o, si el bloqueo es menor, implementen la solución
  mínima reversible y díganlo explícitamente en la PR.
- **Alcance:** ninguna de las 4 features requiere tocar
  `include/cybercba/structures/` ni `PuzzleSystem`. Si su implementación
  empieza a necesitar eso, paren y revisen — probablemente se desviaron
  del scope descrito arriba.
- **Build/test loop:** el mismo de siempre (`AGENTS.md`):
  ```bash
  export VCPKG_ROOT=~/vcpkg
  cmake --preset dev
  cmake --build build/dev
  ctest --test-dir build/dev --output-on-failure
  ```
