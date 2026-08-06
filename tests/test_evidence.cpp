#include <gtest/gtest.h>

#include "cybercba/Evidence.hpp"

namespace
{
cybercba::EvidenceJournal buildSample()
{
    cybercba::EvidenceJournal journal;
    journal.registerItem({"photo", "Fotografia", "Emma y Magga antes del apagon.", "img.photo", "shelter_workshop",
                           {"relationship"}, "trust_established"});
    journal.registerItem({"train41", "Registro del Tren 41", "El tren fue oficialmente destruido.", "img.train",
                           "shelter_archive", {"mystery"}, ""});
    return journal;
}
} // namespace

TEST(Evidence, UnregisteredItemCannotBeDiscovered)
{
    cybercba::EvidenceJournal journal;
    EXPECT_FALSE(journal.discover("missing"));
    EXPECT_FALSE(journal.isDiscovered("missing"));
}

TEST(Evidence, DiscoveringRegisteredItemSucceedsOnce)
{
    auto journal = buildSample();
    EXPECT_TRUE(journal.discover("photo"));
    EXPECT_TRUE(journal.isDiscovered("photo"));
    EXPECT_EQ(journal.discoveredCount(), 1U);
}

TEST(Evidence, DuplicateDiscoveryIsRejected)
{
    auto journal = buildSample();
    ASSERT_TRUE(journal.discover("photo"));
    EXPECT_FALSE(journal.discover("photo"));
    EXPECT_EQ(journal.discoveredCount(), 1U);
}

TEST(Evidence, DiscoveredListReturnsOnlyDiscoveredItems)
{
    auto journal = buildSample();
    journal.discover("train41");
    const auto items = journal.discovered();
    ASSERT_EQ(items.size(), 1U);
    EXPECT_EQ(items[0]->id, "train41");
}

TEST(Evidence, UnlocksFlagIsPreservedOnTheCatalogEntry)
{
    auto journal = buildSample();
    const cybercba::EvidenceItem* photo = journal.find("photo");
    ASSERT_NE(photo, nullptr);
    EXPECT_EQ(photo->unlocksFlag, "trust_established");
}
