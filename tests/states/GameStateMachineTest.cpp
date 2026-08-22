#include "cybercba/game/GameStateMachine.hpp"

#include <gtest/gtest.h>

namespace cybercba::game
{
namespace
{

TEST(GameStateMachineTest, StartsInMainMenuByDefault)
{
    const GameStateMachine machine;
    EXPECT_EQ(machine.currentState(), GameStateId::MainMenu);
}

TEST(GameStateMachineTest, FollowsVs001PrologueFlow)
{
    GameStateMachine machine;

    EXPECT_TRUE(machine.transitionTo(GameStateId::CharacterSelection));
    EXPECT_TRUE(machine.transitionTo(GameStateId::Prologue));
    EXPECT_TRUE(machine.transitionTo(GameStateId::CentralMap));
    EXPECT_TRUE(machine.transitionTo(GameStateId::Mission));
    EXPECT_TRUE(machine.transitionTo(GameStateId::Puzzle));
    EXPECT_TRUE(machine.transitionTo(GameStateId::Mission));
    EXPECT_TRUE(machine.transitionTo(GameStateId::Combat));
    EXPECT_TRUE(machine.transitionTo(GameStateId::Mission));
    EXPECT_TRUE(machine.transitionTo(GameStateId::PostMission));
    EXPECT_TRUE(machine.transitionTo(GameStateId::CentralMap));

    EXPECT_EQ(machine.currentState(), GameStateId::CentralMap);
}

TEST(GameStateMachineTest, RejectsIllegalTransition)
{
    GameStateMachine machine;

    EXPECT_FALSE(machine.transitionTo(GameStateId::Combat));
    EXPECT_EQ(machine.currentState(), GameStateId::MainMenu);
}

TEST(GameStateMachineTest, PauseAndResumeRestoresPreviousState)
{
    GameStateMachine machine;
    ASSERT_TRUE(machine.transitionTo(GameStateId::CharacterSelection));
    ASSERT_TRUE(machine.transitionTo(GameStateId::Prologue));
    ASSERT_TRUE(machine.transitionTo(GameStateId::CentralMap));
    ASSERT_TRUE(machine.transitionTo(GameStateId::Mission));

    EXPECT_TRUE(machine.pause());
    EXPECT_EQ(machine.currentState(), GameStateId::Pause);
    EXPECT_FALSE(machine.pause());

    EXPECT_TRUE(machine.resume());
    EXPECT_EQ(machine.currentState(), GameStateId::Mission);
    EXPECT_FALSE(machine.resume());
}

TEST(GameStateMachineTest, CannotTransitionWhilePaused)
{
    GameStateMachine machine;
    ASSERT_TRUE(machine.transitionTo(GameStateId::CharacterSelection));
    ASSERT_TRUE(machine.pause());

    EXPECT_FALSE(machine.transitionTo(GameStateId::Prologue));
}

} // namespace
} // namespace cybercba::game
