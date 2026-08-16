#ifndef CYBERCBA_STATES_INPUTSNAPSHOT_H
#define CYBERCBA_STATES_INPUTSNAPSHOT_H

namespace cybercba::states
{

// Raylib input translated to a platform-independent snapshot (TAD-001 §18).
// Gameplay code only ever sees this struct, never raylib's IsKeyPressed()/etc.
// POD data holder: member variables intentionally have no m_ prefix (ADR-001
// exception for POD structs with public constant-role fields).
struct InputSnapshot
{
    bool confirm{};
    bool cancel{};
    bool moveUp{};
    bool moveDown{};
    bool moveLeft{};
    bool moveRight{};
};

} // namespace cybercba::states

#endif // CYBERCBA_STATES_INPUTSNAPSHOT_H
