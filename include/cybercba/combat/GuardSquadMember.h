#ifndef CYBERCBA_COMBAT_GUARDSQUADMEMBER_H
#define CYBERCBA_COMBAT_GUARDSQUADMEMBER_H

#include <cstddef>

#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::combat
{

// Un guardia dentro de un GuardSquad (issue #220): sabe su propio nivel de
// alerta y a qué otros guardias del mismo escuadrón puede avisar.
//
// Los vecinos se guardan como índices dentro del DynamicArray que posee
// GuardSquad, no como punteros: GuardSquad guarda a sus miembros por valor,
// así que agregar un guardia nuevo puede reubicar el buffer entero y dejar
// cualquier puntero guardado de antes apuntando a memoria vieja (ver
// DynamicArray.hpp). Un índice no se invalida cuando eso pasa.
//
// GuardSquadMember es un simple contenedor de datos: la recursión que
// propaga la alerta entre vecinos vive en GuardSquad::propagarAlerta, no
// acá (evita tener dos implementaciones de la misma recorrida).
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

  private:
    int                                              m_nivelAlerta = 0;
    cybercba::structures::DynamicArray<std::size_t> m_vecinos;
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDSQUADMEMBER_H
