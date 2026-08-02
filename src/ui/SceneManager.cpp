#include "ui/SceneManager.hpp"

#include <memory>

#include "config/GameConfig.hpp"
#include "ui/IScene.hpp"
#include "ui/scenes/HUDScene.hpp"
#include "ui/scenes/InventoryScene.hpp"
#include "ui/scenes/MainMenuScene.hpp"
#include "ui/scenes/MapScene.hpp"
#include "ui/scenes/SplashScene.hpp"
#include "ui/widgets/DevOverlay.hpp"

namespace
{
    constexpr int SPLASH_SCENE_ID = 0;
    constexpr int MAIN_MENU_SCENE_ID = 1;
    constexpr int MAP_SCENE_ID = 2;
    constexpr int INVENTORY_SCENE_ID = 4;
    constexpr int EXIT_SCENE_ID = -1;
} // namespace

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() = default;

void SceneManager::init(const GameConfig& config)
{
    m_model.addCredits(50);
    m_splashScene = std::make_unique<SplashScene>(config.splashSeconds());
    m_mainMenuScene = std::make_unique<MainMenuScene>();
    m_mapScene = std::make_unique<MapScene>();
    m_inventoryScene = std::make_unique<InventoryScene>(m_model);
    m_hudScene = std::make_unique<HUDScene>(m_model);
    if (config.isDevelopmentMode())
    {
        m_devOverlay = std::make_unique<DevOverlay>();
    }
    changeScene(SPLASH_SCENE_ID);
}

void SceneManager::shutdown()
{
    m_pCurrentScene = nullptr;
    m_devOverlay.reset();
    m_hudScene.reset();
    m_inventoryScene.reset();
    m_mapScene.reset();
    m_mainMenuScene.reset();
    m_splashScene.reset();
}

void SceneManager::update(float dt)
{
    if (m_pCurrentScene == nullptr)
    {
        return;
    }
    m_pCurrentScene->update(dt);
    if (m_currentSceneId == MAP_SCENE_ID)
    {
        m_hudScene->update(dt);
    }
    if (m_devOverlay != nullptr)
    {
        m_devOverlay->update(dt);
    }
    if (m_pCurrentScene->shouldTransition())
    {
        changeScene(m_pCurrentScene->nextScene());
    }
}

void SceneManager::draw() const
{
    if (m_pCurrentScene == nullptr)
    {
        return;
    }
    m_pCurrentScene->draw();
    if (m_currentSceneId == MAP_SCENE_ID)
    {
        m_hudScene->draw();
    }
    if (m_devOverlay != nullptr)
    {
        m_devOverlay->draw();
    }
}

bool SceneManager::shouldExit() const
{
    return m_shouldExit;
}

void SceneManager::changeScene(int sceneId)
{
    m_currentSceneId = sceneId;
    switch (sceneId)
    {
        case SPLASH_SCENE_ID: m_pCurrentScene = m_splashScene.get(); break;
        case MAIN_MENU_SCENE_ID: m_pCurrentScene = m_mainMenuScene.get(); break;
        case MAP_SCENE_ID: m_pCurrentScene = m_mapScene.get(); break;
        case INVENTORY_SCENE_ID: m_pCurrentScene = m_inventoryScene.get(); break;
        case EXIT_SCENE_ID: m_shouldExit = true; break;
        default:
            m_pCurrentScene = m_mainMenuScene.get();
            m_currentSceneId = MAIN_MENU_SCENE_ID;
            break;
    }
}
