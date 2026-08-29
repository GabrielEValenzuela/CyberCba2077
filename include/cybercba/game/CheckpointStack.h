//
// Created by alejo on 29/8/26.
//

#ifndef CYBERCBA2077_CHECKPOINTSTACK_H
#define CYBERCBA2077_CHECKPOINTSTACK_H
#include "cybercba/game/Checkpoint.h"
namespace cybercba::game
{

// Administrador de cadena de puntos de control, guardandola con un tamaño fijo, llegando al tamaño limite descartando el mas antiguo, un sistema FIFO.
class CheckpointStack final
{
    public: CheckpointStack(int MaxSize = 6); //Creacion de una pila vacia con limite de 6.
    ~CheckpointStack(); // Destructor de la clase.


};
}


#endif // CYBERCBA2077_CHECKPOINTSTACK_H
