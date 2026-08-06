#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // Nora "Luma" Pereyra - the voice heard during the blackout flashback
    // ("Emma, tenemos que movernos." / "Una clave no es una persona.", see
    // CYBERCBA 2077.md §7). NarrativeVoice role: dialogue only.
    //
    // TODO(student): flesh out contributeDialogue() with the full blackout
    // flashback lines attributed to Luma and register the character's
    // portrait id once art exists (see docs/adding-a-character.md).
    class LumaCharacter final : public ICharacterModule
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
