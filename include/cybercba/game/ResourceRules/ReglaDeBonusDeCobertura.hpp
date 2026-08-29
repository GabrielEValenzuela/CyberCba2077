#ifndef CYBERCBA_GAME_RESOURCERULES_REGLADEBONUSDECOBERTURA_H
#define CYBERCBA_GAME_RESOURCERULES_REGLADEBONUSDECOBERTURA_H

#include "cybercba/game/ResourceRules/IResourceRule.hpp"

namespace cybercba::game
{

class ReglaDeBonusDeCobertura final : public IResourceRule
{
  public:
    explicit ReglaDeBonusDeCobertura(const IResourceRule* pDependencia = nullptr);

    ResourceType tipo() const override;
    bool puedeConsumir(const CampaignState& state, int cantidad) const override;
    bool puedeOtorgar(const CampaignState& state, int cantidad) const override;
    const IResourceRule* reglaDeLaQueDepende() const override;

  private:
    const IResourceRule* m_pDependencia;
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_RESOURCERULES_REGLADEBONUSDECOBERTURA_H
