#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // The relay Emma hacks during the "protagonist_encounter" node
    // (`EncounterContent::buildEmmaHackingEncounter`). EncounterOpponent role.
    //
    // TODO(student): design the relay's stats/actions and, once ready, retire
    // the inline enemy participant in src/domain/EncounterContent.cpp in
    // favor of this module's buildParticipant()/buildActions() (see
    // docs/adding-a-character.md).
    class ReleInterferenciaCharacter final : public ICharacterModule
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
