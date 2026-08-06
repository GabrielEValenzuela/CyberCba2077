#include "cybercba/characters/ReleInterferenciaCharacter.hpp"

namespace cybercba::characters
{

    std::string ReleInterferenciaCharacter::id() const { return "rele_interferencia"; }

    std::string ReleInterferenciaCharacter::displayName() const { return "[TODO] Relé de interferencia"; }

    CharacterRole ReleInterferenciaCharacter::role() const { return CharacterRole::EncounterOpponent; }

    void ReleInterferenciaCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        (void)graph; // Not a narrative voice - role() is EncounterOpponent.
    }

    EncounterParticipant ReleInterferenciaCharacter::buildParticipant() const
    {
        // TODO(student): tune integrity/resource/initiative and cross-check
        // against buildEmmaHackingEncounter() in src/domain/EncounterContent.cpp.
        return {id(), displayName(), false, 100, 100, 0, "trace", 0, false, {}};
    }

    std::vector<ActionDefinition> ReleInterferenciaCharacter::buildActions() const
    {
        // TODO(student): design the relay's real action set.
        return {{"todo_placeholder_action", "[TODO] Accion sin definir", {"", 0}, 0, "", 0}};
    }

} // namespace cybercba::characters
