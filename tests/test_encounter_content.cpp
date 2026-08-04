#include <gtest/gtest.h>

#include "cybercba/EncounterContent.hpp"

TEST(EncounterContent, EmmaHackingEncounterStartsWithEmmaActingFirst)
{
    auto encounter = cybercba::buildEmmaHackingEncounter();
    encounter.start();
    EXPECT_EQ(encounter.activeParticipantId(), "emma");
    EXPECT_NE(encounter.participant("relay"), nullptr);
}

TEST(EncounterContent, EmmaCanDefeatTheRelayThroughRepeatedActions)
{
    auto encounter = cybercba::buildEmmaHackingEncounter();
    encounter.start();
    while (encounter.outcome() == cybercba::EncounterOutcome::InProgress)
    {
        const std::string& active = encounter.activeParticipantId();
        if (active == "emma")
            encounter.performAction("emma", "extract", "relay");
        else
            encounter.performAction("relay", "relay_pulse", "emma");
    }
    EXPECT_EQ(encounter.outcome(), cybercba::EncounterOutcome::Victory);
}

TEST(EncounterContent, MaggaCombatEncounterStartsWithMaggaActingFirst)
{
    auto encounter = cybercba::buildMaggaCombatEncounter();
    encounter.start();
    EXPECT_EQ(encounter.activeParticipantId(), "magga");
    EXPECT_NE(encounter.participant("drone"), nullptr);
}

TEST(EncounterContent, MaggaCanDefeatTheDroneThroughRepeatedActions)
{
    auto encounter = cybercba::buildMaggaCombatEncounter();
    encounter.start();
    while (encounter.outcome() == cybercba::EncounterOutcome::InProgress)
    {
        const std::string& active = encounter.activeParticipantId();
        if (active == "magga")
            encounter.performAction("magga", "strike", "drone");
        else
            encounter.performAction("drone", "charge_shot", "magga");
    }
    EXPECT_EQ(encounter.outcome(), cybercba::EncounterOutcome::Victory);
}
