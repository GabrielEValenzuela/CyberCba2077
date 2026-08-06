#pragma once

#include "cybercba/CharacterModule.hpp"

namespace cybercba::characters
{

    // Emma's obstacle at "security_control" ("Camaras cubren el control de
    // acceso.", CYBERCBA 2077.md §7) - today only flavor text in
    // GameApp::nodeBody(), with no encounter of its own. EncounterOpponent role.
    //
    // TODO(student): design a real encounter for this obstacle (see
    // "Pendientes no bloqueantes" in docs/roadmap.md) and wire it into the
    // security_control node.
    class SistemaVigilanciaCharacter final : public ICharacterModule
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
