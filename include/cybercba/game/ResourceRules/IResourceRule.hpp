#ifndef CYBERCBA_GAME_RESOURCERULES_IRESOURCERULE_H
#define CYBERCBA_GAME_RESOURCERULES_IRESOURCERULE_H

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

    virtual ResourceType tipo() const = 0;

    virtual bool puedeConsumir(const CampaignState& state, int cantidad) const = 0;

    virtual bool puedeOtorgar(const CampaignState& state, int cantidad) const = 0;

    virtual const IResourceRule* reglaDeLaQueDepende() const = 0;
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_RESOURCERULES_IRESOURCERULE_H
