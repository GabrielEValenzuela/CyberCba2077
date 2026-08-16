#include "cybercba/game/CampaignState.hpp"

namespace cybercba::game
{

void CampaignState::consumeEmpCharge()
{
    if (empCharges > 0)
    {
        --empCharges;
    }
}

void CampaignState::grantCoverBonus(int amount)
{
    coverBonus += amount;
}

} // namespace cybercba::game
