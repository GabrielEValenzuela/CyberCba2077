#include "cybercba/platform/Renderer.hpp"

#include <raylib.h>

namespace cybercba::platform
{

namespace
{

constexpr int TITLE_FONT_SIZE = 32;
constexpr int LABEL_FONT_SIZE = 20;
constexpr int MARGIN          = 40;

const char* stateLabel(game::GameStateId state)
{
    switch (state)
    {
    case game::GameStateId::MainMenu:
        return "MAIN MENU";
    case game::GameStateId::CharacterSelection:
        return "CHARACTER SELECTION";
    case game::GameStateId::Prologue:
        return "PROLOGUE";
    case game::GameStateId::CentralMap:
        return "CENTRAL MAP";
    case game::GameStateId::Mission:
        return "MISSION";
    case game::GameStateId::Puzzle:
        return "PUZZLE";
    case game::GameStateId::Combat:
        return "COMBAT";
    case game::GameStateId::PostMission:
        return "POST-MISSION";
    case game::GameStateId::Pause:
        return "PAUSED";
    case game::GameStateId::Ending:
        return "ENDING";
    case game::GameStateId::Credits:
        return "CREDITS";
    }
    return "UNKNOWN";
}

constexpr int CREDITS_FONT_SIZE = 18;
constexpr int CREDITS_TOP       = MARGIN + TITLE_FONT_SIZE + LABEL_FONT_SIZE + 30;

void drawCredits(const game::Game& gameState)
{
    const std::string& text = gameState.creditsText();
    if (text.empty())
    {
        DrawText("(sin entradas en credits/ todavía — ver credits/README.md)", MARGIN, CREDITS_TOP, LABEL_FONT_SIZE,
                 GRAY);
        return;
    }
    DrawText(text.c_str(), MARGIN, CREDITS_TOP, CREDITS_FONT_SIZE, RAYWHITE);
}

} // namespace

Renderer::Renderer(const AssetStore& assets) : m_pAssets(&assets) {}

void Renderer::draw(const game::Game& gameState) const
{
    ClearBackground(BLACK);
    DrawText("CYBERCBA 2077", MARGIN, MARGIN, TITLE_FONT_SIZE, RAYWHITE);
    DrawText(stateLabel(gameState.currentState()), MARGIN, MARGIN + TITLE_FONT_SIZE + 10, LABEL_FONT_SIZE, GRAY);

    if (gameState.currentState() == game::GameStateId::Credits)
    {
        drawCredits(gameState);
        return;
    }

    DrawTexture(m_pAssets->texture(TextureId::RefugeBackground), MARGIN, MARGIN + 80, WHITE);
}

} // namespace cybercba::platform
