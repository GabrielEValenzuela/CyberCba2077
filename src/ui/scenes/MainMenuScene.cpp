#include "ui/scenes/MainMenuScene.hpp"

#include "raylib.h"
#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmButton.hpp"
#include "ui/widgets/NlmText.hpp"

namespace
{
    constexpr int MAP_SCENE_ID = 2;
    constexpr int EXIT_SCENE_ID = -1;
    constexpr int MENU_ITEMS = 3;
} // namespace

void MainMenuScene::update(float dt)
{
    (void)dt;
    m_shouldTransition = false;
    if (IsKeyPressed(KEY_DOWN))
    {
        m_selectedIndex = (m_selectedIndex + 1) % MENU_ITEMS;
    }
    if (IsKeyPressed(KEY_UP))
    {
        m_selectedIndex = (m_selectedIndex + MENU_ITEMS - 1) % MENU_ITEMS;
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        m_shouldTransition = true;
        m_nextScene = m_selectedIndex == 2 ? EXIT_SCENE_ID : MAP_SCENE_ID;
    }
}

void MainMenuScene::draw() const
{
    NlmText::drawCentered("NEOMETRO", 120, 54, NlmColors::CYAN);
    NlmText::drawCentered("Elegí tu ruta", 190, 24, NlmColors::TEXT_DIM);
    const char* labels[MENU_ITEMS] {"Nueva partida", "Cargar", "Salir"};
    for (int index = 0; index < MENU_ITEMS; ++index)
    {
        NlmButton button {Rectangle {490.0F, 270.0F + index * 78.0F, 300.0F, 58.0F}, labels[index]};
        button.draw(index == m_selectedIndex);
    }
    NlmText::drawCentered("Flechas: navegar | Enter: confirmar", 580, 18, NlmColors::TEXT_DIM);
}

bool MainMenuScene::shouldTransition() const
{
    return m_shouldTransition;
}

int MainMenuScene::nextScene() const
{
    return m_nextScene;
}
