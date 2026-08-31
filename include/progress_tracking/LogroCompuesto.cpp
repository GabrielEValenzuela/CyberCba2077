//
// Created by xbm on 8/26/26.
//

#include "progress_tracking/LogroCompuesto.hpp"
#include "Lista.hpp"
bool LogroCompuesto::estaDesbloqueada(const Lista& progresoLogros, const cyberpunk::GameModel& estadoActualDeljuego) const
{
    Nodo* actual=listaDeLogros.obtenerHead();
    while (actual!=nullptr)
    {
        if (actual->desbloqueado !=nullptr)
        {
            bool seCumplio= actual->desbloqueado->estaDesbloqueada(progresoLogros, estadoActualDeljuego);
          if (!seCumplio)
          {
              return false;  //si una sola condicion falla, no se da el logro
          }
        }
        actual=actual->siguiente;
    }
    return true; //recorrio todas las condiciones, ninguna fallo, entonces se da el logro


}