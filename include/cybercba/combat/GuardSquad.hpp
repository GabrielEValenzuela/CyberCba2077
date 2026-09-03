#ifndef CYBERCBA_COMBAT_GUARDSQUAD_HPP
#define CYBERCBA_COMBAT_GUARDSQUAD_HPP

#include <cstddef>

#include "cybercba/combat/GuardSquadMember.hpp"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::combat
{
/**
* @brief Agrupa a todos los guardias de un encuentro de combate y propaga alertas.
* @details Es el unico dueño de los GuardSquadMember y
* recorre el grafo de vecinos por indice para propagar el nivel de alerta.
*/
class GuardSquad final
{
  public:
    GuardSquad() = default;

    /**
     * @brief Agrega un nuevo guardia al escuadrón.
     * @return std::size_t Índice asignado al nuevo guardia.
     */
    std::size_t addMiembro();

    /**
     * @brief Registra una conexión de vecindad entre dos guardias.
     * @param[in] indiceA Índice del guardia origen.
     * @param[in] indiceB Índice del guardia destino.
     * @param[in] bidireccional Si es true, conecta también B con A.
     */
    void conectar(std::size_t indiceA, std::size_t indiceB, bool bidireccional = true);

    /**
     * @brief Propaga una alerta de forma recursiva a partir de un guardia origen.
     * @param[in] origen Índice del guardia inicial.
     * @param[in] nivel Nivel de alerta a propagar.
     */

    void propagarAlerta(std::size_t origen, int nivel);

    /**
     * @brief Obtiene la cantidad total de guardias en el escuadrón.
     * @return std::size_t Cantidad de miembros.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Acceso mutable a un guardia por su índice.
     * @param[in] indice Posición del guardia.
     * @return GuardSquadMember& Referencia al guardia.
     */
    GuardSquadMember& miembro(std::size_t indice);

    /**
     * @brief Acceso de solo lectura a un guardia por su índice.
     * @param[in] indice Posición del guardia.
     * @return const GuardSquadMember& Referencia constante al guardia.
     */
    [[nodiscard]] const GuardSquadMember& miembro(std::size_t indice) const;

  private:
    // Dueño único de todos los guardias del combate.
    cybercba::structures::DynamicArray<GuardSquadMember> miembros_;
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDSQUAD_HPP
