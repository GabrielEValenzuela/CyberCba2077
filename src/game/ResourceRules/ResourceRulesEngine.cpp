#include "cybercba/game/ResourceRules/ResourceRulesEngine.hpp"

#include <cstddef>
#include <stdexcept>

namespace cybercba::game
{

ResourceRulesEngine::ResourceRulesEngine(const CampaignState* pState,
                                         const structures::DynamicArray<const IResourceRule*>& reglas)
    : m_pState(pState), m_reglas(reglas)
{
}

bool ResourceRulesEngine::consultar(ResourceType tipo, int cantidad) const
{
    if (cantidad == 0 || m_pState == nullptr)
    {
        return false;
    }

    const bool bConsumir = cantidad > 0;
    bool bEncontroRegla  = false;
    for (std::size_t i = 0; i < m_reglas.size(); ++i)
    {
        const IResourceRule* pRegla = m_reglas[i];
        if (pRegla == nullptr || pRegla->tipo() != tipo)
        {
            continue;
        }

        bEncontroRegla = true;

        const bool bValida =
            bConsumir ? pRegla->puedeConsumir(*m_pState, cantidad) : pRegla->puedeOtorgar(*m_pState, cantidad);
        if (!bValida)
        {
            return false;
        }
    }

    return bEncontroRegla;
}
} // namespace cybercba::game
