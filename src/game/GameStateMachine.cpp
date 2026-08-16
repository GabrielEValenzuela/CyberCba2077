#include "cybercba/game/GameStateMachine.hpp"

#include <cstddef>

namespace cybercba::game
{

namespace
{

struct TransitionEdge
{
    GameStateId from;
    GameStateId to;
};

// VS-001 §2 flow: MainMenu -> CharacterSelection -> Prologue -> CentralMap ->
// Mission -> {Puzzle, Combat} -> Mission -> PostMission -> CentralMap.
constexpr TransitionEdge ALLOWED_EDGES[] = {
    {GameStateId::MainMenu, GameStateId::CharacterSelection},
    {GameStateId::CharacterSelection, GameStateId::Prologue},
    {GameStateId::Prologue, GameStateId::CentralMap},
    {GameStateId::CentralMap, GameStateId::Mission},
    {GameStateId::Mission, GameStateId::Puzzle},
    {GameStateId::Mission, GameStateId::Combat},
    {GameStateId::Mission, GameStateId::PostMission},
    {GameStateId::Puzzle, GameStateId::Mission},
    {GameStateId::Combat, GameStateId::Mission},
    {GameStateId::PostMission, GameStateId::CentralMap},
    {GameStateId::CentralMap, GameStateId::Ending},

    // Meta/out-of-fiction credits screen (not part of GPD's narrative flow).
    {GameStateId::MainMenu, GameStateId::Credits},
    {GameStateId::Credits, GameStateId::MainMenu},
};

constexpr std::size_t ALLOWED_EDGE_COUNT = sizeof(ALLOWED_EDGES) / sizeof(ALLOWED_EDGES[0]);

} // namespace

GameStateMachine::GameStateMachine(GameStateId initial) : m_current(initial), m_prePauseState(initial) {}

GameStateId GameStateMachine::currentState() const
{
    return m_current;
}

bool GameStateMachine::canTransitionTo(GameStateId target) const
{
    for (std::size_t i = 0; i < ALLOWED_EDGE_COUNT; ++i)
    {
        if (ALLOWED_EDGES[i].from == m_current && ALLOWED_EDGES[i].to == target)
        {
            return true;
        }
    }
    return false;
}

bool GameStateMachine::transitionTo(GameStateId target)
{
    if (m_isPaused || !canTransitionTo(target))
    {
        return false;
    }
    m_current = target;
    return true;
}

bool GameStateMachine::pause()
{
    if (m_isPaused || m_current == GameStateId::Pause)
    {
        return false;
    }
    m_prePauseState = m_current;
    m_current       = GameStateId::Pause;
    m_isPaused      = true;
    return true;
}

bool GameStateMachine::resume()
{
    if (!m_isPaused)
    {
        return false;
    }
    m_current  = m_prePauseState;
    m_isPaused = false;
    return true;
}

} // namespace cybercba::game
