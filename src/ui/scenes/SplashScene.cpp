#include "ui/scenes/SplashScene.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmText.hpp"

namespace
{
    constexpr int MAIN_MENU_SCENE_ID = 1;
} // namespace

SplashScene::SplashScene(float durationSeconds)
    : m_durationSeconds(durationSeconds)
{
}

void SplashScene::update(float dt)
{
    m_elapsedSeconds += dt;
}

void SplashScene::draw() const
{
    NlmText::drawCentered("CYBER", 245, 72, NlmColors::CYAN);
    NlmText::drawCentered("CBA", 325, 72, NlmColors::VIOLET);
    NlmText::drawCentered("CORDOBA // 2077", 430, 24, NlmColors::TEXT_DIM);
}

bool SplashScene::shouldTransition() const
{
    return m_elapsedSeconds >= m_durationSeconds;
}

int SplashScene::nextScene() const
{
    return MAIN_MENU_SCENE_ID;
}
