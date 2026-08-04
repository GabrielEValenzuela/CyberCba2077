#ifndef CURRENT_OBJECTIVE_PANEL_HPP
#define CURRENT_OBJECTIVE_PANEL_HPP

/**
 * @file CurrentObjectivePanel.hpp
 * @brief Upper-right "current objective" readout (amber title, readable
 * body text), part of the shared header row alongside the mission progress
 * bar (NodeMapView). See docs/mission-screen-templates.md.
 */
#include <string>

#include "raylib.h"

class CurrentObjectivePanel
{
public:
    static void draw(Rectangle bounds, const std::string& title, const std::string& body, const Font* font);
};

#endif // CURRENT_OBJECTIVE_PANEL_HPP
