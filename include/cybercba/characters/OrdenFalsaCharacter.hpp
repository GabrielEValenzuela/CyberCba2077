#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // The fabricated order Magga receives during the blackout, signed with
    // Emma's key ("Redirigir equipo al corredor siete. Evacuacion segura.",
    // CYBERCBA 2077.md §7). NarrativeVoice role.
    //
    // TODO(student): write the fabricated-order lines.
    class OrdenFalsaCharacter final : public ICharacterModule
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
