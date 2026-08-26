//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H
#define CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"

template<typename T>

class MisionCompletada : public IAchievementCondition
{
    public:
    MisionCompletada(T mision) //CONTRUCTOR DE MISIONCOMPLETADA
    {
       logroMisionCompletad a= mision;
    }
    bool estaDesbloqueada(const GameModel& estadoActualDelJuego) const override //METODO MADRE QUE VERIFICA
    {
        return false; // <-----------TRABAJAR LUEGO EN ESTO!!!! AL HACERLO GENERICO (TEMPLATE)
                      // DEBO OLVIDARME DEL .CPP Y ESCRIBIR LA IMPLEMENTACION DE LOS METODOS ACA!
    }


    private:
    T logroMisionCompletada;



};

#endif // CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H
