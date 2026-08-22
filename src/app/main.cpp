#include <raylib.h>

#include "cybercba/game/Game.hpp"
#include "cybercba/platform/AssetStore.hpp"
#include "cybercba/platform/AudioAdapter.hpp"
#include "cybercba/platform/InputAdapter.hpp"
#include "cybercba/platform/Renderer.hpp"

namespace
{

constexpr int WINDOW_WIDTH     = 1280;
constexpr int WINDOW_HEIGHT    = 720;
constexpr int TARGET_FPS       = 60;
const char* const WINDOW_TITLE = "CyberCBA 2077";

} // namespace

// Composition root: wires cybercba_core and cybercba_raylib together and runs
// the game loop. Kept intentionally small — gameplay
// logic lives in Game::update, presentation in Renderer::draw.
int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    const cybercba::platform::AudioAdapter audio;
    const cybercba::platform::AssetStore assets;
    const cybercba::platform::InputAdapter inputAdapter;
    const cybercba::platform::Renderer renderer(assets);
    cybercba::game::Game game;

    while (!WindowShouldClose())
    {
        const cybercba::states::InputSnapshot input = inputAdapter.capture();
        game.update(input);

        BeginDrawing();
        renderer.draw(game);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
