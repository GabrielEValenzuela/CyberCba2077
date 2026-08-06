# Templates de pantalla de misión

Documenta el estado real de los templates de pantalla del RPG narrativo (ver `docs/campaign-bible.md` para la dirección de producto) y qué componentes de `src/ui/widgets/` los implementan hoy. No es un catálogo aspiracional: cada fila dice explícitamente si el template está implementado con un componente reutilizable, con código inline en `GameApp`, o si todavía no existe.

| Template | Estructura | Componente | Estado real |
|---|---|---|---|
| 6.1 Story/prólogo | Label de capítulo + retrato/evidencia + texto | `StoryPanel` (`src/ui/widgets/StoryPanel.{hpp,cpp}`) | **Implementado y wireado**: `GameApp::drawIntro()` |
| 6.2 Selección de acción | Imagen + lista de acciones | `ActionMenu` (`src/ui/widgets/ActionMenu.{hpp,cpp}`) | **Implementado y wireado**: `GameApp::drawButton` delega en `ActionMenu::drawItem`; la rama genérica de nodo en `drawWorld()` usa `ActionMenu::draw()` para la lista completa |
| 6.3 Diálogo con elección | Hablante + texto + fila de respuestas | `ChoicePanel` (`src/ui/widgets/ChoicePanel.{hpp,cpp}`) | **Implementado y wireado**: nodo `narrative_choice` de `GameApp::drawWorld()` (único punto de elección narrativa del prólogo actual) |
| 7.1 Mapa de nodos de misión | Nodos descubiertos/conectados/bloqueados/completados | `NodeMapView` (`src/ui/widgets/NodeMapView.{hpp,cpp}`) | **Implementado y wireado**: `GameApp::nodeMapEntries()` construye las entradas desde `MissionGraph::nodes()`/`isCompleted()`/`availableTransitions()` (real, no decorativo); dibujado como franja horizontal sobre `drawWorld()`. La lista de texto navegable (`nodeOptions()`) sigue siendo el mecanismo real de navegación/input — el mapa es visualización, no reemplaza la selección por teclado/gamepad |
| 7.2 Encuentro por turnos | Fondo + figuras + orden de turno + intención enemiga + menú de acción | `TurnOrderPanel`, `ParticipantPanel` (+ `StatusPanel`), `EnemyIntentPanel`, `ActionMenu` | **Implementado y wireado** en `GameApp::drawEncounter()`: orden de turno real (`Encounter::turnOrder()`), tarjetas de participante, intención enemiga determinista, fondo de hacking de Emma y dron RGBA para el encuentro de Magga. |
| 7.3 Journal de evidencia | Icono + título + descripción + origen | `EvidencePanel` (`src/ui/widgets/EvidencePanel.{hpp,cpp}`) | **Parcial**: `GameApp::drawObjectives()` usa `EvidencePanel` para el thumbnail+título; descripción/origen/contradicciones siguen sin mostrarse (el texto completo de cada `EvidenceItem` no se renderiza en el chip, solo el título) |
| 7.4 Resultados de misión | Título + objetivos completados + evidencia + decisiones + teaser | `MissionResultsView` (`src/ui/widgets/MissionResultsView.{hpp,cpp}`) | **Implementado y wireado**: `GameApp::drawEnd()` usa `MissionResultsView::draw()` (título + texto de decisiones/checkpoint/teaser + tira de `EvidencePanel`), reemplazando el layout inline anterior |
| — Transmisión fragmentada | Barra de calidad de señal + ruido procedural + texto | `TransmissionPanel` (`src/ui/widgets/TransmissionPanel.{hpp,cpp}`) | **Implementado y wireado**: nodo `fragmented_transmission` de `GameApp::drawWorld()` y retrato de transmisión dedicado en intro/modal. `signalQuality` es un valor ilustrativo fijo (0.35) — no hay todavía un valor de intensidad de señal trackeado por el dominio |
| — Diálogo sin elección | Hablante + texto + hint de continuar | `DialoguePanel` (`src/ui/widgets/DialoguePanel.{hpp,cpp}`) | **Implementado y wireado**: `Modal::Dialogue` en `GameApp::drawModal()` |
| PoC diálogo (ADR 0017) | Barra de misión + objetivo arriba, escena grande, retrato busto + texto abajo | `NodeMapView` + `CurrentObjectivePanel` + `DialoguePanel`/`PortraitFrame` | **Piloto integrado**: `PortraitFrame` usa los retratos 4:5 RGBA neutrales del runtime pack, con crop inferior de busto; ya no recorta el sprite pixel-art de cuerpo completo. |
| PoC interacción (ADR 0017) | Escena + objeto resaltado + retrato + reacción + menú de acciones | `InteractionPanel` (`src/ui/widgets/InteractionPanel.{hpp,cpp}`) + corchetes de esquina | **Integrado**: `GameApp::drawWorld()` usa fondo estático de refugio/Neometro y selecciona retrato por nodo (análisis, daño, hacking, decisión, tensión exterior). La foto del refugio usa el hotspot normalizado declarado por el pack; el resto de hotspots específicos necesita assets de escena propios. |

