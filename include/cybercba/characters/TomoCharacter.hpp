#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // Tomás "Tomo" Ferreyra - Neometro operator, briefly foreshadowed at the
    // prologue's Neometro entrance ("Se escucha un tren bajo tierra.",
    // CYBERCBA 2077.md §7); his full role begins in v0.2.0. NarrativeVoice role.
    //
    // TODO(student): write Tomo's cameo line(s) for the prologue only - his
    // v0.2.0 dialogue belongs in a future NeometroContent.cpp, not here (see
    // docs/adding-a-mission.md).
    class TomoCharacter final : public ICharacterModule
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
