#pragma once

#include <string>
#include <vector>

namespace cybercba
{

    struct EvidenceItem
    {
        std::string id;
        std::string title;
        std::string description;
        std::string imageId;
        std::string sourceScene;
        std::vector<std::string> tags;
        std::string unlocksFlag; // mission/narrative flag set on discovery
    };

    // Investigation journal: catalogs evidence definitions and tracks discovery.
    class EvidenceJournal
    {
    public:
        void registerItem(EvidenceItem item);
        const EvidenceItem* find(const std::string& id) const;

        // Returns false if the id is unknown or already discovered.
        bool discover(const std::string& id);
        bool isDiscovered(const std::string& id) const;
        std::size_t discoveredCount() const;
        std::vector<const EvidenceItem*> discovered() const;
        const std::vector<std::string>& discoveredIds() const;

    private:
        std::vector<EvidenceItem> m_catalog;
        std::vector<std::string> m_discoveredIds;
    };

} // namespace cybercba
