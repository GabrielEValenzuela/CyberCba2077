#ifndef CYBERCBA_COMBAT_GUARDSQUAD_H
#define CYBERCBA_COMBAT_GUARDSQUAD_H

#include <cstddef>

#include "cybercba/combat/GuardSquadMember.h"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::combat
{

/// Agrupa a todos los guardias de un encuentro de combate y sabe cómo
/// propagar una alerta entre ellos (VS-001), reemplazando el viejo
/// esquema de "todo o nada" donde todos los guardias se ponían alerta
/// al mismo tiempo.
///
/// GuardSquad es el único dueño de los GuardSquadMember (los guarda por
/// valor en un DynamicArray) y el único que conoce cómo recorrer el
/// grafo de vecinos. Cada GuardSquadMember solo conoce sus propios
/// vecinos, referenciados por índice dentro de este arreglo (no por
/// puntero), tal como se acordó con Facu.
class GuardSquad final
{
  public:
    GuardSquad() = default;

    /// Agrega un nuevo guardia al escuadrón y devuelve su índice. Ese
    /// índice es el valor que se usa después para conectarlo con otros
    /// guardias (conectar()) o como origen de una alerta
    /// (propagarAlerta()).
    std::size_t addMiembro();

    /// Registra a `indiceB` como vecino de `indiceA` (es decir, `indiceA`
    /// puede avisarle a `indiceB`).
    ///
    /// @param bidireccional Si es true (por defecto), también registra a
    ///     `indiceA` como vecino de `indiceB`. La mayoría de los
    ///     escuadrones se avisan en ambos sentidos.
    void conectar(std::size_t indiceA, std::size_t indiceB, bool bidireccional = true);

    /// Propaga una alerta de nivel `nivel` empezando en el guardia
    /// `origen`.
    ///
    /// Es recursiva. El caso base es: si `nivel` no supera el nivel de
    /// alerta que el guardia ya tenía, esta rama no hace nada y corta ahí
    /// (así un guardia nunca "baja" de nivel por un aviso más débil que
    /// llega desde otro lado). Si sí lo supera, actualiza el nivel del
    /// guardia y avisa a cada vecino con `nivel - 1` (un escalón menos,
    /// como indica el diagrama de flujo).
    ///
    /// Esta poda es también lo que garantiza que la propagación termina
    /// aunque los guardias estén conectados en círculo: el nivel baja de
    /// a uno en cada salto, así que tarde o temprano deja de superar el
    /// nivel guardado en algún guardia de la vuelta y la recursión corta
    /// sola, sin necesidad de un arreglo de "visitados" aparte.
    void propagarAlerta(std::size_t origen, int nivel);

    /// Cantidad de guardias en el escuadrón.
    std::size_t size() const;

    /// Acceso mutable a un guardia por índice.
    GuardSquadMember& miembro(std::size_t indice);

    /// Acceso de solo lectura a un guardia por índice.
    const GuardSquadMember& miembro(std::size_t indice) const;

  private:
    /// Dueño único de todos los guardias del combate.
    cybercba::structures::DynamicArray<GuardSquadMember> miembros_;
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDSQUAD_H
