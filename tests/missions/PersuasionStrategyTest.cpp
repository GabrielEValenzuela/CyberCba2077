#include "cybercba/missions/PersuasionStrategy.hpp"

#include <gtest/gtest.h>

namespace cybercba::missions
{
namespace
{

TEST(PersuasionStrategyTest, SelectsHackerProofForEmma)
{
    static constexpr DialogueLine emmaLines[]  = {{"Emma", "Te muestro el canal cifrado.", nullptr, 0}};
    static constexpr DialogueLine maggaLines[] = {{"Magga", "No hay tiempo para esto.", nullptr, 0}};
    static constexpr DialogueScript emmaScript{emmaLines, 1};
    static constexpr DialogueScript maggaScript{maggaLines, 1};

    const HackerProofStrategy emmaStrategy(emmaScript);
    const IntimidationStrategy maggaStrategy(maggaScript);

    const IPersuasionStrategy& selected =
        selectPersuasionStrategy(game::PlayerCharacter::Emma, emmaStrategy, maggaStrategy);

    EXPECT_EQ(&selected.resultScript(), &emmaScript);
}

TEST(PersuasionStrategyTest, SelectsIntimidationForMagga)
{
    static constexpr DialogueLine emmaLines[]  = {{"Emma", "Te muestro el canal cifrado.", nullptr, 0}};
    static constexpr DialogueLine maggaLines[] = {{"Magga", "No hay tiempo para esto.", nullptr, 0}};
    static constexpr DialogueScript emmaScript{emmaLines, 1};
    static constexpr DialogueScript maggaScript{maggaLines, 1};

    const HackerProofStrategy emmaStrategy(emmaScript);
    const IntimidationStrategy maggaStrategy(maggaScript);

    const IPersuasionStrategy& selected =
        selectPersuasionStrategy(game::PlayerCharacter::Magga, emmaStrategy, maggaStrategy);

    EXPECT_EQ(&selected.resultScript(), &maggaScript);
}

TEST(PersuasionStrategyTest, BothRoutesSucceedFunctionally)
{
    static constexpr DialogueLine lines[] = {{"Ratón", "De acuerdo.", nullptr, 0}};
    static constexpr DialogueScript script{lines, 1};
    const HackerProofStrategy hackerStrategy(script);
    const IntimidationStrategy intimidationStrategy(script);
    const game::CampaignState state{};

    EXPECT_TRUE(hackerStrategy.attempt(state));
    EXPECT_TRUE(intimidationStrategy.attempt(state));
}

} // namespace
} // namespace cybercba::missions
