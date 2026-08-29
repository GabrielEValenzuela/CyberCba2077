//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H
#define CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"


class SigiloPerfecto : public IAchievementCondition  //TERMINADO
{
      public:
      bool estaDesbloqueada(const List& progresoLogros, const GameModel& estadoActualDelJuego) const override
      {
            bool detectado= estadoActualDelJuego.fueDetectado(); ///DEPENDE DATO GAMEMODEL
            return !detectado; // SI fue detectado ->devuelve NO al logro
      }

};

#endif // CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H
