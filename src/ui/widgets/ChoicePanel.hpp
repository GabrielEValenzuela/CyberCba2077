#ifndef CHOICE_PANEL_HPP
#define CHOICE_PANEL_HPP

/**
 * @file ChoicePanel.hpp
 * @brief Reusable "dialogue-choice" screen template (speaker name + dialogue
 * text + a row of response buttons). See docs/mission-screen-templates.md
 * template 6.3. Presentation only; focus/selection state is owned by the
 * caller (e.g. GameApp::m_selected), matching NlmButton's convention.
 */
#include <string>
#include <vector>

#include "raylib.h"

class ChoicePanel
{
public:
    /**
     * @brief Draws a speaker line plus its response choices.
     * @param bounds Panel area.
     * @param speaker Speaker name (e.g. "EMMA", "MAGGA", "LA RED").
     * @param body Dialogue line; wrapped internally.
     * @param choices Response labels, drawn as a horizontal row of buttons.
     * @param selectedIndex Index into `choices` with input focus, or -1 for none.
     * @param font UI font, or nullptr to fall back to raylib's default.
     */
    static void draw(Rectangle bounds, const std::string& speaker, const std::string& body,
                      const std::vector<std::string>& choices, int selectedIndex, const Font* font);
};

#endif // CHOICE_PANEL_HPP
