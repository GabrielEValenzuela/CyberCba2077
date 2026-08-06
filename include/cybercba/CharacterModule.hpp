#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cybercba/Dialogue.hpp"
#include "cybercba/Encounter.hpp"

namespace cybercba
{

    enum class CharacterRole
    {
        NarrativeVoice,   // contributes dialogue lines only (e.g. Luma, Tomo)
        EncounterOpponent // contributes a non-player participant + its actions
    };

    // Contract each student implements for one of the nine v0.1.0 NPC/enemy
    // roles - see docs/adding-a-character.md. A module contributes to exactly
    // one system, matching role(): dialogue for NarrativeVoice, an Encounter
    // participant/actions for EncounterOpponent. The unused half of the
    // contract must still compile (return an empty/no-op result) but is never
    // called by the roster for that role.
    class ICharacterModule
    {
    public:
        virtual ~ICharacterModule() = default;

        virtual std::string id() const = 0;
        virtual std::string displayName() const = 0;
        virtual CharacterRole role() const = 0;

        // Called only when role() == NarrativeVoice. Adds this character's
        // lines to the shared prologue dialogue graph.
        virtual void contributeDialogue(DialogueGraph& graph) const = 0;

        // Called only when role() == EncounterOpponent.
        virtual EncounterParticipant buildParticipant() const = 0;
        virtual std::vector<ActionDefinition> buildActions() const = 0;
    };

    using CharacterModulePtr = std::unique_ptr<ICharacterModule>;

    // Collects the nine v0.1.0 character modules. Building the roster does
    // not wire modules into MissionGraph/GameApp - that integration is a
    // separate, later step (see docs/adding-a-character.md "Pendiente").
    class CharacterRoster
    {
    public:
        void add(CharacterModulePtr module);
        const std::vector<CharacterModulePtr>& modules() const;
        const ICharacterModule* find(const std::string& id) const;

    private:
        std::vector<CharacterModulePtr> m_modules;
    };

    // Builds the roster with the nine placeholder modules for v0.1.0. Each
    // module is a TODO(student) stub - see docs/adding-a-character.md.
    CharacterRoster buildPrologueCharacterRoster();

} // namespace cybercba
