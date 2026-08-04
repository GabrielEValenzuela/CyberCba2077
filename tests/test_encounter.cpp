#include <gtest/gtest.h>

#include "cybercba/Encounter.hpp"

namespace
{
cybercba::Encounter buildCombat()
{
    std::vector<cybercba::EncounterParticipant> participants;
    participants.push_back({"magga", "Magga", true, 100, 100, 60, "momentum", 10, false, {}});
    participants.push_back({"drone", "Dron de seguridad", false, 40, 40, 0, "energy", 5, false, {}});

    std::vector<cybercba::ActionDefinition> actions;
    actions.push_back({"strike", "Golpe", {"momentum", 20}, 25, "", 0});
    actions.push_back({"tactical_impact", "Impacto tactico", {"momentum", 40}, 15, "guard_broken", 2});
    actions.push_back({"overload_push", "Empujon final", {"momentum", 100}, 40, "", 0});
    actions.push_back({"drone_shot", "Disparo", {}, 15, "", 0});

    return cybercba::Encounter(cybercba::EncounterType::Combat, participants, actions);
}

cybercba::Encounter buildDurableCombat()
{
    std::vector<cybercba::EncounterParticipant> participants;
    participants.push_back({"magga", "Magga", true, 100, 100, 60, "momentum", 10, false, {}});
    participants.push_back({"drone", "Dron de seguridad", false, 100, 100, 0, "energy", 5, false, {}});

    std::vector<cybercba::ActionDefinition> actions;
    actions.push_back({"strike", "Golpe", {"momentum", 20}, 25, "", 0});
    actions.push_back({"tactical_impact", "Impacto tactico", {"momentum", 40}, 15, "guard_broken", 2});
    actions.push_back({"drone_shot", "Disparo", {}, 15, "", 0});

    return cybercba::Encounter(cybercba::EncounterType::Combat, participants, actions);
}
} // namespace

TEST(Encounter, TurnOrderIsSortedByInitiativeDescending)
{
    auto encounter = buildCombat();
    encounter.start();
    ASSERT_EQ(encounter.turnOrder().size(), 2U);
    EXPECT_EQ(encounter.turnOrder()[0], "magga");
    EXPECT_EQ(encounter.turnOrder()[1], "drone");
    EXPECT_EQ(encounter.activeParticipantId(), "magga");
}

TEST(Encounter, ActionOutOfTurnIsRejected)
{
    auto encounter = buildCombat();
    encounter.start();
    const auto result = encounter.performAction("drone", "drone_shot", "magga");
    EXPECT_FALSE(result.valid);
}

TEST(Encounter, ActionWithoutEnoughResourceIsRejected)
{
    auto encounter = buildCombat();
    encounter.start();
    const auto result = encounter.performAction("magga", "overload_push", "drone");
    EXPECT_FALSE(result.valid);
    EXPECT_EQ(encounter.activeParticipantId(), "magga"); // rejected action does not advance the turn
}

TEST(Encounter, SuccessfulActionAppliesDamageAndAdvancesTurn)
{
    auto encounter = buildCombat();
    encounter.start();
    const auto result = encounter.performAction("magga", "strike", "drone");
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.amountApplied, 25);
    EXPECT_EQ(encounter.participant("drone")->integrity, 15);
    EXPECT_EQ(encounter.activeParticipantId(), "drone");
}

TEST(Encounter, StatusEffectIsAppliedAndExpiresAfterDuration)
{
    auto encounter = buildDurableCombat();
    encounter.start();
    ASSERT_TRUE(encounter.performAction("magga", "tactical_impact", "drone").valid);
    ASSERT_EQ(encounter.participant("drone")->statuses.size(), 1U);
    EXPECT_EQ(encounter.participant("drone")->statuses[0].id, "guard_broken");

    // drone's turn (no-op against magga to advance), then round 2 begins and ticks status once.
    ASSERT_TRUE(encounter.performAction("drone", "drone_shot", "magga").valid);
    EXPECT_EQ(encounter.round(), 2);
    EXPECT_EQ(encounter.participant("drone")->statuses.size(), 1U);

    ASSERT_TRUE(encounter.performAction("magga", "strike", "drone").valid);
    ASSERT_TRUE(encounter.performAction("drone", "drone_shot", "magga").valid);
    EXPECT_EQ(encounter.round(), 3);
    EXPECT_TRUE(encounter.participant("drone")->statuses.empty());
}

TEST(Encounter, DefeatingAllEnemiesEndsInVictory)
{
    auto encounter = buildCombat();
    encounter.start();
    EXPECT_EQ(encounter.outcome(), cybercba::EncounterOutcome::InProgress);
    ASSERT_TRUE(encounter.performAction("magga", "strike", "drone").valid); // 40 -> 15
    ASSERT_TRUE(encounter.performAction("drone", "drone_shot", "magga").valid);
    ASSERT_TRUE(encounter.performAction("magga", "strike", "drone").valid); // 15 -> 0, defeated
    EXPECT_TRUE(encounter.participant("drone")->defeated);
    EXPECT_EQ(encounter.outcome(), cybercba::EncounterOutcome::Victory);
}

TEST(Encounter, ActionAfterOutcomeDecidedIsRejected)
{
    auto encounter = buildCombat();
    encounter.start();
    ASSERT_TRUE(encounter.performAction("magga", "strike", "drone").valid);
    ASSERT_TRUE(encounter.performAction("drone", "drone_shot", "magga").valid);
    ASSERT_TRUE(encounter.performAction("magga", "strike", "drone").valid);
    ASSERT_EQ(encounter.outcome(), cybercba::EncounterOutcome::Victory);

    const auto afterVictory = encounter.performAction("drone", "drone_shot", "magga");
    EXPECT_FALSE(afterVictory.valid);
}
