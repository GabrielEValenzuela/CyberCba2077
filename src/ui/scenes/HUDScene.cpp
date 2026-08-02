#include "ui/scenes/HUDScene.hpp"

#include <cstdio>

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/NlmText.hpp"

HUDScene::HUDScene(const cyberpunk::GameModel& model)
    : m_model(model)
{
}

void HUDScene::update(float dt)
{
    (void)dt;
}

void HUDScene::draw() const
{
    NlmPanel::draw(Rectangle {920.0F, 32.0F, 310.0F, 142.0F}, NlmColors::VIOLET);
    NlmText::draw("OPERADOR // ACTIVO", 942, 54, 18, NlmColors::TEXT);
    char credits[64] {};
    std::snprintf(credits, sizeof(credits), "CREDITOS: %lld", static_cast<long long>(m_model.credits()));
    NlmText::draw(credits, 942, 88, 22, NlmColors::GREEN);
    NlmText::draw("Accion: explorar red", 942, 126, 16, NlmColors::TEXT_DIM);
}

bool HUDScene::shouldTransition() const
{
    return false;
}

int HUDScene::nextScene() const
{
    return 2;
}
