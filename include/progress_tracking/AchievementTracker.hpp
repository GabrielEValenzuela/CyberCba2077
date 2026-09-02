//
// Created by xbm on 8/25/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_ACHIEVEMENTTRACKER_H
#define CYBERPUNK_CORDOBA_2077_ACHIEVEMENTTRACKER_H

#include "model/GameModel.hpp"
#include "Lista.hpp"
#include "progress_tracking/IAchievementCondition.hpp"

class AchievementTracker
{
     public:
     Lista revisarTodas(const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego);

     void registrarLogro(IAchievementCondition* condicion, const std::string& nombre);

      private:
      Lista condicionesLogros;


};

#endif // CYBERPUNK_CORDOBA_2077_ACHIEVEMENTTRACKER_H
