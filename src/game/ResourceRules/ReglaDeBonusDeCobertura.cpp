#include "cybercba/game/ResourceRules/ReglaDeBonusDeCobertura.hpp"

namespace cybercba::game
{

ReglaDeBonusDeCobertura::ReglaDeBonusDeCobertura(const IResourceRule* pDependencia) : m_pDependencia(pDependencia) {}

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
    return cantidad < 0 && state.coverBonus + (-cantidad);
}

const IResourceRule* ReglaDeBonusDeCobertura::reglaDeLaQueDepende() const
{
    return m_pDependencia;
}

} // namespace cybercba::game
