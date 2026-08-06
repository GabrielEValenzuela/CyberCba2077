#include "cybercba/characters/DronSeguridadCharacter.hpp"

namespace cybercba::characters
{

    std::string DronSeguridadCharacter::id() const { return "dron_seguridad"; }

    std::string DronSeguridadCharacter::displayName() const { return "[TODO] Dron de seguridad"; }

    CharacterRole DronSeguridadCharacter::role() const { return CharacterRole::EncounterOpponent; }

    void DronSeguridadCharacter::contributeDialogue(DialogueGraph& graph) const
    {
        (void)graph; // Not a narrative voice - role() is EncounterOpponent.
    }

    EncounterParticipant DronSeguridadCharacter::buildParticipant() const
    {
        // TODO(student): tune integrity/resource/initiative and cross-check
        // against buildMaggaCombatEncounter() in src/domain/EncounterContent.cpp.
        return {id(), displayName(), false, 100, 100, 0, "momentum", 0, false, {}};
    }

    std::vector<ActionDefinition> DronSeguridadCharacter::buildActions() const
    {
        // TODO(student): design the drone's real action set.
        return {{"todo_placeholder_action", "[TODO] Accion sin definir", {"", 0}, 0, "", 0}};
    }

} // namespace cybercba::characters
