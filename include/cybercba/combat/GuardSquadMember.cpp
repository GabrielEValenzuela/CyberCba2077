//
// Created by facundo on 2/9/26.
//

#include "GuardSquadMember.h"

using namespace GuardSquadMember;



//acá implementé el constructor, los guardias nacen con cero y sin vecinos
GuardSquadMember(const EstrategiaDeAtaque* primeraEstrategia)
{
    EsteNivelAlerta =0
    EstaEstrategia = primeraEstrategia;
}

// para agregar un vecino lo inicio acá
void addVecino(GuardSquadMember* vecino)
{
    //este metodo estaba esta en la estructura del profe
    //el método pushback lo que hace es tomar el puntero y lo coloca en el primer casillero vacio.

    esteVecino.pushBack(vecino);

}

int RecibirAlerta (int nivel, nivel guardia) {
    if (nivel > nivelGuardia) {
        this.Guardia = nivel;
        avisarVecino();
    }
    return nivel;
}

void avisarVecino();
{

}


int RecibirAlerta (int nivel, nivel guardia)
{
// 
    if (nivel > nivelGuardia)
    {
        EsteGuardia =nivelEntrada;
        avisarVecino();

    }
    return nivel;
}