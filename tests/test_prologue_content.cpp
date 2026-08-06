#include <gtest/gtest.h>

#include "cybercba/PrologueContent.hpp"

TEST(PrologueContent, MissionGraphStartsAtRefugeWithInvestigationNodes)
{
    auto graph = cybercba::buildPrologueMissionGraph();
    EXPECT_EQ(graph.current(), "refuge");
    for (const char* id : {"rest_area", "workshop", "comms_room", "archive", "blackout", "power_recovery",
                            "fragmented_transmission", "protagonist_encounter", "narrative_choice", "exterior_route",
                            "security_control", "neometro_entrance", "prologue_complete"})
        EXPECT_TRUE(graph.hasNode(id)) << id << " should exist";
}

TEST(PrologueContent, PowerRecoveryGatesFragmentedTransmissionBehindAFlag)
{
    auto graph = cybercba::buildPrologueMissionGraph();
    ASSERT_TRUE(graph.advanceTo("blackout"));
    ASSERT_TRUE(graph.advanceTo("power_recovery"));
    EXPECT_FALSE(graph.advanceTo("fragmented_transmission"));
    graph.setFlag("power_restored");
    EXPECT_TRUE(graph.advanceTo("fragmented_transmission"));
}

TEST(PrologueContent, EvidenceCatalogCoversTheRequiredNarrativeBeats)
{
    auto journal = cybercba::buildPrologueEvidenceCatalog();
    for (const char* id : {"photo", "insignia", "network_map", "unsent_message", "train41_record"})
        EXPECT_NE(journal.find(id), nullptr) << id << " should be registered";
    EXPECT_EQ(journal.discoveredCount(), 0U); // catalog only, nothing discovered yet
}

TEST(PrologueContent, DialogueCoversNetworkFirefliesBlackoutAndTransmission)
{
    const auto& dialogue = cybercba::prologueDialogue();
    for (const char* id : {"opening_network", "opening_fireflies", "opening_blackout", "transmission_fragment",
                            "transmission_complete", "choice_prompt", "neometro_ending"})
        EXPECT_NE(dialogue.find(id), nullptr) << id << " should exist";
}

TEST(PrologueContent, UnselectedProtagonistVoicesTheTransmission)
{
    const auto& dialogue = cybercba::prologueDialogue();
    EXPECT_EQ(dialogue.resolveSpeakerName("transmission_complete", cybercba::CharacterId::Emma),
              cybercba::characterName(cybercba::CharacterId::Magga));
    EXPECT_EQ(dialogue.resolveSpeakerName("transmission_complete", cybercba::CharacterId::Magga),
              cybercba::characterName(cybercba::CharacterId::Emma));
}

TEST(PrologueContent, ChoicePromptOffersReplyAnalyzeAndDisconnect)
{
    const auto& dialogue = cybercba::prologueDialogue();
    const auto* line = dialogue.find("choice_prompt");
    ASSERT_NE(line, nullptr);
    ASSERT_EQ(line->choices.size(), 3U);
    EXPECT_EQ(line->choices[0].setFlag, "chose_reply");
    EXPECT_EQ(line->choices[1].setFlag, "chose_analyze");
    EXPECT_EQ(line->choices[2].setFlag, "chose_disconnect");
}
