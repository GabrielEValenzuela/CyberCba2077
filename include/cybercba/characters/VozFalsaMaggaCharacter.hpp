#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // The fabricated evidence voice Emma receives during the blackout
    // ("Confirmo ubicacion. Sector oeste. Acceso por mantenimiento.",
    // CYBERCBA 2077.md §7). NarrativeVoice role.
    //
    // TODO(student): write the fabricated-voice lines and make sure they are
    // distinguishable in tone from Magga's real lines (see docs/dialogue-system.md).
    class VozFalsaMaggaCharacter final : public ICharacterModule
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
