#ifndef CYBERCBA_GAME_GAMESTATEID_H
#define CYBERCBA_GAME_GAMESTATEID_H

#include <cstdint>

namespace cybercba::game
{

/// Principal game states. Transitions go through GameStateMachine::transitionTo.
enum class GameStateId : std::uint8_t
{
    MainMenu,
    CharacterSelection,
    Prologue,
    CentralMap,
    Mission,
    Puzzle,
    Combat,
    PostMission,
    Pause,
    Ending,

    // Meta/out-of-fiction screen:
    // shows CreditsLoader's contributor entries. Reachable from MainMenu, per
    // the original repo's "credits tab".
    Credits
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_GAMESTATEID_H
