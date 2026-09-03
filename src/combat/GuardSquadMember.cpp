//
// Created by facundo on 2/9/26.
//

#include "cybercba/combat/GuardSquadMember.h"
#include <cstddef>


namespace cybercba::combat
{
using namespace std;


//acá implementé el constructor, los guardias nacen con cero y sin vecinos
GuardSquadMember::GuardSquadMember(const IGuardBehaviorStrategy* primeraEstrategia)
{
    EsteNivelAlerta = 0;
    EstaEstrategia = primeraEstrategia;
}


void GuardSquadMember::addVecino(GuardSquadMember* vecino) {
    // este metodo estaba esta en la estructura del profe
    // el método pushback lo que hace es tomar el puntero y lo coloca en el primer casillero vacio.
    estosVecinos.pushBack(vecino);
}
//nota: el guardia astá en la clase y el guardia ya conoce su propio
void GuardSquadMember::RecibirAlerta (int nivelEntrada) {
    //el nivel de entrada es mayor al mio? (nivelAlerta)
    if (nivelEntrada > EsteNivelAlerta) {
        EsteNivelAlerta= nivelEntrada; //acá actualizo

        int nivelAtenuado = atenuador(nivelEntrada);
        avisarVecinos(nivelAtenuado);
    }

}
int GuardSquadMember::atenuador(int nivelEntrada)
{
    int nivelAtenuado = nivelEntrada -10;
    return nivelAtenuado;
}

void GuardSquadMember::avisarVecinos(int nivelAtenuado) //esta función itera sobre el arreglo de los punteritos
{
    if (nivelAtenuado > 0) {
        for (unsigned int i  = 0; i < estosVecinos.size() ; ++i) {
            //entra a la memoria del vecino especifico y ejecuta reacción
            estosVecinos[i]->RecibirAlerta(nivelAtenuado);
        }
    }

}
}