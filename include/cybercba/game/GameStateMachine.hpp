#ifndef CYBERCBA_GAME_GAMESTATEMACHINE_H
#define CYBERCBA_GAME_GAMESTATEMACHINE_H

#include "cybercba/game/GameStateId.hpp"

namespace cybercba::game
{

/// Explicit state machine for GameStateId.
///
/// It owns the current state and the table of legal transitions.
//
// Ownership: value type, no dynamic memory. Embedded by value inside Game.
class GameStateMachine final
{
  public:
    explicit GameStateMachine(GameStateId initial = GameStateId::MainMenu);

    GameStateId currentState() const;

    // Returns true and applies the transition if (current -> target) is a
    // legal edge per the transition table; returns false and leaves state
    // unchanged otherwise; callers must check the return value.
    bool transitionTo(GameStateId target);

    bool canTransitionTo(GameStateId target) const;

    // Pauses from any state (Pause is not itself pausable) and remembers the
    // state to return to; resume() reverses that. Returns false if misused
    // (pausing while already paused, or resuming while not paused).
    bool pause();
    bool resume();

  private:
    GameStateId m_current;
    GameStateId m_prePauseState;
    bool m_isPaused{false};
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_GAMESTATEMACHINE_H
