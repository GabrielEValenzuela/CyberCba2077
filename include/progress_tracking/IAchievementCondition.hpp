//
// Created by xbm on 8/25/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_IACHIEVEMENTCONDITION_H
#define CYBERPUNK_CORDOBA_2077_IACHIEVEMENTCONDITION_H

#include "model/GameModel.hpp"



class IAchievementCondition
{
   public:
   bool estaDesbloqueada(const GameModel& estadoActualDeljuego) const;

};

#endif // CYBERPUNK_CORDOBA_2077_IACHIEVEMENTCONDITION_H
