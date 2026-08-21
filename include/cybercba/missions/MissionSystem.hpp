#ifndef CYBERCBA_MISSIONS_MISSIONSYSTEM_H
#define CYBERCBA_MISSIONS_MISSIONSYSTEM_H

#include <cstddef>
#include <cstdint>

#include "cybercba/missions/DialogueSystem.hpp"

namespace cybercba::missions
{

// What a scene actually requires from the player (VS-001 §3/§5: prologue
// P1-P6, mission 1 M1-M4). Puzzle/Combat scenes are resolved by whichever
// system owns that gameplay (PuzzleSystem/CombatSystem) — MissionSystem only
// tracks *that* a scene of that type is active, not the concrete puzzle or
// combat instance, so it stays link-safe even while PuzzleSystem depends on
// unimplemented structures (VS-001 §12.2).
enum class SceneType : std::uint8_t
{
    Dialogue,
    Puzzle,
    Combat,
};

/// Static, immutable scene data. pDialogue is required when
// type == Dialogue and ignored otherwise.
struct SceneDefinition
{
    SceneType type;
    const DialogueScript* pDialogue{nullptr};
};

// Static, immutable mission data — the ordered scene list for the Prologue
// or Mission 1 (VS-001 §3, §5).
struct MissionDefinition
{
    const SceneDefinition* pScenes;
    std::size_t sceneCount;
};

/// Orchestrates the linear scene sequence within one mission.
// VS-001 §3/§5). Dialogue scenes are driven internally via DialogueSystem;
// Puzzle/Combat scenes are resolved externally (by whoever owns the actual
// PuzzleSystem/CombatSystem instance) and reported back via
// completeCurrentScene().
//
// Ownership: does not own the MissionDefinition (caller keeps mission data,
// e.g. static storage, alive for this system's lifetime). Owns its internal
// DialogueSystem by value.
class MissionSystem final
{
  public:
    void begin(const MissionDefinition& mission);

    bool isFinished() const;
    SceneType currentSceneType() const;

    // Valid only while currentSceneType() == Dialogue.
    const DialogueLine& currentDialogueLine() const;
    bool hasDialogueChoices() const;

    // Advances dialogue by one line; if that finishes the scene's script,
    // automatically advances to the next scene (VS-001 §3: consecutive
    // dialogue scenes flow without an extra player action). Returns false if
    // the current scene has choices pending (call selectDialogueChoice
    // instead) or the mission is already finished.
    bool advanceDialogue();
    bool selectDialogueChoice(std::size_t choiceIndex);

    // Called once a Puzzle/Combat scene has been resolved externally.
    // Returns false if the current scene is not Puzzle/Combat, or the
    // mission is already finished.
    bool completeCurrentScene();

  private:
    void enterScene(std::size_t index);
    bool advanceToNextScene();

    const MissionDefinition* m_pMission{nullptr};
    std::size_t m_currentSceneIndex{};
    DialogueSystem m_dialogueSystem;
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_MISSIONSYSTEM_H
