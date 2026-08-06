#ifndef INTERACTION_PANEL_HPP
#define INTERACTION_PANEL_HPP

/**
 * @file InteractionPanel.hpp
 * @brief Reusable "interaction" bottom panel (template 8): portrait left,
 * contextual narration center, action menu right. Composes PortraitFrame
 * and ActionMenu instead of duplicating their rendering.
 */
#include <string>
#include <vector>

#include "raylib.h"

class InteractionPanel
{
public:
    static void draw(Rectangle bounds, const std::string& label, const Texture2D* portrait, Color accent,
                      const std::string& body, const std::vector<std::string>& actions, int selectedAction, const Font* font);
};

#endif // INTERACTION_PANEL_HPP
