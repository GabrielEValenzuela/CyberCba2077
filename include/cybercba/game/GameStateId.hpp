#ifndef CYBERCBA_GAME_GAMESTATEID_H
#define CYBERCBA_GAME_GAMESTATEID_H

#include <cstdint>

namespace cybercba::game
{

// Principal game states (TAD-001 §16, GPD §57). Transitions between them MUST
// go through GameStateMachine::transitionTo — never through scattered bool flags.
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

    // Meta/out-of-fiction screen (not part of GPD's narrative state machine):
    // shows CreditsLoader's contributor entries. Reachable from MainMenu, per
    // the original repo's "credits tab".
    Credits
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_GAMESTATEID_H
