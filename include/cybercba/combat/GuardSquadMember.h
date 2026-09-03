#ifndef CYBERCBA_COMBAT_GUARDSQUADMEMBER_H
#define CYBERCBA_COMBAT_GUARDSQUADMEMBER_H

#include <cstddef>

#include "cybercba/combat/CombatState.hpp"
#include "cybercba/combat/GuardBehaviorStrategy.hpp"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::combat
{

// Un guardia dentro de un GuardSquad (issue #220): sabe su propio nivel de
// alerta, a qué otros guardias del mismo escuadrón puede avisar, y qué
// IGuardBehaviorStrategy usar según ese nivel (VS-001 §9.5.2, diagrama de
// clases de la issue: GuardSquadMember -> IGuardBehaviorStrategy).
//
// Los vecinos se guardan como índices dentro del DynamicArray que posee
// GuardSquad, no como punteros: GuardSquad guarda a sus miembros por valor,
// así que agregar un guardia nuevo puede reubicar el buffer entero y dejar
// cualquier puntero guardado de antes apuntando a memoria vieja (ver
// DynamicArray.hpp). Un índice no se invalida cuando eso pasa.
//
// La recursión que propaga la alerta entre vecinos vive en
// GuardSquad::propagarAlerta, no acá (evita tener dos implementaciones de
// la misma recorrida) — GuardSquadMember solo guarda su propio estado.
class GuardSquadMember
{
  public:
    GuardSquadMember() = default;

    // Nivel de alerta actual de este guardia.
    int nivelAlerta() const;

    // Actualiza el nivel. GuardSquad::propagarAlerta decide cuándo llamarlo
    // (solo si el nivel nuevo supera al que el guardia ya tenía).
    void setNivelAlerta(int nivel);

    // Registra a `indice` (la posición de otro guardia dentro del mismo
    // GuardSquad) como alguien a quien este guardia puede avisar.
    void addVecino(std::size_t indice);

    // Índices de los vecinos de este guardia.
    const cybercba::structures::DynamicArray<std::size_t>& vecinos() const;

    // Estrategia que le corresponde a este guardia según su nivel de
    // alerta actual: sin alerta (nivel 0) se comporta como un guardia
    // estándar; con alerta, su daño escala con el nivel propagado.
    const IGuardBehaviorStrategy& estrategia() const;

    // Atajo: resuelve el daño de este guardia con su propia estrategia y
    // su propio nivel de alerta, para que quien llame no tenga que repetir
    // la lógica de "cuál estrategia según qué nivel".
    int decideDamage(const CombatState& state) const;

  private:
    int                                              m_nivelAlerta = 0;
    cybercba::structures::DynamicArray<std::size_t> m_vecinos;
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDSQUADMEMBER_H
