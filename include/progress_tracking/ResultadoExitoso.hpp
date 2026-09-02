//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_RESULTADOEXITOSO_H
#define CYBERPUNK_CORDOBA_2077_RESULTADOEXITOSO_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"
#include "Lista.hpp"

class ResultadoExitoso : public IAchievementCondition
{
      public:                               //SUPOSICION
                                           //CONSTRUCTOR QUE VERIFICA SI EL VALOR
                                           //DE LO QUE SEA, LOGRA TENER EXITO
                                           //HACIENDO UNA COMPARACION DEL VALOR OBJT CON EL VALOR RECIBIDO POR EL ESTADO DEGAMEMODEL
        bool estaDesbloqueada(const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego) const override
        {
        int valor=estadoActualDeljuego.resultadoObtenido(); // DEPENDE DATO GAMEMODEL
        return valor>=60;   //CUAL SERIA EL VALOR OBJETIVO? suponemos que el minimo para pasar es 60%

         }


};

#endif // CYBERPUNK_CORDOBA_2077_RESULTADOEXITOSO_H
