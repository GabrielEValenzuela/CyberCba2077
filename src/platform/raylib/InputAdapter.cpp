#include "cybercba/platform/InputAdapter.hpp"

#include <raylib.h>

namespace cybercba::platform
{

states::InputSnapshot InputAdapter::capture() const
{
    states::InputSnapshot snapshot;
    snapshot.confirm   = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
    snapshot.cancel    = IsKeyPressed(KEY_ESCAPE);
    snapshot.moveUp    = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    snapshot.moveDown  = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    snapshot.moveLeft  = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    snapshot.moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    return snapshot;
}

} // namespace cybercba::platform
