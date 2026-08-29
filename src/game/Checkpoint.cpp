//
// Created by alejo on 29/8/26.
//

#include "../../include/cybercba/game/Checkpoint.h"

namespace cybercba::game
{
// Declaracion del constructor diciendo que a estado lo va a llamar m_estado y a previo lo llamara m_previo
Checkpoint::Checkpoint(const CampaignState& estado, Checkpoint* previo) : m_estado(estado), m_previo(previo) {}

// Devuelve el estado que se guarda.
const CampaignState& Checkpoint::estado() const
{
    return m_estado;
}
// Devuelve el puntero al Checkpoint anterior
Checkpoint* Checkpoint::previo() const
{
    return m_previo;
}
} // namespace cybercba::game