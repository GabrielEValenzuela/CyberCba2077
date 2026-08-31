#ifndef CYBERCBA2077_CHECKPOINTPUZZLEPOLICY_H
#define CYBERCBA2077_CHECKPOINTPUZZLEPOLICY_H

#include "cybercba/game/ICheckpointPolicy.h"

namespace cybercba::game
{

class CheckpointPuzzlePolicy final : public ICheckpointPolicy
{
public:
    bool correspondeGuardar(GameStateId estado) const override;
};

} // namespace cybercba::game

#endif // CYBERCBA2077_CHECKPOINTPUZZLEPOLICY_H
