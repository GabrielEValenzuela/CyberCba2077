#ifndef ENEMY_INTENT_PANEL_HPP
#define ENEMY_INTENT_PANEL_HPP

/**
 * @file EnemyIntentPanel.hpp
 * @brief Reusable "what the opponent will do next" indicator (template 7.2).
 * The current prologue encounters use a small deterministic enemy AI (see
 * GameApp::updateEncounter), so the caller can compute a real intent label
 * ahead of time instead of guessing — this widget never fabricates intent.
 */
#include <string>

#include "raylib.h"

class EnemyIntentPanel
{
public:
    static void draw(Rectangle bounds, const std::string& intentLabel, const Font* font);
};

#endif // ENEMY_INTENT_PANEL_HPP
