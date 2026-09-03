//
// Created by tacox on 9/3/26.
//

#include "cybercba/missions/IUnlockCondition.hpp"
namespace cybercba::missions
{
RequisitoDeMisionPrevia::RequisitoDeMisionPrevia(int idMision)
{
    m_idMision = idMision;
}

bool RequisitoDeMisionPrevia::estaSatisfecha(const PlayerProgress& progreso) const
{
    return progreso.estaCompletada(m_idMision);
}

RequisitoDeFlag::RequisitoDeFlag(const std::string& flag)
{
    m_flag = flag;
}
bool RequisitoDeFlag::estaSatisfecha(const PlayerProgress& progreso) const
{
    return progreso.estaSatisfecha(m_flag);
}

;

} // namespace cybercba::missions