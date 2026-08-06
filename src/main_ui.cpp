#include "config/GameConfig.hpp"

#include <string>

#include "raylib.h"
#include "app/GameApp.hpp"

int main(int argc, char* argv[])
{
    const char* configPath = CYBER_CBA_DEFAULT_CONFIG_PATH;
    if (argc == 3 && std::string(argv[1]) == "--config")
    {
        configPath = argv[2];
    }
    GameConfig config;
    config.loadFromFile(configPath);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(config.screenWidth(), config.screenHeight(), config.windowTitle().c_str());
    SetWindowMinSize(960, 540);
    SetTargetFPS(config.targetFps());

    GameApp app(config);
    while (!WindowShouldClose() && !app.shouldExit())
    {
        const float dt = GetFrameTime();
        app.update(dt);
        app.draw();
    }
    CloseWindow();
    return 0;
}
