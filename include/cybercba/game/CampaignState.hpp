#ifndef CYBERCBA_GAME_CAMPAIGNSTATE_H
#define CYBERCBA_GAME_CAMPAIGNSTATE_H

#include <cstdint>

namespace cybercba::game
{

// Which protagonist the player chose (GPD §10). Permanent for the campaign.
enum class PlayerCharacter : std::uint8_t
{
    Emma,
    Magga
};

// Outcome of the prologue's climactic combat (VS-001 §3.5). Fantasma is
// captured either way — this flag records the tactical detail, not whether
// the capture happens.
enum class MissionOutcome : std::uint8_t
{
    Undetermined,
    Success,
    Captured
};

// Campaign-wide narrative and resource flags for VS-001 (§8, §11). In-memory
// only — no persistence to disk in this slice (TAD-001 §53).
//
// POD data holder: fields intentionally have no m_ prefix (ADR-001 exception
// for structs whose fields are read/written directly, without invariants
// enforced by member functions).
//
// Ownership: value type, owned by whoever holds it (Game). No dynamic memory.
struct CampaignState
{
    PlayerCharacter player{PlayerCharacter::Emma};

    // Prologue flags (VS-001 §3).
    bool alarmTriggered{};
    bool intelComplete{};
    int guardCountModifier{};
    MissionOutcome missionOutcome{MissionOutcome::Undetermined};

    // Resources (VS-001 §8).
    int empCharges{1};
    int coverBonus{};

    bool hasEmpCharge() const
    {
        return empCharges > 0;
    }

    void consumeEmpCharge();
    void grantCoverBonus(int amount);
};

} // namespace cybercba::game

#endif // CYBERCBA_GAME_CAMPAIGNSTATE_H
