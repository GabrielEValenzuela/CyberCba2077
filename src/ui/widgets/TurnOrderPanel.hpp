#ifndef TURN_ORDER_PANEL_HPP
#define TURN_ORDER_PANEL_HPP

/**
 * @file TurnOrderPanel.hpp
 * @brief Reusable turn-order strip for the encounter screen (template 7.2).
 * Presentation only; order/active state come from cybercba::Encounter
 * (turnOrder()/activeParticipantId()).
 */
#include <string>
#include <vector>

#include "raylib.h"

class TurnOrderPanel
{
public:
    struct Entry
    {
        std::string id;
        std::string label;
    };

    static void draw(Rectangle bounds, const std::vector<Entry>& order, const std::string& activeId, const Font* font);
};

#endif // TURN_ORDER_PANEL_HPP
