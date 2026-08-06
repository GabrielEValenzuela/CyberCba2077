#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // The refuge's automated voice ("Fuente principal desconectada. Señal
    // entrante detectada.", CYBERCBA 2077.md §7). NarrativeVoice role.
    //
    // TODO(student): write the full set of Sistema announcements for power
    // recovery, the blackout and the transmitter sequence.
    class SistemaCharacter final : public ICharacterModule
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
