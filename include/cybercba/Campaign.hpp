#pragma once

#include <string>

namespace cybercba
{

    enum class CharacterId
    {
        None,
        Emma,
        Magga
    };
    enum class CharacterSpecialty
    {
        Hacking,
        Combat
    };
    enum class MissionState
    {
        Locked,
        Available,
        Active,
        Completed,
        Failed
    };
    enum class ObjectiveState
    {
        Inactive,
        Active,
        Completed,
        Failed
    };
    enum class PrologueStage
    {
        Shelter,
        Transmission,
        Route,
        Convergence,
        Epilogue,
        Complete
    };

    struct CharacterDefinition
    {
        CharacterId id;
        const char* displayName;
        const char* origin;
        CharacterSpecialty specialty;
        const char* abilityName;
    };

    const CharacterDefinition& characterDefinition(CharacterId id);
    CharacterId otherCharacter(CharacterId id);
    const char* characterName(CharacterId id);
    const char* characterAbility(CharacterId id);

    struct PlayerState
    {
        float health {100.0F};
        float stamina {100.0F};
        float abilityCooldown {0.0F};
        float invulnerability {0.0F};
    };

    struct NarrativeState
    {
        int trust {0};
        int unresolvedHurt {1};
        int recoveredTruth {0};
    };

    struct CampaignProgress
    {
        CharacterId selectedCharacter {CharacterId::None};
        MissionState prologue {MissionState::Available};
        ObjectiveState objective {ObjectiveState::Inactive};
        PrologueStage stage {PrologueStage::Shelter};
        bool prologueStarted {false};
        bool prologueCompleted {false};
        bool checkpointReached {false};
        bool neometroUnlocked {false};
        std::string checkpoint {"shelter"};
    };

    enum class AbilityResult
    {
        Success,
        InvalidTarget,
        CoolingDown,
        InsufficientStamina,
        Cancelled
    };

    class AbilitySystem
    {
    public:
        static bool hasHacking(CharacterId character);
        static bool hasCombat(CharacterId character);
        static AbilityResult hack(PlayerState& player, bool targetAvailable, float dt = 0.0F);
        static AbilityResult strike(PlayerState& player, bool targetAvailable, bool heavy = false);
        static bool applyDamage(PlayerState& player, float damage);
        static void update(PlayerState& player, float dt);
    };

    class MissionSystem
    {
    public:
        static bool startPrologue(CampaignProgress& progress, CharacterId character);
        static bool advance(CampaignProgress& progress,
                            PrologueStage expected,
                            PrologueStage next,
                            const std::string& checkpoint = "");
        static bool complete(CampaignProgress& progress);
    };

} // namespace cybercba
