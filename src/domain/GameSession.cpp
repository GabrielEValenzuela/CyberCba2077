#include "cybercba/GameSession.hpp"

#include <algorithm>

namespace cybercba
{
    cyberpunk::GameModel& GameSession::model()
    {
        return m_model;
    }
    const cyberpunk::GameModel& GameSession::model() const
    {
        return m_model;
    }
    PlayerProgress& GameSession::progress()
    {
        return m_progress;
    }
    const PlayerProgress& GameSession::progress() const
    {
        return m_progress;
    }
    AccessibilitySettings& GameSession::accessibility()
    {
        return m_accessibility;
    }
    const AccessibilitySettings& GameSession::accessibility() const
    {
        return m_accessibility;
    }
    AudioSettings& GameSession::audio()
    {
        return m_audio;
    }
    const AudioSettings& GameSession::audio() const
    {
        return m_audio;
    }
    void GameSession::startNewGame()
    {
        const AccessibilitySettings accessibility = m_accessibility;
        const AudioSettings audio = m_audio;
        m_model = cyberpunk::GameModel {};
        m_progress = PlayerProgress {};
        m_progress.hasSave = true;
        m_campaign = CampaignProgress {};
        m_player = PlayerState {};
        m_narrative = NarrativeState {};
        m_accessibility = accessibility;
        m_audio = audio;
    }
    void GameSession::recordQueueResult(int accuracyPercent, std::int64_t credits)
    {
        m_progress.hasSave = true;
        m_progress.tutorialCompleted = true;
        m_progress.queueMissionCompleted = true;
        m_progress.unlockedMission = std::max(m_progress.unlockedMission, 2);
        m_progress.bestQueueAccuracy = std::max(m_progress.bestQueueAccuracy, accuracyPercent);
        m_model.addCredits(credits);
    }
    CampaignProgress& GameSession::campaign()
    {
        return m_campaign;
    }
    const CampaignProgress& GameSession::campaign() const
    {
        return m_campaign;
    }
    PlayerState& GameSession::player()
    {
        return m_player;
    }
    const PlayerState& GameSession::player() const
    {
        return m_player;
    }
    NarrativeState& GameSession::narrative()
    {
        return m_narrative;
    }
    const NarrativeState& GameSession::narrative() const
    {
        return m_narrative;
    }
    bool GameSession::startPrologue(CharacterId character)
    {
        startNewGame();
        return MissionSystem::startPrologue(m_campaign, character);
    }
    void GameSession::reachCheckpoint(const std::string& id)
    {
        m_campaign.checkpoint = id;
        m_campaign.checkpointReached = true;
        m_progress.hasSave = true;
    }
    bool GameSession::resetToCheckpoint()
    {
        if (!m_campaign.prologueStarted || !m_campaign.checkpointReached)
            return false;
        if (m_campaign.checkpoint == "shelter")
            m_campaign.stage = PrologueStage::Shelter;
        else if (m_campaign.checkpoint == "transmission")
            m_campaign.stage = PrologueStage::Transmission;
        else if (m_campaign.checkpoint == "street")
            m_campaign.stage = PrologueStage::Route;
        else if (m_campaign.checkpoint == "service_gate")
            m_campaign.stage = PrologueStage::Convergence;
        else if (m_campaign.checkpoint == "neometro_gate")
            m_campaign.stage = PrologueStage::Epilogue;
        else
            return false;
        m_campaign.prologue = MissionState::Active;
        m_campaign.objective = ObjectiveState::Active;
        m_campaign.prologueCompleted = false;
        m_player.health = 100.0F;
        m_player.stamina = 100.0F;
        m_player.abilityCooldown = 0.0F;
        m_player.invulnerability = 0.0F;
        return true;
    }
} // namespace cybercba
