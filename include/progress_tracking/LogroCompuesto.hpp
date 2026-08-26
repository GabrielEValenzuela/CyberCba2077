//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_LOGROCOMPUESTO_H
#define CYBERPUNK_CORDOBA_2077_LOGROCOMPUESTO_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"

class LogroCompuesto : public IAchievementCondition
{
    public:
    LogroCompuesto (Lista requerimientos); //constructor, que va a recibir una lista
                                          //de los logros que deben estar completos.
                                         // implementar en cpp, que recorra la lista
                                         // y vea si cada logro esta completo para decir
                                         // que el logro compuesto esta completo


    bool estaDesbloqueada(const GameModel& estadoDelJuego) const override;



    private:
    Lista listaDeLogros;


};

#endif // CYBERPUNK_CORDOBA_2077_LOGROCOMPUESTO_H
