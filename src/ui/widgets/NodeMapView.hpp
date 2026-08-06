#ifndef NODE_MAP_VIEW_HPP
#define NODE_MAP_VIEW_HPP

/**
 * @file NodeMapView.hpp
 * @brief Reusable "mission node map" screen template (7.1): discovered
 * nodes, connections, locked/optional/completed state, current position.
 * Presentation only — the caller builds `Entry` from
 * cybercba::MissionGraph::nodes()/isCompleted()/current() (see
 * GameApp::buildNodeMapEntries in src/app/GameApp.cpp), so this widget has
 * no dependency on the mission-graph domain type.
 */
#include <string>
#include <vector>

#include "raylib.h"

class NodeMapView
{
public:
    struct Entry
    {
        std::string id;
        std::string label;
        bool completed {false};
        bool current {false};
        bool optional {false};
        bool locked {false};
        std::vector<std::string> connectsTo; // ids of other entries reachable from this one
    };

    /**
     * @brief Draws every entry along a single horizontal line inside
     * `bounds`, in the order given, connecting nodes per `connectsTo`.
     * Layout is deliberately simple (no branching geometry) since the real
     * mission graph so far is a mostly-linear chain with a few optional
     * side nodes — see docs/mission-screen-templates.md.
     */
    static void draw(Rectangle bounds, const std::vector<Entry>& entries, const Font* font);
};

#endif // NODE_MAP_VIEW_HPP
