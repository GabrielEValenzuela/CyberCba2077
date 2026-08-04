#pragma once

#include <cstdint>
#include <string>

#include "cybercba/Campaign.hpp"
#include "cybercba/Evidence.hpp"
#include "cybercba/MissionGraph.hpp"
#include "model/GameModel.hpp"

namespace cybercba
{

    struct AccessibilitySettings
    {
        bool reducedMotion {false};
        bool scanlines {true};
        bool subtitles {true};
        bool highContrast {false};
        bool reduceFlashes {false};
        bool screenShake {true};
        bool persistentPrompts {false};
        float uiScale {1.0F};
        float dialogueSpeed {1.0F};
    };
    struct AudioSettings
    {
        bool muted {false};
        float masterVolume {1.0F};
        float musicVolume {0.65F};
        float ambienceVolume {0.70F};
        float dialogueVolume {0.85F};
        float effectsVolume {0.80F};
    };
    struct PlayerProgress
    {
        bool hasSave {false};
        bool tutorialCompleted {false};
        bool queueMissionCompleted {false};
        int bestQueueAccuracy {0};
        int unlockedMission {1};
        std::string profileId {"operator"};
    };

    class GameSession
    {
    public:
        cyberpunk::GameModel& model();
        const cyberpunk::GameModel& model() const;
        PlayerProgress& progress();
        const PlayerProgress& progress() const;
        AccessibilitySettings& accessibility();
        const AccessibilitySettings& accessibility() const;
        AudioSettings& audio();
        const AudioSettings& audio() const;
        void startNewGame();
        void recordQueueResult(int accuracyPercent, std::int64_t credits);
        CampaignProgress& campaign();
        const CampaignProgress& campaign() const;
        PlayerState& player();
        const PlayerState& player() const;
        NarrativeState& narrative();
        const NarrativeState& narrative() const;
        bool startPrologue(CharacterId character);
        void reachCheckpoint(const std::string& id);
        bool resetToCheckpoint();

        MissionGraph& missionGraph();
        const MissionGraph& missionGraph() const;
        EvidenceJournal& evidenceJournal();
        const EvidenceJournal& evidenceJournal() const;

    private:
        cyberpunk::GameModel m_model;
        PlayerProgress m_progress;
        AccessibilitySettings m_accessibility;
        AudioSettings m_audio;
        CampaignProgress m_campaign;
        PlayerState m_player;
        NarrativeState m_narrative;
        MissionGraph m_missionGraph;
        EvidenceJournal m_evidenceJournal;
    };

} // namespace cybercba
