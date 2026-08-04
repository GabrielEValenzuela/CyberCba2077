#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cybercba/Encounter.hpp"
#include "cybercba/GameSession.hpp"
#include "cybercba/SaveService.hpp"
#include "cybercba/WorldLayout.hpp"
#include "raylib.h"
#include "ui/InputRouter.hpp"
#include "ui/widgets/NodeMapView.hpp"

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
        End,
        Encounter
    };
    enum class Modal
    {
        None,
        ConfirmNewGame,
        ConfirmExit,
        ConfirmRestart,
        ConfirmMenu,
        ConfirmCharacter,
        Dialogue
    };
    void setScreen(Screen screen);
    void save();
    void start(cybercba::CharacterId character);
    void resetCheckpoint();
    cybercba::WorldZone nodeZone(const std::string& id) const;
    std::string nodeTitle(const std::string& id) const;
    std::string nodeBody(const std::string& id) const;
    struct NodeOption { std::string label; std::string action; };
    std::vector<NodeOption> nodeOptions() const;
    std::vector<NodeMapView::Entry> nodeMapEntries() const;
    const cybercba::WorldProp* interactionHighlight(const std::string& nodeId) const;
    void enterNode(const std::string& id);
    void selectNodeOption(const std::string& action);
    void updateWorldNode();
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
    void drawWorldLighting(cybercba::WorldZone zone) const;
    void drawWeather(cybercba::WorldZone zone) const;
    void drawWorldSignage(cybercba::WorldZone zone) const;
    void drawObjectives() const;
    std::string evidenceImagePath(const std::string& evidenceId) const;
    void drawSettings() const;
    void drawCredits() const;
    void drawPause() const;
    void drawEnd() const;
    void drawModal() const;
    void drawChapters() const;
    void drawEncounter() const;
    void updateEncounter();
    std::string nextEnemyActionId() const;
    void startEncounter(cybercba::Encounter encounter, cybercba::CharacterId protagonist);
    void resolveEncounterOutcome();
    void drawOverlay(const std::string& title, const std::string& body, const std::string& hint) const;
    void drawButton(const char* label, int x, int y, int width, bool selected, bool disabled = false) const;
    Vector2 virtualMouse() const;
    const char* devicePrompt(const char* keyboard, const char* gamepad) const;
    void drawText(const std::string& text, int x, int y, int size, Color color) const;

    const GameConfig& m_config;
    cybercba::GameSession m_session;
    cybercba::SaveService m_save;
    InputRouter m_input;
    std::unique_ptr<DevOverlay> m_overlay;
    std::unique_ptr<AudioService> m_audio;
    std::unique_ptr<AssetStore> m_assets;
    const Font* m_uiFont {nullptr};
    RenderTexture2D m_virtual {};
    Screen m_screen {Screen::Splash};
    int m_selected {0};
    int m_settingsTab {0};
    int m_settingsRow {0};
    float m_splashElapsed {0.0F};
    float m_toastSeconds {0.0F};
    float m_powerSequenceSeconds {0.0F};
    bool m_overlayVisible {false};
    bool m_confirmOverwrite {false};
    Modal m_modal {Modal::None};
    bool m_exit {false};
    std::string m_message;
    std::string m_creditsEntries;
    std::unique_ptr<cybercba::Encounter> m_encounter;
    cybercba::CharacterId m_encounterProtagonist {cybercba::CharacterId::None};
    int m_encounterAction {0};
};
