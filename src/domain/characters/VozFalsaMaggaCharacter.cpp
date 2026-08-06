#include "cybercba/characters/VozFalsaMaggaCharacter.hpp"

namespace cybercba::characters
{

    std::string VozFalsaMaggaCharacter::id() const { return "voz_falsa_magga"; }

    std::string VozFalsaMaggaCharacter::displayName() const { return "Voz falsa de Magga"; }

    CharacterRole VozFalsaMaggaCharacter::role() const { return CharacterRole::NarrativeVoice; }

    void VozFalsaMaggaCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        // TODO(student): replace with the real fabricated-transmission line
        // ("Confirmo ubicacion. Sector oeste. Acceso por mantenimiento.")
        // from CYBERCBA 2077.md §7.
        graph.addLine({"npc_voz_falsa_magga_placeholder", DialogueSpeaker::Npc, "Voz falsa de Magga",
                       "[TODO(student): linea de la voz fabricada sin escribir todavia.]", "", "", "", "", {}});
    }

    EncounterParticipant VozFalsaMaggaCharacter::buildParticipant() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

    std::vector<ActionDefinition> VozFalsaMaggaCharacter::buildActions() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

} // namespace cybercba::characters
