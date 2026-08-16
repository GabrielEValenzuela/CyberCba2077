#include "cybercba/missions/PersuasionStrategy.hpp"

namespace cybercba::missions
{

HackerProofStrategy::HackerProofStrategy(const DialogueScript& script) : m_pScript(&script) {}

bool HackerProofStrategy::attempt(const game::CampaignState& /*state*/) const
{
    return true;
}

const DialogueScript& HackerProofStrategy::resultScript() const
{
    return *m_pScript;
}

IntimidationStrategy::IntimidationStrategy(const DialogueScript& script) : m_pScript(&script) {}

bool IntimidationStrategy::attempt(const game::CampaignState& /*state*/) const
{
    return true;
}

const DialogueScript& IntimidationStrategy::resultScript() const
{
    return *m_pScript;
}

const IPersuasionStrategy& selectPersuasionStrategy(game::PlayerCharacter character,
                                                    const HackerProofStrategy& emmaStrategy,
                                                    const IntimidationStrategy& maggaStrategy)
{
    if (character == game::PlayerCharacter::Emma)
    {
        return emmaStrategy;
    }
    return maggaStrategy;
}

} // namespace cybercba::missions
