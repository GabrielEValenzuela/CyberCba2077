#include "cybercba/characters/PatrullaSeguridadCharacter.hpp"

namespace cybercba::characters
{

    std::string PatrullaSeguridadCharacter::id() const { return "patrulla_seguridad"; }

    std::string PatrullaSeguridadCharacter::displayName() const { return "[TODO] Patrulla de seguridad"; }

    CharacterRole PatrullaSeguridadCharacter::role() const { return CharacterRole::EncounterOpponent; }

    void PatrullaSeguridadCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        (void)graph; // Not a narrative voice - role() is EncounterOpponent.
    }

    EncounterParticipant PatrullaSeguridadCharacter::buildParticipant() const
    {
        // TODO(student): design the patrol obstacle Magga faces at
        // security_control (see docs/roadmap.md "Pendientes no bloqueantes").
        return {id(), displayName(), false, 100, 100, 0, "momentum", 0, false, {}};
    }

    std::vector<ActionDefinition> PatrullaSeguridadCharacter::buildActions() const
    {
        // TODO(student): design the patrol's real action set.
        return {{"todo_placeholder_action", "[TODO] Accion sin definir", {"", 0}, 0, "", 0}};
    }

} // namespace cybercba::characters
