#include "cybercba/characters/SistemaVigilanciaCharacter.hpp"

namespace cybercba::characters
{

    std::string SistemaVigilanciaCharacter::id() const { return "sistema_vigilancia"; }

    std::string SistemaVigilanciaCharacter::displayName() const { return "[TODO] Sistema de vigilancia"; }

    CharacterRole SistemaVigilanciaCharacter::role() const { return CharacterRole::EncounterOpponent; }

    void SistemaVigilanciaCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        (void)graph; // Not a narrative voice - role() is EncounterOpponent.
    }

    EncounterParticipant SistemaVigilanciaCharacter::buildParticipant() const
    {
        // TODO(student): design the surveillance obstacle Emma faces at
        // security_control (see docs/roadmap.md "Pendientes no bloqueantes").
        return {id(), displayName(), false, 100, 100, 0, "trace", 0, false, {}};
    }

    std::vector<ActionDefinition> SistemaVigilanciaCharacter::buildActions() const
    {
        // TODO(student): design the surveillance system's real action set.
        return {{"todo_placeholder_action", "[TODO] Accion sin definir", {"", 0}, 0, "", 0}};
    }

} // namespace cybercba::characters
