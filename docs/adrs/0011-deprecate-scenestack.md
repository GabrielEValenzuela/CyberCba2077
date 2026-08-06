# ADR 0011: Deprecación de SceneStack/IScene/SceneManager

Auditoría confirmada en código: `src/ui/SceneStack.{hpp,cpp}`, `src/ui/IScene.hpp` y las escenas concretas en `src/ui/scenes/*` están compiladas dentro de `cyber_cba`, pero ningún símbolo (`SceneStack`, `IScene`, `Splash/MainMenu/Map/HUD/InventoryScene`) es referenciado por `GameApp.hpp`/`GameApp.cpp` ni por `main_ui.cpp`. `src/ui/SceneManager.cpp` ni siquiera está en `CMakeLists.txt`. El flujo real de pantallas es un enum `Screen` despachado por un if/else dentro de `GameApp`.

Se marca explícitamente `SceneStack`/`IScene`/`SceneManager`/`src/ui/scenes/*` como **Deprecado** (no implementado en la práctica, sin contrato real que preservar). No se borra el código en esta fase — se preserva por si se decide una futura migración a un patrón de pila de escenas real — pero no se integra ni se referencia como el mecanismo de transición de pantallas.

Las pantallas nuevas requeridas por el pivot narrativo (mapa de nodos, escena narrativa, investigación, diario de evidencia, encuentro) se agregan (Phase 2/3) siguiendo el patrón que `GameApp` ya usa hoy: un valor nuevo de `Screen` + un método privado `draw*()`/`update*()`, ahora renderizando datos de `MissionGraph`/`DialogueGraph`/`EvidenceJournal`/`Encounter` en vez de lógica hardcodeada por pantalla.
