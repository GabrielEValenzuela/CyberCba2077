//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H
#define CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"
#include "Lista.hpp"


class SigiloPerfecto : public IAchievementCondition  //TERMINADO
{
      public:
      bool estaDesbloqueada(const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego) const override
      {
            bool detectado= estadoActualDeljuego.fueDetectado(); ///DEPENDE DATO GAMEMODEL
            return !detectado; // SI fue detectado ->devuelve NO al logro
      }

};

#endif // CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H
