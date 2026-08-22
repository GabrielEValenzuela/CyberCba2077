#include "cybercba/missions/DialogueSystem.hpp"

#include <gtest/gtest.h>

namespace cybercba::missions
{
namespace
{

TEST(DialogueSystemTest, AdvancesThroughLinearLines)
{
    static constexpr DialogueLine lines[] = {
        {"Emma", "Primera línea.", nullptr, 0},
        {"Magga", "Segunda línea.", nullptr, 0},
    };
    static constexpr DialogueScript script{lines, 2};

    DialogueSystem system;
    system.begin(script);

    EXPECT_EQ(system.currentLine().speaker, "Emma");
    EXPECT_TRUE(system.advance());
    EXPECT_EQ(system.currentLine().speaker, "Magga");
    EXPECT_FALSE(system.isFinished());
    EXPECT_TRUE(system.advance());
    EXPECT_TRUE(system.isFinished());
}

TEST(DialogueSystemTest, StopsAtChoicesAndResolvesSelection)
{
    static constexpr DialogueChoice choices[] = {
        {"Quedarme y terminar el descifrado", 2},
        {"Escuchar a Magga y evacuar", 1},
    };
    static constexpr DialogueLine lines[] = {
        {"Emma", "Hay una capa de cifrado extra.", choices, 2},
        {"Magga", "Evacuemos ahora.", nullptr, 0},
        {"Emma", "Voy a terminar el descifrado.", nullptr, 0},
    };
    static constexpr DialogueScript script{lines, 3};

    DialogueSystem system;
    system.begin(script);

    EXPECT_TRUE(system.hasChoices());
    EXPECT_FALSE(system.advance()); // must use selectChoice, not advance

    EXPECT_TRUE(system.selectChoice(0));
    EXPECT_EQ(system.currentLine().speaker, "Emma");
    EXPECT_EQ(system.currentLine().text, "Voy a terminar el descifrado.");
}

TEST(DialogueSystemTest, RejectsOutOfRangeChoice)
{
    static constexpr DialogueChoice choices[] = {
        {"Única opción", 1},
    };
    static constexpr DialogueLine lines[] = {
        {"Ratón", "Decidí.", choices, 1},
        {"Ratón", "Bien.", nullptr, 0},
    };
    static constexpr DialogueScript script{lines, 2};

    DialogueSystem system;
    system.begin(script);

    EXPECT_FALSE(system.selectChoice(5));
}

} // namespace
} // namespace cybercba::missions
