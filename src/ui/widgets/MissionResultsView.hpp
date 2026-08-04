#ifndef MISSION_RESULTS_VIEW_HPP
#define MISSION_RESULTS_VIEW_HPP

/**
 * @file MissionResultsView.hpp
 * @brief Reusable "mission results" screen template (7.4): title, decisions/
 * outcome text, and recovered evidence (via EvidencePanel). Composes
 * EvidencePanel rather than duplicating its thumbnail+title rendering.
 */
#include <string>
#include <vector>

#include "raylib.h"

class MissionResultsView
{
public:
    struct EvidenceEntry
    {
        const Texture2D* image {nullptr};
        std::string title;
    };

    static void draw(Rectangle bounds, const std::string& title, const std::string& body, const std::string& hint,
                      const std::vector<EvidenceEntry>& evidence, const Font* font);
};

#endif // MISSION_RESULTS_VIEW_HPP
