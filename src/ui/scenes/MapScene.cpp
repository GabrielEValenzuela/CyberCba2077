#include "ui/scenes/MapScene.hpp"

#include "raylib.h"
#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/NlmText.hpp"

namespace
{
    constexpr int MAIN_MENU_SCENE_ID = 1;
    constexpr int INVENTORY_SCENE_ID = 4;
} // namespace

void MapScene::update(float dt)
{
    (void)dt;
    m_shouldTransition = false;
    if (IsKeyPressed(KEY_I))
    {
        m_shouldTransition = true;
        m_nextScene = INVENTORY_SCENE_ID;
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        m_shouldTransition = true;
        m_nextScene = MAIN_MENU_SCENE_ID;
    }
}

void MapScene::draw() const
{
    NlmText::draw("MAPA // VISTA PREVIEW", 48, 90, 28, NlmColors::TEXT);
    NlmText::draw("El grafo del modelo se conectará aquí", 48, 128, 18, NlmColors::TEXT_DIM);
    const Vector2 nodes[] {{240.0F, 360.0F}, {450.0F, 260.0F}, {650.0F, 400.0F}, {880.0F, 255.0F}, {1040.0F, 440.0F}};
    const Color lines[] {NlmColors::B1, NlmColors::B2, NlmColors::B3, NlmColors::B4};
    for (int index = 0; index < 4; ++index)
    {
        DrawLineEx(nodes[index], nodes[index + 1], 7.0F, lines[index]);
    }
    for (const Vector2 node : nodes)
    {
        DrawCircleV(node, 18.0F, NlmColors::BG_CARD);
        DrawCircleLinesV(node, 18.0F, NlmColors::TEXT);
    }
    NlmPanel::draw(Rectangle {48.0F, 610.0F, 560.0F, 54.0F}, NlmColors::CYAN);
    NlmText::draw("I: inventario  |  ESC: menú", 70, 627, 18, NlmColors::TEXT_DIM);
}

bool MapScene::shouldTransition() const
{
    return m_shouldTransition;
}

int MapScene::nextScene() const
{
    return m_nextScene;
}
