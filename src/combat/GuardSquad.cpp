#include "cybercba/combat/GuardSquad.hpp"

namespace cybercba::combat
{

std::size_t GuardSquad::addMiembro()
{
    // NOLINTNEXTLINE(clang-analyzer-core.NullDereference)
    miembros_.pushBack(GuardSquadMember());
    return miembros_.size() - 1;
}

void GuardSquad::conectar(std::size_t indiceA, std::size_t indiceB, bool bidireccional)
{
    miembros_[indiceA].addVecino(indiceB);
    if (bidireccional)
    {
        miembros_[indiceB].addVecino(indiceA);
    }
}

void GuardSquad::propagarAlerta(std::size_t origen, int nivel)
{
    GuardSquadMember& guardia = miembros_[origen];

    if (nivel <= guardia.nivelAlerta())
    {
        return;
    }

    guardia.setNivelAlerta(nivel);

    const cybercba::structures::DynamicArray<std::size_t>& vecinos = guardia.vecinos();
    for (std::size_t i = 0; i < vecinos.size(); ++i)
    {
        propagarAlerta(vecinos[i], nivel - 1);
    }
}

std::size_t GuardSquad::size() const noexcept
{
    return miembros_.size();
}

GuardSquadMember& GuardSquad::miembro(std::size_t indice)
{
    return miembros_[indice];
}

const GuardSquadMember& GuardSquad::miembro(std::size_t indice) const
{
    return miembros_[indice];
}

} // namespace cybercba::combat
