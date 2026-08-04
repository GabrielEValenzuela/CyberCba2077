#ifndef ACTION_MENU_HPP
#define ACTION_MENU_HPP

/**
 * @file ActionMenu.hpp
 * @brief Reusable "action-selection" screen template (vertical list of
 * focusable action buttons). See docs/mission-screen-templates.md
 * template 6.2. Focus/selection state is owned by the caller.
 */
#include <string>
#include <vector>

#include "raylib.h"

class ActionMenu
{
public:
    /**
     * @brief Draws a single action button.
     * @param bounds Button area.
     * @param label Action label.
     * @param selected Whether this item has input focus.
     * @param disabled Whether the action is currently unavailable.
     * @param font UI font, or nullptr to fall back to raylib's default.
     */
    static void drawItem(Rectangle bounds, const std::string& label, bool selected, bool disabled, const Font* font);

    /**
     * @brief Draws a vertical list of actions, one per label.
     * @param bounds Only x/y/width are used; height is `labels.size()` items
     * of `itemHeight` plus `gap` between them.
     * @param selectedIndex Index with input focus, or -1 for none.
     * @param itemHeight/gap Per-item size; defaults match GameApp's previous
     * inline rendering (42px item, 8px gap). Callers with tighter panels
     * (e.g. the encounter screen) may pass smaller values.
     */
    static void draw(Rectangle bounds, const std::vector<std::string>& labels, int selectedIndex, const Font* font, float itemHeight = 42.0F, float gap = 8.0F);
};

#endif // ACTION_MENU_HPP
