//
// Created by xbm on 8/31/26.
//

#include "Lista.hpp"
void Lista::agregar(IAchievementCondition* condicion, const std::string& nombreDelLogro)
{
    //constructor de un nodo
    Nodo* nuevoNodo=new Nodo();
    nuevoNodo->nombreDelLogro=nombreDelLogro;
    nuevoNodo->desbloqueado=condicion;
    nuevoNodo->siguiente=nullptr;

    //si la lista esta vacia, solo lo agrega
    if (head==nullptr)
    {
        head=nuevoNodo;
    }
    else
    {
        Nodo* actual=head;
        while (actual->siguiente!=nullptr)
        {
            actual=actual->siguiente;
        }
        actual->siguiente=nuevoNodo;

    }




}