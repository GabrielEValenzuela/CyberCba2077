#ifndef CYBERCBA_GAME_RESOURCERULES_IRESOURCERULE_H
#define CYBERCBA_GAME_RESOURCERULES_IRESOURCERULE_H

#include <cstdint>

#include "cybercba/game/CampaignState.hpp"

namespace cybercba::game
{

enum class ResourceType : std::uint8_t
{
    EmpCharge,
    CoverBonus,
};

class IResourceRule
{
  public:
    virtual ~IResourceRule() = default;

    virtual ResourceType tipo();

    virtual bool puedeConsumir(const CampaignState& state, int cantidad);

    virtual bool puedeOtorgar(const CampaignState& state, int cantidad);

    virtual const IResourceRule* reglaDeLaQueDepende();
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_RESOURCERULES_IRESOURCERULE_H
