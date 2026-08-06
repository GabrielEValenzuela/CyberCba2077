#include "ui/scenes/InventoryScene.hpp"

#include <cstdio>

#include "raylib.h"
#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/NlmText.hpp"

void InventoryScene::update(float dt)
{
    (void)dt;
    m_shouldTransition = IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_I);
}

InventoryScene::InventoryScene(const cyberpunk::GameModel& model)
    : m_model(model)
{
}

void InventoryScene::draw() const
{
    NlmText::draw("INVENTARIO // SOLO LECTURA", 80, 100, 34, NlmColors::CYAN);
    NlmText::draw("Estructura del curso pendiente de integración", 80, 148, 18, NlmColors::TEXT_DIM);
    NlmPanel::draw(Rectangle {80.0F, 215.0F, 1120.0F, 160.0F}, NlmColors::GREEN);
    NlmText::draw("RECURSOS DISPONIBLES", 112, 245, 20, NlmColors::TEXT);
    char credits[64] {};
    std::snprintf(credits, sizeof(credits), "Creditos del GameModel: %lld", static_cast<long long>(m_model.credits()));
    NlmText::draw(credits, 112, 294, 30, NlmColors::GREEN);
    NlmText::draw("No se modifica el modelo desde esta pantalla.", 80, 440, 20, NlmColors::TEXT_DIM);
    NlmText::draw("I o ESC: volver al mapa", 80, 620, 18, NlmColors::TEXT_DIM);
}

bool InventoryScene::shouldTransition() const
{
    return m_shouldTransition;
}

int InventoryScene::nextScene() const
{
    return 2;
}
