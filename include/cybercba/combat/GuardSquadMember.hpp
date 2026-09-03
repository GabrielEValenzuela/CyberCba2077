#ifndef CYBERCBA_COMBAT_GUARDSQUADMEMBER_HPP
#define CYBERCBA_COMBAT_GUARDSQUADMEMBER_HPP
#include <cstddef>

#include "cybercba/combat/CombatState.hpp"
#include "cybercba/combat/GuardBehaviorStrategy.hpp"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::combat
{

/**
 * @brief Un guardia dentro de un GuardSquad
 * @details Conoce su nivel de alerta y a qué guardias del mismo escuadron
 * avisar y qué IGuardBehaviorStrategy utilizar segun dicho nivel.
 */
class GuardSquadMember
{
  public:
    GuardSquadMember();

    /**
     * @brief Devuelve el nivel de alerta actual de este guardia.
     * @return int NIvel de alerta almacenado.
     */
    [[nodiscard]] int nivelAlerta() const noexcept;

    /**
     * @brief Actualiza el nivel de alerta del guardia
     * @param[in] nivel Nuevo nivel de alerta
     */
    void setNivelAlerta(int nivel);

    /**
     * @brief Registrar la posicion de otro guardia como vecino a avisar.
     * @param[in] indice Indice del guardia dentro del GuardSquad
     */
    void addVecino(std::size_t indice);

    /**
     * @brief Devuelve los indices de los vecinos de este guardia.
     * @return Referencia a la lista de indices.
     */
    [[nodiscard]] const cybercba::structures::DynamicArray<std::size_t>& vecinos() const noexcept;

    /**
     * @brief Obtiene la estrategia correspondiente segun el nivel de alerta.
     * @return Referencia a la estrategia de comportamiento.
     */
    [[nodiscard]] const IGuardBehaviorStrategy& estrategia() const;

    /**
     *@brief Resuelve el daño de este guardia usando su estrategia y nivel.
     * @param[in] state Estado actual del combate.
     * @return int Cantidad de daño calculada.
     *
     */
    [[nodiscard]] int decideDamage(const CombatState& state) const;

  private:
    int m_nivelAlerta = 0;
    cybercba::structures::DynamicArray<std::size_t> m_vecinos;
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDSQUADMEMBER_HPP
