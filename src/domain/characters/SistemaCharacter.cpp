#include "cybercba/characters/SistemaCharacter.hpp"

namespace cybercba::characters
{

    std::string SistemaCharacter::id() const { return "sistema"; }

    std::string SistemaCharacter::displayName() const { return "Sistema"; }

    CharacterRole SistemaCharacter::role() const { return CharacterRole::NarrativeVoice; }

    void SistemaCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        // TODO(student): replace with the real Sistema announcements
        // ("Fuente principal desconectada. Señal entrante detectada.") from
        // CYBERCBA 2077.md §7.
        graph.addLine({"npc_sistema_placeholder", DialogueSpeaker::Npc, "Sistema",
                       "[TODO(student): anuncios del Sistema sin escribir todavia.]", "", "", "", "", {}});
    }

    EncounterParticipant SistemaCharacter::buildParticipant() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

    std::vector<ActionDefinition> SistemaCharacter::buildActions() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

} // namespace cybercba::characters
