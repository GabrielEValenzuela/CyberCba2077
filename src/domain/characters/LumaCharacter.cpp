#include "cybercba/characters/LumaCharacter.hpp"

namespace cybercba::characters
{

    std::string LumaCharacter::id() const { return "luma"; }

    std::string LumaCharacter::displayName() const { return "Nora \"Luma\" Pereyra"; }

    CharacterRole LumaCharacter::role() const { return CharacterRole::NarrativeVoice; }

    void LumaCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        // TODO(student): replace with the real blackout flashback lines
        // ("Emma, tenemos que movernos." / "Una clave no es una persona." /
        // "No sabes quien la escribio.") from CYBERCBA 2077.md §7.
        graph.addLine({"npc_luma_placeholder", DialogueSpeaker::Npc, "Luma",
                       "[TODO(student): dialogo de Luma sin escribir todavia.]", "", "", "", "", {}});
    }

    EncounterParticipant LumaCharacter::buildParticipant() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

    std::vector<ActionDefinition> LumaCharacter::buildActions() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

} // namespace cybercba::characters
