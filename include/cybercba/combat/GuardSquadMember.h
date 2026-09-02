//
// Created by facundo on 2/9/26.
//

cybercba::structures::DynamicArray<GuardSquadMember*>

#ifndef ALGORITMOS_Y_ESTRUCTURAS_DE_DATOS_GUARDSQUADMEMBER_H
#define ALGORITMOS_Y_ESTRUCTURAS_DE_DATOS_GUARDSQUADMEMBER_H

// Incluimos la lista dinámica de la cátedra para guardar a los vecinos
#include "cybercba/structures/DynamicArray.hpp"
#include "cybercba/combat/GuardBehaviorStrategy.hpp"

using namespace cybercba::combat;
using namespace cybercba::structures;


class GuardSquadMember
{
    int nivelAlerta; //esta es la info que tiene el guardia
    DynArray<GuardSquadMember*> vecinos;  // el arreglo tiene los punteros de los guardias

    const EstrategiaDeAtaque* m_estrategia;
     public:
    GuardSquadMember(const EstrategiaDeAtaque* primeraEstrategia); //este es el constructor que crea un guardia
                                                                    //Este ya nacie
    //entonces estos de acá abajo son las funciones que se vana a ejecutar
    //pero como es un header nomás nombro las funciones que deberia tener
    //las implementaciones van en el cpp

    void addVecino(GuardSquadMember* vecino);
    void recibirAlerta(int nivel );

};

#endif // ALGORITMOS_Y_ESTRUCTURAS_DE_DATOS_GUARDSQUADMEMBER_H
