#include "cybercba/game/ResourceRules/ReglaDeCargaEMP.hpp"

namespace cybercba::game
{

ReglaDeCargaEMP::ReglaDeCargaEMP(const IResourceRule* pDependencia) : m_pDependencia(pDependencia) {}

ResourceType ReglaDeCargaEMP::tipo() const
{
    return ResourceType::EmpCharge;
}

bool ReglaDeCargaEMP::puedeConsumir(const CampaignState& state, int cantidad) const
{
    if (m_pDependencia != nullptr && !m_pDependencia->puedeConsumir(state, cantidad))
    {
        return false;
    }
    return cantidad > 0 && cantidad <= state.empCharges;
}

bool ReglaDeCargaEMP::puedeOtorgar(const CampaignState& state, int cantidad) const
{
    if (m_pDependencia != nullptr && !m_pDependencia->puedeOtorgar(state, cantidad))
    {
        return false;
    }
    return cantidad < 0 && state.empCharges + (-cantidad);
}

const IResourceRule* ReglaDeCargaEMP::reglaDeLaQueDepende() const
{
    return m_pDependencia;
}

} // namespace cybercba::game
