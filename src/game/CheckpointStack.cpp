//
// Created by alejo on 29/8/26.
//

#include "../../include/cybercba/game/CheckpointStack.h"
#include <stdexcept>

namespace cybercba::game
{

CheckpointStack::CheckpointStack(int maxSize)
    : m_maxSize(maxSize), m_tope(nullptr)
{
}

CheckpointStack::~CheckpointStack()
{
    vaciar();
}

void CheckpointStack::agregar(const CampaignState& foto)
{
    if (m_maxSize <= 0)
    {
        return;
    }

    // Se agrega el nuevo checkpoint apuntando al tope actual
    m_tope = new Checkpoint(foto, m_tope);

    // Si excede el tamaño máximo, se elimina el nodo más antiguo (el fondo)
    if (profundidad() > m_maxSize)
    {
        Checkpoint* anterior = nullptr;
        Checkpoint* actual = m_tope;

        while (actual->previo() != nullptr)
        {
            anterior = actual;
            actual = actual->previo();
        }

        if (anterior != nullptr)
        {
            delete actual;
            // Nota: si Checkpoint no tiene setter para previo,
            // podemos necesitar desvincularlo o manejarlo con una lista interna
        }
    }
}

CampaignState CheckpointStack::volverAlUltimo()
{
    if (estaVacia())
    {
        throw std::runtime_error("Pila de checkpoints vacia.");
    }

    Checkpoint* actual = m_tope;
    CampaignState estadoRecuperado = actual->estado();

    m_tope = actual->previo();
    delete actual;

    return estadoRecuperado;
}

int CheckpointStack::profundidad() const
{
    if (m_tope == nullptr)
    {
        return 0;
    }
    return contarRecursivo(m_tope);
}

// Diagrama de flujo recursivo
int CheckpointStack::contarRecursivo(const Checkpoint* nodo)
{
    if (nodo == nullptr)
    {
        return 0;
    }
    if (nodo->previo() == nullptr)
    {
        return 1; // Caso base: llegó al principio
    }
    return 1 + contarRecursivo(nodo->previo()); // Paso recursivo
}

void CheckpointStack::vaciar()
{
    while (m_tope != nullptr)
    {
        Checkpoint* aux = m_tope;
        m_tope = m_tope->previo();
        delete aux;
    }
}

bool CheckpointStack::estaVacia() const
{
    return m_tope == nullptr;
}

int CheckpointStack::limiteMaximo() const
{
    return m_maxSize;
}

} // namespace cybercba::game