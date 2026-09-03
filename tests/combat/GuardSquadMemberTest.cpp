#include "cybercba/combat/GuardSquadMember.h"

#include <gtest/gtest.h>

#include "cybercba/combat/GuardBehaviorStrategy.hpp"

namespace cybercba::combat
{
namespace
{

TEST(GuardSquadMemberTest, NewGuardUsesStandardStrategy)
{
    const GuardSquadMember guard;
    const CombatState      state{30, 15};

    EXPECT_EQ(&guard.estrategia(), &GuardStrategies::standard());
    EXPECT_EQ(guard.decideDamage(state), GuardStrategies::standard().decideDamage(state, 0));
}

TEST(GuardSquadMemberTest, AlertedGuardUsesEscalatingStrategy)
{
    GuardSquadMember   guard;
    const CombatState state{30, 15};

    guard.setNivelAlerta(3);

    EXPECT_EQ(&guard.estrategia(), &GuardStrategies::escalating());
    EXPECT_EQ(guard.decideDamage(state), GuardStrategies::escalating().decideDamage(state, 3));
}

TEST(GuardSquadMemberTest, DamageGrowsAsAlertLevelRises)
{
    GuardSquadMember   guard;
    const CombatState state{30, 15};

    guard.setNivelAlerta(1);
    const int lowAlertDamage = guard.decideDamage(state);

    guard.setNivelAlerta(3);
    const int highAlertDamage = guard.decideDamage(state);

    EXPECT_GT(highAlertDamage, lowAlertDamage);
}

} // namespace
} // namespace cybercba::combat
