//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H
#define CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"
#include <string>
class MisionCompletada : public IAchievementCondition
{
    public:    //va a buscar en la lista de misiones esta ultima que se completo, para dar el logro

    bool estaDesbloqueada(const List& progresoLogros, const GameModel& estadoActualDelJuego) const override //METODO MADRE QUE VERIFICA
   {
      const std::string misionesDelJueg[]={"Consigue un arma","Elimina a Arasaka","Asistir al ultimo concierto de samurai","conseguir nuevos implantes oculares"};
      std::string mision= estadoActualDelJuego.ultimaMision();

      for (std::string misionDelJuego : misionesDelJueg )
      {
        if (misionDelJuego==mision)
        {
          return true;
        }

      }
      return false;


    }

};

#endif // CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H
