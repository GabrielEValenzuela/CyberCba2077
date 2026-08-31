#ifndef CYBERCBA2077_ICHECKPOINTPOLICY_H
#define CYBERCBA2077_ICHECKPOINTPOLICY_H

#include "cybercba/game/GameStateId.hpp"

namespace cybercba::game
{

class ICheckpointPolicy
{
public:
    virtual ~ICheckpointPolicy() = default;

    virtual bool correspondeGuardar(GameStateId estado) const = 0;
};

} // namespace cybercba::game

#endif // CYBERCBA2077_ICHECKPOINTPOLICY_H
