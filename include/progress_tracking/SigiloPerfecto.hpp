//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H
#define CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"


class SigiloPerfecto : public IAchievementCondition
{
      public:
      SigiloPerfecto(bool detectado)  //CONSTRUCTOR
      {
          logroSigilo= detectado;   //ESTE CUERPO PUEDE IR EN EL CPP

      }
      bool estaDesbloqueada(const GameModel& estadoActualDelJuego) const override;

      private:
      bool logroSigilo;


};

#endif // CYBERPUNK_CORDOBA_2077_SIGILOPERFECTO_H
