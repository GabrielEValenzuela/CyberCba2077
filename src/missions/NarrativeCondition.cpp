#include "cybercba/missions/NarrativeCondition.hpp"

namespace cybercba::missions
{

// ==========================================
// Implementación de CondicionDeFlag
// ==========================================

CondicionDeFlag::CondicionDeFlag(progression::SkillId id, progression::SkillLevel nivelRequerido)
    : m_skillId(id), m_nivelRequerido(nivelRequerido)
{
}

bool CondicionDeFlag::estaSatisfecha(const progression::SkillProgress& progreso)
{
    ++m_evalCount;
    // Retorna true si el nivel del jugador es igual o superior al requerido
    return progreso.levelOf(m_skillId) >= m_nivelRequerido;
}

std::size_t CondicionDeFlag::evalCount() const
{
    return m_evalCount;
}

// ==========================================
// Implementación de TodasLasCondiciones
// ==========================================

void TodasLasCondiciones::agregarCondicion(NarrativeCondition* pCondicion)
{
    m_condiciones.pushBack(pCondicion);
}

bool TodasLasCondiciones::estaSatisfecha(const progression::SkillProgress& progreso)
{
    for (std::size_t i = 0; i < m_condiciones.size(); ++i)
    {
        // Cortocircuito: si una falla, se cancela el resto de evaluaciones
        if (!m_condiciones[i]->estaSatisfecha(progreso))
        {
            return false;
        }
    }
    return true;
}

} // namespace cybercba::missions