//
// Created by xbm on 8/31/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_LISTA_H
#define CYBERPUNK_CORDOBA_2077_LISTA_H
#include <string>
#include "progress_tracking/IAchievementCondition.hpp"


struct Nodo
{
    std::string nombreDelLogro;
    IAchievementCondition* desbloqueado;
    Nodo* siguiente;

};

class Lista
{
    public:
    Lista() //constructor
    {
        head=nullptr;
    }
    ~Lista() //destructor
    {

        Nodo* actual=head;

        while (actual!=nullptr)
        {
            Nodo* siguiente= actual->siguiente;
            delete actual;
            actual=siguiente;
        }
        head=nullptr;
    }

    //METODOS:
    Nodo* obtenerHead() const
    {
        return head;
    };

    void agregar(IAchievementCondition* condicion, const std::string& nombreDelLogro);


    private:
    Nodo* head;

};

#endif // CYBERPUNK_CORDOBA_2077_LISTA_H
