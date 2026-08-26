//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_RESULTADOEXITOSO_H
#define CYBERPUNK_CORDOBA_2077_RESULTADOEXITOSO_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"

class ResultadoExitoso : public IAchievementCondition
{
      public:
      ResultadoExitoso (int valorObjetivo) //CONSTRUCTOR QUE VERIFICA SI EL VALOR
                                           //DE LO QUE SEA, LOGRA TENER EXITO
                                           //SE IMPLEMENTA EN EL CPP,
                                           //HACIENDO UNA COMPARACION DEL VALOR OBJT CON EL VALOR RECIBIDO POR EL ESTADO DEGAMEMODEL
      {
         valor=valorObjetivo;
      }


      bool estaDesbloqueada (const GameModel& estadoActualDelJuego) const override;

      private:
      int valor;

};

#endif // CYBERPUNK_CORDOBA_2077_RESULTADOEXITOSO_H
