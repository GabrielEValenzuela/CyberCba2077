#include <gtest/gtest.h>

#include <set>

#include "cybercba/CharacterModule.hpp"

TEST(CharacterModule, PrologueRosterHasNineUniquelyIdentifiedModules)
{
    auto roster = cybercba::buildPrologueCharacterRoster();
    EXPECT_EQ(roster.modules().size(), 9U);

    std::set<std::string> ids;
    for (const auto& module : roster.modules())
        ids.insert(module->id());
    EXPECT_EQ(ids.size(), 9U) << "character ids must be unique";
}

TEST(CharacterModule, PrologueRosterCoversTheNineExpectedRoles)
{
    auto roster = cybercba::buildPrologueCharacterRoster();
    for (const char* id : {"luma", "sistema", "voz_falsa_magga", "orden_falsa", "rele_interferencia",
                            "dron_seguridad", "sistema_vigilancia", "patrulla_seguridad", "tomo"})
        EXPECT_NE(roster.find(id), nullptr) << id << " should be registered";
}

TEST(CharacterModule, NarrativeVoiceModulesContributeAtLeastOneDialogueLine)
{
    auto roster = cybercba::buildPrologueCharacterRoster();
    for (const auto& module : roster.modules())
    {
        if (module->role() != cybercba::CharacterRole::NarrativeVoice)
            continue;
        cybercba::DialogueGraph graph;
        module->contributeDialogue(graph);
        EXPECT_NE(graph.find("npc_" + module->id() + "_placeholder"), nullptr)
            << module->id() << " should register a placeholder line";
    }
}

TEST(CharacterModule, EncounterOpponentModulesBuildAParticipantWithAtLeastOneAction)
{
    auto roster = cybercba::buildPrologueCharacterRoster();
    for (const auto& module : roster.modules())
    {
        if (module->role() != cybercba::CharacterRole::EncounterOpponent)
            continue;
        const auto participant = module->buildParticipant();
        EXPECT_EQ(participant.id, module->id());
        EXPECT_FALSE(participant.isPlayerControlled);
        EXPECT_FALSE(module->buildActions().empty()) << module->id() << " should define at least one action";
    }
}
