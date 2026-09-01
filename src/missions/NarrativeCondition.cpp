#include "cybercba/missions/NarrativeCondition.hpp"

namespace cybercba::missions
{

// --- CondicionDeFlag ---

CondicionDeFlag::CondicionDeFlag(progression::SkillId id, progression::SkillLevel nivelRequerido)
    : m_skillId(id), m_nivelRequerido(nivelRequerido)
{
}

bool CondicionDeFlag::estaSatisfecha(const progression::SkillProgress& progreso)
{
    ++m_evalCount;

    // Compara el nivel actual del jugador contra el nivel requerido por el diálogo
    return progreso.levelOf(m_skillId) >= m_nivelRequerido;
}

std::size_t CondicionDeFlag::evalCount() const
{
    return m_evalCount;
}

// --- TodasLasCondiciones ---

void TodasLasCondiciones::agregarCondicion(NarrativeCondition* pCondicion)
{
    m_condiciones.pushBack(pCondicion);
}

bool TodasLasCondiciones::estaSatisfecha(const progression::SkillProgress& progreso)
{
    for (std::size_t i = 0; i < m_condiciones.size(); ++i)
    {
        if (!m_condiciones[i]->estaSatisfecha(progreso))
        {
            return false; // Evaluación de corto circuito
        }
    }
    return true;
}

} // namespace cybercba::missions