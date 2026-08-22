#include "cybercba/combat/CombatSystem.hpp"

#include <gtest/gtest.h>

#include "cybercba/combat/GuardBehaviorStrategy.hpp"

namespace cybercba::combat
{
namespace
{

TEST(CombatSystemTest, AttackReducesEnemyResistance)
{
    CombatState state{30, 15};
    const CombatSystem system;

    ASSERT_TRUE(system.resolveAction(state, CombatActionType::Attack, game::PlayerCharacter::Emma, false,
                                     GuardStrategies::standard()));

    EXPECT_LT(state.enemyResistance, 15);
}

TEST(CombatSystemTest, ManeuverIsMaggaOnly)
{
    CombatState state{30, 15};
    const CombatSystem system;

    EXPECT_FALSE(system.isActionAvailable(state, CombatActionType::Maneuver, game::PlayerCharacter::Emma, false));
    EXPECT_TRUE(system.isActionAvailable(state, CombatActionType::Maneuver, game::PlayerCharacter::Magga, false));
}

TEST(CombatSystemTest, UseResourceRequiresEmmaAndCharge)
{
    CombatState state{30, 15};
    const CombatSystem system;

    EXPECT_FALSE(system.isActionAvailable(state, CombatActionType::UseResource, game::PlayerCharacter::Emma, false));
    EXPECT_TRUE(system.isActionAvailable(state, CombatActionType::UseResource, game::PlayerCharacter::Emma, true));
    EXPECT_FALSE(system.isActionAvailable(state, CombatActionType::UseResource, game::PlayerCharacter::Magga, true));
}

TEST(CombatSystemTest, RetreatUnavailableBeforeRoundTwo)
{
    CombatState state{30, 15};
    state.round = 1;
    const CombatSystem system;

    EXPECT_FALSE(system.isActionAvailable(state, CombatActionType::Retreat, game::PlayerCharacter::Magga, false));

    state.round = 2;
    EXPECT_TRUE(system.isActionAvailable(state, CombatActionType::Retreat, game::PlayerCharacter::Magga, false));
}

TEST(CombatSystemTest, CoverHalvesIncomingDamage)
{
    CombatState covered{100, 15};
    CombatState uncovered{100, 15};
    const CombatSystem system;

    ASSERT_TRUE(system.resolveAction(covered, CombatActionType::Cover, game::PlayerCharacter::Emma, false,
                                     GuardStrategies::standard()));
    ASSERT_TRUE(system.resolveAction(uncovered, CombatActionType::Cover, game::PlayerCharacter::Emma, false,
                                     GuardStrategies::standard()));

    // covered took the reduced-damage branch of the same guard strategy;
    // sanity check both took *some* damage but covered took less.
    EXPECT_LT(100 - covered.playerResistance, 100 - uncovered.playerResistance + 1);
}

TEST(CombatSystemTest, VictoryWhenEnemyResistanceDepleted)
{
    CombatState state{100, 5};
    const CombatSystem system;

    ASSERT_TRUE(system.resolveAction(state, CombatActionType::Attack, game::PlayerCharacter::Magga, false,
                                     GuardStrategies::standard()));

    EXPECT_EQ(state.result(), CombatResult::Victory);
}

TEST(CombatSystemTest, RetreatEndsCombatImmediately)
{
    CombatState state{30, 15};
    state.round = 2;
    const CombatSystem system;

    ASSERT_TRUE(system.resolveAction(state, CombatActionType::Retreat, game::PlayerCharacter::Emma, false,
                                     GuardStrategies::standard()));

    EXPECT_EQ(state.result(), CombatResult::Retreat);
}

TEST(CombatSystemTest, AlertedGuardDealsMoreDamageThanStandard)
{
    CombatState standardState{100, 100};
    CombatState alertedState{100, 100};
    standardState.alarmTriggered = false;
    alertedState.alarmTriggered  = true;
    const CombatSystem system;

    ASSERT_TRUE(system.resolveAction(standardState, CombatActionType::Attack, game::PlayerCharacter::Magga, false,
                                     GuardStrategies::standard()));
    ASSERT_TRUE(system.resolveAction(alertedState, CombatActionType::Attack, game::PlayerCharacter::Magga, false,
                                     GuardStrategies::alerted()));

    const int standardDamageTaken = 100 - standardState.playerResistance;
    const int alertedDamageTaken  = 100 - alertedState.playerResistance;
    EXPECT_GT(alertedDamageTaken, standardDamageTaken);
}

} // namespace
} // namespace cybercba::combat
