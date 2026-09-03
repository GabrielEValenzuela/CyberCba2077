//
// Created by facundo on 2/9/26.
//

#include "GuardSquad.h"

#include "GuardSquadMember.h"
//esto "envuelve" todo en el entorno del juego (contexto)
namespace cybercba::combat
{
//
GuardSquad::GuardSquad()
{
    // acá se queda vacio porque la "libreta de contactos" (dinamicArray) arranca vacia por defecto.

}

//DESTRUCTOR (limpia la ram cuando el combate termina)
GuardSquad::~GuardSquad()
{
    for (unsigned int i = 0; i < estosMiembros.size(); i++) //recorre la lista con el unsigned para que no salet el warning (preguntar profe)
    {

        // el delete va ala direción de memoria y "mata" fisicamente al guardia para devolverle ram a la compu y evitar fuga de memoria
        delete estosMiembros[i];
    }
}

//nace un guardia en la libreta del ESCUADRÓN
void GuardSquad::agregarGuardia(GuardSquadMember* guardia)
{
    estosMiembros.pushBack(guardia);
}

void GuardSquad::iniciarAlerta(GuardSquadMember* origen, int nivelInicial)

{
    // acá pregunta si el puntero origen trae una dirección de memoria real?
    if (origen != nullptr) //asegura que el guardia exista antes de mandar la orden
    {
        //la flechita va a la memoria del guarsia y le tira nivel de alerta por la cabeza
        origen->RecibirAlerta(nivelInicial);
    }
}
}