//
// Created by xbm on 8/26/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H
#define CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H

#include "model/GameModel.hpp"
#include "progress_tracking/IAchievementCondition.hpp"
#include "Lista.hpp"
#include <string>

class MisionCompletada : public IAchievementCondition
{
public:
  bool estaDesbloqueada(const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego) const override
  {
    // Arreglado el typo: misionesDelJuego
    const std::string misionesDelJuego[] = {
      "Consigue un arma",
      "Elimina a Arasaka",
      "Asistir al ultimo concierto de samurai",
      "conseguir nuevos implantes oculares"
  };

    std::string mision = estadoActualDeljuego.ultimaMision();

    for (const std::string& misionDelJuego : misionesDelJuego)
    {
      if (misionDelJuego == mision)
      {
        return true;
      }
    }

    return false;
  }
};

#endif // CYBERPUNK_CORDOBA_2077_MISIONCOMPLETADA_H