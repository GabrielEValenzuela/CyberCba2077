#include "cybercba/progression/ProgressionSystem.hpp"

#include <gtest/gtest.h>

#include "cybercba/progression/SkillDefinition.hpp"

namespace cybercba::progression
{
namespace
{

TEST(ProgressionSystemTest, SkillsStartAtLevelI)
{
    const SkillProgress progress;
    EXPECT_EQ(progress.levelOf(SkillId::Perception), SkillLevel::LevelI);
}

TEST(ProgressionSystemTest, CanUpgradeFromLevelIToLevelII)
{
    SkillProgress progress;
    const ProgressionSystem system;

    ASSERT_TRUE(system.canUpgrade(progress, SkillId::Hacking));
    EXPECT_TRUE(system.applyUpgrade(progress, SkillId::Hacking));
    EXPECT_EQ(progress.levelOf(SkillId::Hacking), SkillLevel::LevelII);
}

TEST(ProgressionSystemTest, CannotUpgradePastLevelIIInThisSlice)
{
    SkillProgress progress;
    const ProgressionSystem system;

    ASSERT_TRUE(system.applyUpgrade(progress, SkillId::Tactics));
    EXPECT_FALSE(system.canUpgrade(progress, SkillId::Tactics));
    EXPECT_FALSE(system.applyUpgrade(progress, SkillId::Tactics));
    EXPECT_EQ(progress.levelOf(SkillId::Tactics), SkillLevel::LevelII);
}

TEST(SkillDefinitionTest, CommonSkillsAvailableToBothCharacters)
{
    EXPECT_TRUE(isSkillAvailableTo(SkillId::Perception, game::PlayerCharacter::Emma));
    EXPECT_TRUE(isSkillAvailableTo(SkillId::Perception, game::PlayerCharacter::Magga));
}

TEST(SkillDefinitionTest, ExclusiveSkillsRespectCharacter)
{
    EXPECT_TRUE(isSkillAvailableTo(SkillId::Hacking, game::PlayerCharacter::Emma));
    EXPECT_FALSE(isSkillAvailableTo(SkillId::Hacking, game::PlayerCharacter::Magga));

    EXPECT_TRUE(isSkillAvailableTo(SkillId::Tactics, game::PlayerCharacter::Magga));
    EXPECT_FALSE(isSkillAvailableTo(SkillId::Tactics, game::PlayerCharacter::Emma));
}

} // namespace
} // namespace cybercba::progression
