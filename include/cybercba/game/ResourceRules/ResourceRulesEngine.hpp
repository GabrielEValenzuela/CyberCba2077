#ifndef CYBERCBA_GAME_RESOURCERULES_RESOURCERULESENGINE_H
#define CYBERCBA_GAME_RESOURCERULES_RESOURCERULESENGINE_H

#include "cybercba/game/ResourceRules/IResourceRule.hpp"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::game
{

class ResourceRulesEngine final
{
  public:
    ResourceRulesEngine(const CampaignState* pState, const structures::DynamicArray<const IResourceRule*>& reglas);

    bool consultar(ResourceType tipo, int cantidad) const;

  private:
    void validarGrafo() const;

    const CampaignState* m_pState;
    structures::DynamicArray<const IResourceRule*> m_reglas;
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_RESOURCERULES_RESOURCERULESENGINE_H
