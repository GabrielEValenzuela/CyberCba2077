#include "cybercba/game/ResourceRules/ReglaDeBonusDeCobertura.hpp"

namespace cybercba::game
{

ReglaDeBonusDeCobertura::ReglaDeBonusDeCobertura(const IResourceRule* pDependencia, int max, int min)
    : m_pDependencia(pDependencia), m_max(max), m_min(min)
{
}

ResourceType ReglaDeBonusDeCobertura::tipo() const
{
    return ResourceType::CoverBonus;
}

bool ReglaDeBonusDeCobertura::puedeConsumir(const CampaignState& state, int cantidad) const
{
    if (m_pDependencia != nullptr && !m_pDependencia->puedeConsumir(state, cantidad))
    {
        return false;
    }
    return cantidad > 0 && cantidad <= state.coverBonus;
}

bool ReglaDeBonusDeCobertura::puedeOtorgar(const CampaignState& state, int cantidad) const
{
    if (m_pDependencia != nullptr && !m_pDependencia->puedeOtorgar(state, cantidad))
    {
        return false;
    }
    return cantidad < m_max && state.coverBonus + (-cantidad) && cantidad > m_min;
}

const IResourceRule* ReglaDeBonusDeCobertura::reglaDeLaQueDepende() const
{
    return m_pDependencia;
}

} // namespace cybercba::game
