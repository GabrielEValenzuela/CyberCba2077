#ifndef STATUS_PANEL_HPP
#define STATUS_PANEL_HPP

/**
 * @file StatusPanel.hpp
 * @brief Reusable status-effect row for the encounter screen (template 7.2).
 * Draws cybercba::StatusEffect entries as compact id+remaining-turns chips
 * (no dedicated status icon art exists yet — see docs/static-art-direction.md
 * `icon` category — so this stays text-based, not a fake icon placeholder).
 */
#include <vector>

#include "cybercba/Encounter.hpp"
#include "raylib.h"

class StatusPanel
{
public:
    static void draw(Rectangle bounds, const std::vector<cybercba::StatusEffect>& statuses, const Font* font);
};

#endif // STATUS_PANEL_HPP
