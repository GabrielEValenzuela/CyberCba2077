#include "cybercba/characters/TomoCharacter.hpp"

namespace cybercba::characters
{

    std::string TomoCharacter::id() const { return "tomo"; }

    std::string TomoCharacter::displayName() const { return "Tomás \"Tomo\" Ferreyra"; }

    CharacterRole TomoCharacter::role() const { return CharacterRole::NarrativeVoice; }

    void TomoCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        // TODO(student): replace with Tomo's real prologue cameo line(s) at
        // the Neometro entrance (CYBERCBA 2077.md §7).
        graph.addLine({"npc_tomo_placeholder", DialogueSpeaker::Npc, "Tomo",
                       "[TODO(student): cameo de Tomo sin escribir todavia.]", "", "", "", "", {}});
    }

    EncounterParticipant TomoCharacter::buildParticipant() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

    std::vector<ActionDefinition> TomoCharacter::buildActions() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

} // namespace cybercba::characters
