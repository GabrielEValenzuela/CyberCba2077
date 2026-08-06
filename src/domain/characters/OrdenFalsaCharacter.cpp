#include "cybercba/characters/OrdenFalsaCharacter.hpp"

namespace cybercba::characters
{

    std::string OrdenFalsaCharacter::id() const { return "orden_falsa"; }

    std::string OrdenFalsaCharacter::displayName() const { return "Orden falsa (firma de Emma)"; }

    CharacterRole OrdenFalsaCharacter::role() const { return CharacterRole::NarrativeVoice; }

    void OrdenFalsaCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        // TODO(student): replace with the real fabricated-order line
        // ("Redirigir equipo al corredor siete. Evacuacion segura.") from
        // CYBERCBA 2077.md §7.
        graph.addLine({"npc_orden_falsa_placeholder", DialogueSpeaker::Npc, "Orden falsa",
                       "[TODO(student): linea de la orden fabricada sin escribir todavia.]", "", "", "", "", {}});
    }

    EncounterParticipant OrdenFalsaCharacter::buildParticipant() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

    std::vector<ActionDefinition> OrdenFalsaCharacter::buildActions() const
    {
        return {}; // Not an encounter opponent - role() is NarrativeVoice.
    }

} // namespace cybercba::characters
