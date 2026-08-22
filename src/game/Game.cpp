#include "cybercba/game/Game.hpp"

#include "cybercba/core/CreditsLoader.hpp"

namespace cybercba::game
{

Game::Game(const std::string& creditsDirectory) : m_creditsText(core::CreditsLoader::loadEntries(creditsDirectory)) {}

void Game::update(const states::InputSnapshot& input)
{
    if (input.cancel)
    {
        // Credits is a meta/out-of-fiction screen reachable from MainMenu and
        // returns to it,
        // toggled on the input that is otherwise unused there.
        if (m_stateMachine.currentState() == GameStateId::MainMenu)
        {
            m_stateMachine.transitionTo(GameStateId::Credits);
        }
        else if (m_stateMachine.currentState() == GameStateId::Credits)
        {
            m_stateMachine.transitionTo(GameStateId::MainMenu);
        }
        return;
    }

    if (!input.confirm)
    {
        return;
    }

    switch (m_stateMachine.currentState())
    {
    case GameStateId::MainMenu:
        m_stateMachine.transitionTo(GameStateId::CharacterSelection);
        break;
    case GameStateId::CharacterSelection:
        m_stateMachine.transitionTo(GameStateId::Prologue);
        break;
    case GameStateId::Prologue:
        m_stateMachine.transitionTo(GameStateId::CentralMap);
        break;
    default:
        break;
    }
}

GameStateId Game::currentState() const
{
    return m_stateMachine.currentState();
}

const CampaignState& Game::campaign() const
{
    return m_campaign;
}

const std::string& Game::creditsText() const
{
    return m_creditsText;
}

} // namespace cybercba::game
