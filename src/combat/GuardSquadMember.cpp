#include "cybercba/combat/GuardSquadMember.h"

namespace cybercba::combat
{

int GuardSquadMember::nivelAlerta() const
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

const cybercba::structures::DynamicArray<std::size_t>& GuardSquadMember::vecinos() const
{
    return m_vecinos;
}

} // namespace cybercba::combat
