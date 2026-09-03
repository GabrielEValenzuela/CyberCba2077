//
// Created by facundo on 2/9/26.
//

#include "../../include/cybercba/combat/GuardSquadMember.h"
using namespace GuardSquadMember;



//acá implementé el constructor, los guardias nacen con cero y sin vecinos
GuardSquadMember(const EstrategiaDeAtaque* primeraEstrategia)
{
    EsteNivelAlerta = 0;
    EstaEstrategia = primeraEstrategia;
}

// para agregar un vecino lo inicio acá
void addVecino(GuardSquadMember* vecino)
{
    //este metodo estaba esta en la estructura del profe
    //el método pushback lo que hace es tomar el puntero y lo coloca en el primer casillero vacio.
    estosVecinos.pushBack(vecino);
}
//nota: el guardia astá en la clase y el guardia ya conoce su propio
void RecibirAlerta (int nivelEntrada) {
    //el nivel de entrada es mayor al mio? (nivelAlerta)
    if (nivelEntrada1 > esteNivelAlerta) {
        esteNivelAlerta= nivelEntrada; //acá actualizo

        atenuador(nivelEntrada);
        avisarVecino(nivelAtenuado);
    }

}
int atenuador(int nivelEntrada)
{
    int nivelAtenuado = nivelEntrada -10;
    return nivelAtenuado;
}

void avisarVecinos(int nivelAtenuado) //esta función itera sobre el arreglo de los punteritos
{
    if (nivelAtenuado > 0) {
        for (i = 0; i < estosVecinos; ++i) {
            //entra a la memoria del vecino especifico y ejecuta reacción
            estosVecinos[i]->recibirAlerta(nivelAtenuado);
        }
    }

}


