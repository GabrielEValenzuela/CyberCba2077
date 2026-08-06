#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // The security drone Magga fights during the "protagonist_encounter"
    // node (`EncounterContent::buildMaggaCombatEncounter`). EncounterOpponent role.
    //
    // TODO(student): design the drone's stats/actions and, once ready, retire
    // the inline enemy participant in src/domain/EncounterContent.cpp in
    // favor of this module's buildParticipant()/buildActions() (see
    // docs/adding-a-character.md).
    class DronSeguridadCharacter final : public ICharacterModule
    {
    public:
        std::string id() const override;
        std::string displayName() const override;
        CharacterRole role() const override;

        void contributeDialogue(DialogueGraph& graph) const override;

        EncounterParticipant buildParticipant() const override;
        std::vector<ActionDefinition> buildActions() const override;
    };

} // namespace cybercba::characters
