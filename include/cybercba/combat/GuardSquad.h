//
// Created by facundo on 2/9/26.
//

#ifndef ALGORITMOS_Y_ESTRUCTURAS_DE_DATOS_GUARDSQUAD_H
#define ALGORITMOS_Y_ESTRUCTURAS_DE_DATOS_GUARDSQUAD_H
#include "../structures/DynamicArray.hpp"

namespace cybercba::combat
{

    using namespace cybercba::structures;
    class GuardSquad
    {
        DynamicArray<GuardSquadMember*> estosMiembros;

    public:
        GuardSquad();
        ~GuardSquad(); // Destructor para limpiar la memoria

        void agregarGuardia(GuardSquadMember* guardia);
        void iniciarAlerta(GuardSquadMember* origen, int nivelInicial);


    };

}







#endif // ALGORITMOS_Y_ESTRUCTURAS_DE_DATOS_GUARDSQUAD_H
