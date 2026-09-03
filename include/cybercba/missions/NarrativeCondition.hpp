#ifndef CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP
#define CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP

#include "cybercba/progression/ProgressionSystem.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <cstddef>

namespace cybercba::missions
{

/**
 * @brief Interfaz base para todas las condiciones narrativas de un nodo de diálogo.
 */
class NarrativeCondition
{
  public:
    virtual ~NarrativeCondition() = default;

    /**
     * @brief Determina si la condición se cumple según el estado actual del jugador.
     * @param progreso Objeto con el nivel de habilidades del jugador.
     * @return true si la condición se satisface, false en caso contrario.
     */
    virtual bool estaSatisfecha(const progression::SkillProgress& progreso) = 0;
};

/**
 * @brief Condición simple que verifica si una habilidad alcanza un nivel mínimo.
 */
class CondicionDeFlag : public NarrativeCondition
{
  private:
    progression::SkillId m_skillId;
    progression::SkillLevel m_nivelRequerido;
    mutable std::size_t m_evalCount{0};

  public:
    /**
     * @brief Constructor de la condición individual.
     * @param id Identificador de la habilidad a evaluar.
     * @param nivelRequerido Nivel mínimo exigido para habilitar el diálogo.
     */
    CondicionDeFlag(progression::SkillId id, progression::SkillLevel nivelRequerido);

    bool estaSatisfecha(const progression::SkillProgress& progreso) override;

    /**
     * @brief Devuelve la cantidad de veces que se ha evaluado esta condición.
     */
    std::size_t evalCount() const;
};

/**
 * @brief Condición compuesta (AND) que almacena múltiples condiciones en un arreglo.
 */
class TodasLasCondiciones : public NarrativeCondition
{
  private:
    structures::DynamicArray<NarrativeCondition*> m_condiciones;

  public:
    /**
     * @brief Añade una nueva condición al arreglo de validación.
     * @param pCondicion Puntero a la condición narrativa a agregar.
     */
    void agregarCondicion(NarrativeCondition* pCondicion);

    bool estaSatisfecha(const progression::SkillProgress& progreso) override;
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP