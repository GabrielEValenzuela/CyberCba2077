#include "cybercba/Evidence.hpp"

#include <algorithm>

namespace cybercba
{

    void EvidenceJournal::registerItem(EvidenceItem item)
    {
        if (find(item.id) != nullptr)
            return;
        m_catalog.push_back(std::move(item));
    }

    const EvidenceItem* EvidenceJournal::find(const std::string& id) const
    {
        for (const auto& item : m_catalog)
        {
            if (item.id == id)
                return &item;
        }
        return nullptr;
    }

    bool EvidenceJournal::discover(const std::string& id)
    {
        if (find(id) == nullptr || isDiscovered(id))
            return false;
        m_discoveredIds.push_back(id);
        return true;
    }

    bool EvidenceJournal::isDiscovered(const std::string& id) const
    {
        return std::find(m_discoveredIds.begin(), m_discoveredIds.end(), id) != m_discoveredIds.end();
    }

    std::size_t EvidenceJournal::discoveredCount() const
    {
        return m_discoveredIds.size();
    }

    const std::vector<std::string>& EvidenceJournal::discoveredIds() const
    {
        return m_discoveredIds;
    }

    std::vector<const EvidenceItem*> EvidenceJournal::discovered() const
    {
        std::vector<const EvidenceItem*> result;
        result.reserve(m_discoveredIds.size());
        for (const auto& id : m_discoveredIds)
        {
            if (const EvidenceItem* item = find(id))
                result.push_back(item);
        }
        return result;
    }

} // namespace cybercba
