//
// Created by facundo on 2/9/26.
//

#include "GuardSquad.h"

#include "GuardSquadMember.h"
using namespace GuardSquad;

GuardSquad()
{
    // acá se queda vacio porque solo necesita inicializase

}

//DESTRUCTOR
~GuardSquad()
{
    for (int i = 0; i < estosMiembros.size(); i++)
    {
        delete estosMiembros[i];
    }
}

void agregarGuardia(GuardSquadMember* guardia)
{
    estosMiembros.push_back(guardia);
}

void iniciarAlerta(GuardSquadMember* origen, int nivelInicial)
{
    if (origen->nivelInicial != nullptr)
    {
        origen->recibirAlerta(nivelInicial);
    }
}