## Componentes compartidos

- `NlmPanel`/`NlmButton`/`NlmText` (`src/ui/widgets/`) — widgets genéricos pre-pivot. `NlmPanel::draw()` es reutilizado por `EvidencePanel`/`StoryPanel`/`ChoicePanel`/`ActionMenu` para el borde/fondo; `NlmButton` (el widget completo, con su propio manejo de mouse) sigue sin usarse — `ActionMenu::drawItem` es el reemplazo real que sí pasa por `virtualMouse()`/el canvas escalado (ver ADR `0016`). `NlmText` sigue sin uso.
- `TextWrap` (`src/ui/widgets/TextWrap.{hpp,cpp}`) — wrap de texto compartido, extraído de `GameApp.cpp` para que los widgets nuevos midan contra la misma fuente real (`DejaVuSansMono`, ADR `0015`) que usa `GameApp::drawText`.
- `ActionMenu` (`src/ui/widgets/ActionMenu.{hpp,cpp}`) — ver fila 6.2. También usado como base de `GameApp::drawButton` (menú principal, pausa, tabs de configuración), no solo en el mapa de nodos.
- `NodeMapView` (`src/ui/widgets/NodeMapView.{hpp,cpp}`) — ver fila 7.1. Requirió agregar `MissionGraph::nodes()` (`include/cybercba/MissionGraph.hpp`) para poder enumerar el grafo completo; cubierto por `tests/test_mission_graph.cpp::NodesExposesTheFullRegisteredGraphForMapRendering`.
- `TurnOrderPanel`, `ParticipantPanel`, `StatusPanel`, `EnemyIntentPanel` (`src/ui/widgets/`) — ver fila 7.2. `ParticipantPanel` depende directamente de `cybercba::EncounterParticipant`/`StatusEffect` (a diferencia de los demás widgets, que reciben solo strings) porque son DTOs de solo lectura del dominio — no rompe la regla de "dominio sin raylib" (la dependencia va en un solo sentido: UI → dominio).
- `MissionResultsView` — ver fila 7.4. Compone `EvidencePanel` internamente en vez de duplicar su render.
- `TransmissionPanel`, `DialoguePanel` — ver filas "Transmisión fragmentada"/"Diálogo sin elección".
- `UiMetrics` (`src/ui/widgets/UiMetrics.hpp`) — constantes de layout compartidas (ADR 0017): reemplaza cajas ad hoc de 970×525 dentro de un canvas de 1280×720 por un layout que ocupa casi todo el canvas (`contentBounds()`/`sceneBounds()`/`bottomPanelBounds()`).
- `CurrentObjectivePanel` (`src/ui/widgets/CurrentObjectivePanel.{hpp,cpp}`) — objetivo actual arriba a la derecha, junto a `NodeMapView`, formando la franja superior compartida que pide el PoC.
- `PortraitFrame` (`src/ui/widgets/PortraitFrame.{hpp,cpp}`) — marco de retrato busto; ver limitación de placeholder en la fila "PoC diálogo".
- `InteractionPanel` (`src/ui/widgets/InteractionPanel.{hpp,cpp}`) — ver fila "PoC interacción". Compone `PortraitFrame` + `ActionMenu`.

Con esto, todos los templates listados en la sección 6/7 del pedido original tienen al menos una implementación real wireada; los gaps restantes son de **contenido** (más nodos de diálogo simple, más beats de transmisión) y de **arte** (assets estáticos reales), no de arquitectura de componentes.

## Pendiente (no implementado, no simulado como hecho)

- Las categorías `icon`, `uiFrame` y `effect` siguen sin assets reales (`docs/static-art-direction.md`) — los componentes renderizan texto/HUD cuando no hay imagen, no un placeholder falso.
- Descripción/origen/contradicciones de cada evidencia (7.3) no se muestran, solo título.
- `signalQuality` de `TransmissionPanel` es un valor fijo ilustrativo (0.35), no un estado real del dominio.
- **`EncounterScreen` completo** (sección 10 del pedido): `drawEncounter()` no fue migrado a `UiMetrics`/header compartido en esta sesión — sigue con su propio layout de panel `{120,80,1040,560}`.
- **Verbos de acción específicos** (`EXAMINAR`/`GUARDAR EVIDENCIA`/`VOLVER`): los nodos de investigación siguen usando las transiciones de sala existentes como "acciones"; no se agregó el mecanismo de acciones data-driven con costo/condición/consecuencia de la sección 8 del pedido.
- **Manifest de assets extendido** (sección 14 del pedido: `portraitCrop`, `speaker`, `expression`, `safeArea`, etc.) no fue agregado al esquema — sigue en `schemaVersion` 6.
- Tests nuevos para esta pantalla (selección de portrait, exposición de acciones, fallback de asset faltante — sección 17) no fueron escritos.
- Verificación en múltiples resoluciones (1280×720/1600×900/1920×1080/ultrawide/fullscreen) no fue realizada — sin sesión gráfica confiable en este entorno para probarlo.
