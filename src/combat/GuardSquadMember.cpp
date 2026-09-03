#include "cybercba/combat/GuardSquadMember.hpp"
namespace cybercba::combat
{

int GuardSquadMember::nivelAlerta() const noexcept
{
    return m_nivelAlerta;
}

void GuardSquadMember::setNivelAlerta(int nivel)
{
    m_nivelAlerta = nivel;
}

void GuardSquadMember::addVecino(std::size_t indice)
{
    m_vecinos.pushBack(indice);
}

const cybercba::structures::DynamicArray<std::size_t>& GuardSquadMember::vecinos() const noexcept
{
    return m_vecinos;
}

const IGuardBehaviorStrategy& GuardSquadMember::estrategia() const
{
    if (m_nivelAlerta > 0)
    {
        return GuardStrategies::escalating();
    }
    return GuardStrategies::standard();
}

int GuardSquadMember::decideDamage(const CombatState& state) const
{
    return estrategia().decideDamage(state, m_nivelAlerta);
}

} // namespace cybercba::combat
