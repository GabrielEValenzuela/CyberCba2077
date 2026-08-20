#ifndef CYBERCBA_PLATFORM_INPUTADAPTER_H
#define CYBERCBA_PLATFORM_INPUTADAPTER_H

#include "cybercba/states/InputSnapshot.hpp"

namespace cybercba::platform
{

// Translates raylib polling (IsKeyPressed/etc.) into an InputSnapshot
// (TAD-001 §18). This is the only place in the codebase allowed to call
// raylib input functions — cybercba_core never does.
//
// Ownership: stateless adapter, no dynamic memory.
class InputAdapter final
{
  public:
    states::InputSnapshot capture() const;
};

} // namespace cybercba::platform

#endif // CYBERCBA_PLATFORM_INPUTADAPTER_H
