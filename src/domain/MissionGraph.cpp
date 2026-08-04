#include "cybercba/MissionGraph.hpp"

#include <algorithm>

namespace cybercba
{

    void MissionGraph::addNode(MissionNode node)
    {
        if (hasNode(node.id))
            return;
        m_nodes.push_back(std::move(node));
    }

    bool MissionGraph::hasNode(const std::string& id) const
    {
        return find(id) != nullptr;
    }

    const MissionNode* MissionGraph::find(const std::string& id) const
    {
        for (const auto& node : m_nodes)
        {
            if (node.id == id)
                return &node;
        }
        return nullptr;
    }

    bool MissionGraph::setStart(const std::string& id)
    {
        if (!hasNode(id))
            return false;
        m_current = id;
        return true;
    }

    const std::string& MissionGraph::current() const
    {
        return m_current;
    }

    bool MissionGraph::isCompleted(const std::string& id) const
    {
        return std::find(m_completed.begin(), m_completed.end(), id) != m_completed.end();
    }

    void MissionGraph::markCompleted(const std::string& id)
    {
        if (!isCompleted(id))
            m_completed.push_back(id);
    }

    void MissionGraph::setFlag(const std::string& flag, bool value)
    {
        for (auto& entry : m_flags)
        {
            if (entry.first == flag)
            {
                entry.second = value;
                return;
            }
        }
        m_flags.emplace_back(flag, value);
    }

    bool MissionGraph::flag(const std::string& flag) const
    {
        for (const auto& entry : m_flags)
        {
            if (entry.first == flag)
                return entry.second;
        }
        return false;
    }

    std::vector<std::string> MissionGraph::activeFlags() const
    {
        std::vector<std::string> result;
        for (const auto& entry : m_flags)
        {
            if (entry.second)
                result.push_back(entry.first);
        }
        return result;
    }

    std::vector<std::string> MissionGraph::availableTransitions() const
    {
        std::vector<std::string> result;
        const MissionNode* node = find(m_current);
        if (!node)
            return result;
        for (const auto& transition : node->transitions)
        {
            if (transition.requiredFlag.empty() || flag(transition.requiredFlag))
                result.push_back(transition.targetNodeId);
        }
        return result;
    }

    bool MissionGraph::advanceTo(const std::string& targetNodeId)
    {
        const auto reachable = availableTransitions();
        if (std::find(reachable.begin(), reachable.end(), targetNodeId) == reachable.end())
            return false;
        markCompleted(m_current);
        m_current = targetNodeId;
        return true;
    }

} // namespace cybercba
