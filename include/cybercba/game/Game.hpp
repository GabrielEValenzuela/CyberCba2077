#ifndef CYBERCBA_GAME_GAME_H
#define CYBERCBA_GAME_GAME_H

#include <string>

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/game/GameStateMachine.hpp"
#include "cybercba/states/InputSnapshot.hpp"

namespace cybercba::game
{

/// Composition root for gameplay state.
///
/// The raylib application drives this object with platform-independent input.
//
// Full scene/mission content (MissionSystem wired to the
// actual Prologue/Mission 1 SceneDefinitions) is not authored yet — that is
// narrative-data work, not architecture. This class currently only proves
// the MainMenu -> CharacterSelection -> CentralMap skeleton transitions
// so the app has a real, testable loop to run.
//
// Ownership: owns CampaignState and GameStateMachine by value. No dynamic
// memory.
class Game final
{
  public:
    /// `creditsDirectory` is passed in by the composition root so
    // Game stays free of raylib but still doesn't hardcode a filesystem path
    // — the app decides where "credits/" lives relative to the executable.
    explicit Game(const std::string& creditsDirectory = "credits");

    void update(const states::InputSnapshot& input);

    GameStateId currentState() const;
    const CampaignState& campaign() const;

    // Contributor entries loaded once at construction via CreditsLoader
    // (credits/*.txt, see credits/README.md). Empty if the directory is
    // missing or has no entries — Renderer must handle that gracefully.
    const std::string& creditsText() const;

  private:
    GameStateMachine m_stateMachine;
    CampaignState m_campaign;
    std::string m_creditsText;
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_GAME_H
