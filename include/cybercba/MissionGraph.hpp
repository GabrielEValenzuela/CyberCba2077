#pragma once

#include <string>
#include <utility>
#include <vector>

namespace cybercba
{

    enum class MissionNodeType
    {
        Scene,
        Dialogue,
        Investigation,
        Encounter,
        Decision,
        Transition
    };

    struct MissionTransition
    {
        std::string targetNodeId;
        std::string requiredFlag; // empty = unconditional
    };

    struct MissionNode
    {
        std::string id;
        MissionNodeType type {MissionNodeType::Scene};
        bool optional {false};
        std::vector<MissionTransition> transitions;
    };

    // Reusable node-based mission exploration graph. Infrastructure-level
    // (not an academic structure), so plain std::vector is used deliberately.
    class MissionGraph
    {
    public:
        void addNode(MissionNode node);
        bool hasNode(const std::string& id) const;
        const MissionNode* find(const std::string& id) const;
        const std::vector<MissionNode>& nodes() const { return m_nodes; }

        bool setStart(const std::string& id);
        const std::string& current() const;

        bool isCompleted(const std::string& id) const;
        void markCompleted(const std::string& id);

        void setFlag(const std::string& flag, bool value = true);
        bool flag(const std::string& flag) const;
        std::vector<std::string> activeFlags() const;

        // Transitions from the current node whose requiredFlag (if any) is satisfied.
        std::vector<std::string> availableTransitions() const;

        // Validates target is reachable from current via a satisfied transition,
        // marks current as completed, and moves to target.
        bool advanceTo(const std::string& targetNodeId);

    private:
        std::vector<MissionNode> m_nodes;
        std::string m_current;
        std::vector<std::string> m_completed;
        std::vector<std::pair<std::string, bool>> m_flags;
    };

} // namespace cybercba
