//
// Created by alejo on 29/8/26.
//

/*
#ifndef CYBERCBA2077_CHECKPOINTSTACK_H
#define CYBERCBA2077_CHECKPOINTSTACK_H
#include "cybercba/game/Checkpoint.h"
namespace cybercba::game
{

// Administrador de cadena de puntos de control, guardandola con un tamaño fijo, llegando al tamaño limite descartando
// el mas antiguo, un sistema FIFO.
class CheckpointStack final
{
  public:
    CheckpointStack(int maxSize = 6); // Creacion de una pila vacia con limite de 6.
    ~CheckpointStack();               // Destructor de la clase.
    private:
    int m_maxSize;
};
}


#endif // CYBERCBA2077_CHECKPOINTSTACK_H

*/

//-----------------------------------------OCPION MATEO----------------------------------------------------
#ifndef CYBERCBA2077_CHECKPOINTSTACK_H
#define CYBERCBA2077_CHECKPOINTSTACK_H

#include "cybercba/game/Checkpoint.h"
#include "cybercba/game/CampaignState.hpp"
#include <cstddef> //Libreria de excepciones para errores comunes --> Recomendacion de la IA

namespace cybercba::game
{

// Administrador de cadena de puntos de control con tamaño fijo.
// Al alcanzar el límite, descarta el punto más antiguo.
class CheckpointStack final //--> Por que FINAL? porque es una clase concreta pensada para gestionar la pila de memoria, no una plantilla o interfaz pensada para extenderse.
{
public: //Predeterminadamente "Class" viene en private, asi q hay q dejar claro que parte sera public.
    // Creación de una pila vacía con límite por defecto de 6
    explicit CheckpointStack(int maxSize = 6);
    ~CheckpointStack(); //--> Destructor de cada checkpoint de la pila

    // Deshabilitar copia para prevenir problemas de doble liberación de memoria
    CheckpointStack(const CheckpointStack&) = delete;
    CheckpointStack& operator=(const CheckpointStack&) = delete;

    // Métodos según el diagrama UML
    void agregar(const CampaignState& foto);
    CampaignState volverAlUltimo();
    int profundidad() const;

    // Métodos auxiliares recomendados
    void vaciar();
    bool estaVacia() const;
    int limiteMaximo() const;

private:
    int m_maxSize;
    Checkpoint* m_tope; // Puntero al checkpoint más reciente

    // Función recursiva para contar según el diagrama de flujo
    static int contarRecursivo(const Checkpoint* nodo);
};

} // namespace cybercba::game

#endif // CYBERCBA2077_CHECKPOINTSTACK_H