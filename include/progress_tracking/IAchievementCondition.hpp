//
// Created by xbm on 8/25/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_IACHIEVEMENTCONDITION_H
#define CYBERPUNK_CORDOBA_2077_IACHIEVEMENTCONDITION_H

#include "Lista.hpp"
#include "model/GameModel.hpp"

class IAchievementCondition
{
   public:
   virtual ~IAchievementCondition() = default;
   virtual bool estaDesbloqueada( const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego) const=0;

};

#endif // CYBERPUNK_CORDOBA_2077_IACHIEVEMENTCONDITION_H
