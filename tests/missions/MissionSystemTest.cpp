#include "cybercba/missions/MissionSystem.hpp"

#include <gtest/gtest.h>

namespace cybercba::missions
{
namespace
{

static constexpr DialogueLine SCENE_ONE_LINES[] = {
    {"Fantasma", "Bienvenidos al briefing.", nullptr, 0},
    {"Emma", "Entendido.", nullptr, 0},
};
static constexpr DialogueScript SCENE_ONE_SCRIPT{SCENE_ONE_LINES, 2};

static constexpr DialogueLine SCENE_THREE_LINES[] = {
    {"Emma", "Sala de servidores.", nullptr, 0},
};
static constexpr DialogueScript SCENE_THREE_SCRIPT{SCENE_THREE_LINES, 1};

static constexpr SceneDefinition PROLOGUE_SCENES[] = {
    {SceneType::Dialogue, &SCENE_ONE_SCRIPT},
    {SceneType::Puzzle, nullptr},
    {SceneType::Combat, nullptr},
    {SceneType::Dialogue, &SCENE_THREE_SCRIPT},
};
static constexpr MissionDefinition PROLOGUE{PROLOGUE_SCENES, 4};

TEST(MissionSystemTest, BeginEntersFirstDialogueScene)
{
    MissionSystem system;
    system.begin(PROLOGUE);

    EXPECT_FALSE(system.isFinished());
    EXPECT_EQ(system.currentSceneType(), SceneType::Dialogue);
    EXPECT_EQ(system.currentDialogueLine().speaker, "Fantasma");
}

TEST(MissionSystemTest, DialogueScriptEndAdvancesToNextScene)
{
    MissionSystem system;
    system.begin(PROLOGUE);

    ASSERT_TRUE(system.advanceDialogue()); // -> "Entendido." (still scene 0)
    EXPECT_EQ(system.currentSceneType(), SceneType::Dialogue);

    ASSERT_TRUE(system.advanceDialogue()); // scene 0 script finished -> scene 1 (Puzzle)
    EXPECT_EQ(system.currentSceneType(), SceneType::Puzzle);
}

TEST(MissionSystemTest, CannotAdvanceDialogueDuringPuzzleScene)
{
    MissionSystem system;
    system.begin(PROLOGUE);
    ASSERT_TRUE(system.advanceDialogue());
    ASSERT_TRUE(system.advanceDialogue());
    ASSERT_EQ(system.currentSceneType(), SceneType::Puzzle);

    EXPECT_FALSE(system.advanceDialogue());
}

TEST(MissionSystemTest, CompleteCurrentSceneMovesPastPuzzleAndCombat)
{
    MissionSystem system;
    system.begin(PROLOGUE);
    ASSERT_TRUE(system.advanceDialogue());
    ASSERT_TRUE(system.advanceDialogue());
    ASSERT_EQ(system.currentSceneType(), SceneType::Puzzle);

    EXPECT_TRUE(system.completeCurrentScene());
    EXPECT_EQ(system.currentSceneType(), SceneType::Combat);

    EXPECT_TRUE(system.completeCurrentScene());
    EXPECT_EQ(system.currentSceneType(), SceneType::Dialogue);
    EXPECT_EQ(system.currentDialogueLine().speaker, "Emma");
}

TEST(MissionSystemTest, CompleteCurrentSceneRejectedDuringDialogue)
{
    MissionSystem system;
    system.begin(PROLOGUE);

    EXPECT_FALSE(system.completeCurrentScene());
}

TEST(MissionSystemTest, MissionFinishesAfterLastScene)
{
    MissionSystem system;
    system.begin(PROLOGUE);
    ASSERT_TRUE(system.advanceDialogue());
    ASSERT_TRUE(system.advanceDialogue());
    ASSERT_TRUE(system.completeCurrentScene());
    ASSERT_TRUE(system.completeCurrentScene());
    ASSERT_TRUE(system.advanceDialogue());

    EXPECT_TRUE(system.isFinished());
}

} // namespace
} // namespace cybercba::missions
