#include "cybercba/combat/GuardSquad.hpp"

namespace cybercba::combat
{

std::size_t GuardSquad::addMiembro()
{
    // Se agrega un guardia recién creado (nace en nivel 0, sin vecinos,
    // según el constructor por defecto de GuardSquadMember) y se devuelve
    // su posición dentro del arreglo, que funciona como su "identidad"
    // para el resto del escuadrón.
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

    // Caso base: si este nivel no es mejor que el que el guardia ya
    // tenía, no hace nada y se termina esta rama de la recursión.
    if (nivel <= guardia.nivelAlerta())
    {
        return;
    }

    // Actualiza el nivel de alerta de este guardia.
    guardia.setNivelAlerta(nivel);

    // Avisa a cada vecino, con el nivel un escalón más bajo.
    const cybercba::structures::DynamicArray<std::size_t>& vecinos = guardia.vecinos();
    for (std::size_t i = 0; i < vecinos.size(); ++i)
    {
        propagarAlerta(vecinos[i], nivel - 1);
    }
}

std::size_t GuardSquad::size() const
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
