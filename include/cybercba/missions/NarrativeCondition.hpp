#ifndef CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP
#define CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP

#include "cybercba/progression/ProgressionSystem.hpp"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::missions
{

/**
 * @brief Interfaz base para la evaluación de condiciones narrativas según el progreso del jugador.
 */
class NarrativeCondition
{
  public:
    virtual ~NarrativeCondition() = default;

    /**
     * @brief Evalúa si la condición se satisface con el progreso actual.
     * @param progreso Referencia al progreso de habilidades del jugador.
     * @return true si la condición se cumple, false en caso contrario.
     */
    virtual bool estaSatisfecha(const progression::SkillProgress& progreso) = 0;
};

/**
 * @brief Condición narrativa que compara el nivel de habilidad del jugador contra el exigido por la rama.
 */
class CondicionDeFlag : public NarrativeCondition
{
  private:
    progression::SkillId m_skillId;
    progression::SkillLevel m_nivelRequerido;
    mutable std::size_t m_evalCount{0};

  public:
    /**
     * @brief Construye la condición asociando una habilidad y el nivel mínimo requerido.
     * @param id Identificador de la habilidad.
     * @param nivelRequerido Nivel exigido por este nodo de diálogo.
     */
    CondicionDeFlag(progression::SkillId id, progression::SkillLevel nivelRequerido);

    /**
     * @brief Verifica si el nivel del jugador es suficiente para activar la rama.
     * @param progreso Referencia al progreso de habilidades del jugador.
     * @return true si nivelActual >= nivelRequerido.
     */
    bool estaSatisfecha(const progression::SkillProgress& progreso) override;

    /**
     * @brief Obtiene la cantidad de veces que se evaluó la condición.
     * @return Cantidad de ejecuciones.
     */
    std::size_t evalCount() const;
};

/**
 * @brief Condición compuesta (AND) que exige la satisfacción de múltiples condiciones.
 */
class TodasLasCondiciones : public NarrativeCondition
{
  private:
    structures::DynamicArray<NarrativeCondition*> m_condiciones;

  public:
    /**
     * @brief Agrega una nueva condición a la lista.
     * @param pCondicion Puntero a la condición.
     */
    void agregarCondicion(NarrativeCondition* pCondicion);

    /**
     * @brief Evalúa todas las condiciones internas mediante cortocircuito.
     * @param progreso Referencia al progreso del jugador.
     * @return true solo si TODAS las condiciones se cumplen.
     */
    bool estaSatisfecha(const progression::SkillProgress& progreso) override;
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP