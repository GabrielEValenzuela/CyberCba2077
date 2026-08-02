#pragma once

#include <memory>
#include <string>

#include "cybercba/GameSession.hpp"
#include "cybercba/SaveService.hpp"
#include "cybercba/WorldLayout.hpp"
#include "raylib.h"
#include "ui/InputRouter.hpp"

class GameConfig;
class DevOverlay;
class AudioService;
class AssetStore;

class GameApp
{
public:
    explicit GameApp(const GameConfig& config);
    ~GameApp();
    void update(float dt);
    void draw();
    bool shouldExit() const;

private:
    enum class Screen
    {
        Splash,
        Menu,
        CharacterSelection,
        Intro,
        World,
        Chapters,
        Settings,
        Credits,
        Pause,
        Objectives,
        Defeat,
        End
    };
    enum class Modal
    {
        None,
        ConfirmNewGame,
        ConfirmExit,
        ConfirmRestart,
        ConfirmMenu,
        ConfirmCharacter,
        Hacking,
        Dialogue
    };
    void setScreen(Screen screen);
    void save();
    void start(cybercba::CharacterId character);
    void interact();
    void resetCheckpoint();
    void updateMenu();
    void updateSettings();
    void updatePause();
    void updateModal();
    void persistSettings();
    void drawBackground() const;
    void drawMenu() const;
    void drawSelection() const;
    void drawIntro() const;
    void drawWorld() const;
    void drawShelterShell() const;
    void drawExteriorShell() const;
    void drawWorldProps(cybercba::WorldZone zone, cybercba::WorldLayer layer) const;
    void drawWorldDebug(cybercba::WorldZone zone) const;
    void drawWorldLighting(cybercba::WorldZone zone) const;
    void drawWeather(cybercba::WorldZone zone) const;
    void drawWorldSignage(cybercba::WorldZone zone) const;
    void drawHud() const;
    void drawObjectives() const;
    void drawSettings() const;
    void drawCredits() const;
    void drawPause() const;
    void drawEnd() const;
    void drawModal() const;
    void drawChapters() const;
    void drawOverlay(const std::string& title, const std::string& body, const std::string& hint) const;
    void drawButton(const char* label, int x, int y, int width, bool selected, bool disabled = false) const;
    Vector2 virtualMouse() const;
    const char* devicePrompt(const char* keyboard, const char* gamepad) const;
    void drawText(const std::string& text, int x, int y, int size, Color color) const;
    std::string prompt() const;
    const cybercba::WorldProp* nearbyWorldProp() const;

    const GameConfig& m_config;
    cybercba::GameSession m_session;
    cybercba::SaveService m_save;
    InputRouter m_input;
    std::unique_ptr<DevOverlay> m_overlay;
    std::unique_ptr<AudioService> m_audio;
    std::unique_ptr<AssetStore> m_assets;
    RenderTexture2D m_virtual {};
    Screen m_screen {Screen::Splash};
    int m_selected {0};
    int m_settingsTab {0};
    int m_settingsRow {0};
    float m_splashElapsed {0.0F};
    float m_toastSeconds {0.0F};
    float m_hackProgress {0.0F};
    float m_powerSequenceSeconds {0.0F};
    Vector2 m_playerPosition {300.0F, 470.0F};
    bool m_overlayVisible {false};
    bool m_confirmOverwrite {false};
    bool m_exitUnlocked {false};
    Modal m_modal {Modal::None};
    bool m_exit {false};
    std::string m_message;
};
