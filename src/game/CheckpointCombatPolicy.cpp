#include "cybercba/game/CheckpointCombatPolicy.h"

namespace cybercba::game
{

bool CheckpointCombatPolicy::correspondeGuardar(GameStateId estado) const
{
    return estado == GameStateId::Combat;
}

} // namespace cybercba::game