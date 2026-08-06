#ifndef NLM_SCENE_MANAGER_HPP
#define NLM_SCENE_MANAGER_HPP

/**
 * @file SceneManager.hpp
 * @brief Controla el ciclo de vida y las transiciones de las escenas.
 * @details Es el controlador de UI: conserva el modelo, actualiza la escena activa y superpone el HUD en el mapa.
 * @invariant Solo una escena principal está activa a la vez; el HUD solo se dibuja sobre el mapa.
 */
#include <memory>

#include "model/GameModel.hpp"

class GameConfig;
class DevOverlay;
class IScene;
class HUDScene;

class SceneManager
{
public:
    /** @brief Construye un administrador sin escenas registradas. */
    SceneManager();
    /** @brief Destruye las escenas después de que sus tipos estén completos. */
    ~SceneManager();
    /** @brief Registra las escenas y prepara el modelo del MVP. @param config Configuración externa del juego. @post
     * Queda activa la escena splash. */
    void init(const GameConfig& config);
    /** @brief Libera escenas y recursos de UI. @post No quedan escenas registradas. */
    void shutdown();
    /** @brief Actualiza la escena activa y procesa transiciones. @param dt Delta time en segundos. */
    void update(float dt);
    /** @brief Dibuja la escena activa y el HUD cuando corresponde. @pre Debe llamarse dentro de
     * BeginDrawing/EndDrawing. */
    void draw() const;
    /** @brief Indica si se solicitó cerrar la aplicación. @return `true` si el menú eligió salir. */
    bool shouldExit() const;

private:
    void changeScene(int sceneId);

    cyberpunk::GameModel m_model;
    std::unique_ptr<IScene> m_splashScene;
    std::unique_ptr<IScene> m_mainMenuScene;
    std::unique_ptr<IScene> m_mapScene;
    std::unique_ptr<IScene> m_inventoryScene;
    std::unique_ptr<HUDScene> m_hudScene;
    std::unique_ptr<DevOverlay> m_devOverlay;
    IScene* m_pCurrentScene {nullptr};
    int m_currentSceneId {0};
    bool m_shouldExit {false};
};

#endif // NLM_SCENE_MANAGER_HPP
