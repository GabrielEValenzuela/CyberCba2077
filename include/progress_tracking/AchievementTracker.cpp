//
// Created by xbm on 8/25/26.
//

#include "AchievementTracker.hpp"

Lista AchievementTracker::revisarTodas(const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego)
{
    Lista logrosDesbloqueados;
    Nodo* actual= condicionesLogros.obtenerHead();

    while (actual != nullptr)
    {
        if (actual ->desbloqueado !=nullptr)  //compara la lista de logrosTOTALES con el estado del juego y anota lo conseguido
        {
            bool desbloqLogro= actual->desbloqueado->estaDesbloqueada(progresoLogros, estadoActualDeljuego);


            if (desbloqLogro==true)
            {
                logrosDesbloqueados.agregar(actual->desbloqueado , actual -> nombreDelLogro);
            }
        }
        actual=actual ->siguiente;
    }
    return logrosDesbloqueados;
}
void AchievementTracker::registrarLogro(IAchievementCondition* condicion, const std::string& nombre)
{
    condicionesLogros.agregar(condicion, nombre);

}
