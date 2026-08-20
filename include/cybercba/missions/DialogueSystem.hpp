#ifndef CYBERCBA_MISSIONS_DIALOGUESYSTEM_H
#define CYBERCBA_MISSIONS_DIALOGUESYSTEM_H

#include <cstddef>
#include <string_view>

namespace cybercba::missions
{

// One branch a player can pick at a decision point (GPD §18, VS-001 §3.3).
// nextLineIndex is an index into the owning DialogueScript::lines.
// POD data holder: fields intentionally have no m_ prefix (ADR-001 exception).
struct DialogueChoice
{
    std::string_view text;
    std::size_t nextLineIndex;
};

// One line of dialogue (GPD §18: portrait + name + text, portrait/name
// resolved by the raylib layer via speaker). choices is empty for linear
// lines; not every line offers a decision (GPD §18).
struct DialogueLine
{
    std::string_view speaker;
    std::string_view text;
    const DialogueChoice* pChoices{nullptr};
    std::size_t choiceCount{};
};

// Static, immutable script data (TAD-001 §21). Owned by whichever
// SceneDefinition declares it (constexpr storage duration); DialogueSystem
// only observes it.
struct DialogueScript
{
    const DialogueLine* pLines;
    std::size_t lineCount;
};

// Advances a DialogueScript line by line, pausing at lines with choices.
// Testable without raylib (TAD-001 §68): no rendering, no input polling.
//
// Ownership: DialogueSystem does not own the script; the caller must keep the
// DialogueScript (and its backing arrays) alive for the system's lifetime.
class DialogueSystem final
{
  public:
    void begin(const DialogueScript& script);

    const DialogueLine& currentLine() const;
    bool hasChoices() const;
    bool isFinished() const;

    // Advances to the next line in sequence. Returns false if the current
    // line has choices (must call selectChoice instead) or if already
    // finished.
    bool advance();

    // Jumps to the target of choices[choiceIndex]. Returns false if the
    // current line has no such choice.
    bool selectChoice(std::size_t choiceIndex);

  private:
    const DialogueScript* m_pScript{nullptr};
    std::size_t m_currentIndex{};
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_DIALOGUESYSTEM_H
