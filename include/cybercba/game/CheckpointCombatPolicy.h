#ifndef CYBERCBA2077_CHECKPOINTCOMBATPOLICY_H
#define CYBERCBA2077_CHECKPOINTCOMBATPOLICY_H

#include "cybercba/game/ICheckpointPolicy.h"

namespace cybercba::game
{

class CheckpointCombatPolicy final : public ICheckpointPolicy
{
public:
    bool correspondeGuardar(GameStateId estado) const override;
};

} // namespace cybercba::game

#endif // CYBERCBA2077_CHECKPOINTCOMBATPOLICY_H