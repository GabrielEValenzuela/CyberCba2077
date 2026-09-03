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
    return true; // TODO
}

RequisitoDeFlag::RequisitoDeFlag(const std::string& flag)
{
    m_flag = flag;
}
bool RequisitoDeFlag::estaSatisfecha(const PlayerProgress& progreso) const
{
    return true; // TODO
}

;

} // namespace cybercba::missions