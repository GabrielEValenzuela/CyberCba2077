//
// Created by LaPobreza on 29/8/26.
//

#ifndef CYBERCBA2077_CHECKPOINT_H
#define CYBERCBA2077_CHECKPOINT_H
#include "cybercba/game/CampaignState.hpp"

namespace cybercba::game
{
//Un punto de control es un tipo de foto del progreso del jugador, apuntando al punto anterior formando una cadena hacia atras.
class Checkpoint
{
public:
    //Declaro el constructor con sus parametros inciales, el estado y si hay algun punto previo de checkpoint
    Checkpoint(const CampaingState& estado, Checkpoint* previo = nullptr);

    //Devuelve la informacion del estado de la partida solo para lectura
    const CampaingState& estado() const;

    //Devuelve un puntero al checkpoint anterior
    Checkpoint* previo() const;

private:
    CampaignState m_estado; // Variable de tipo CampaignState que guarda toda la informacion y progreso del jugador
    Checkpoint* m_previo; // Puntero que guarda la informacion del Checkpoint anterior

};
}
#endif // CYBERCBA2077_CHECKPOINT_H
