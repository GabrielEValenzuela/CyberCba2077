#pragma once

#include <string>
#include <vector>

#include "cybercba/Campaign.hpp"

namespace cybercba
{

    enum class DialogueSpeaker
    {
        Narrator,
        Protagonist,
        OtherProtagonist,
        Npc
    };

    struct DialogueChoice
    {
        std::string id;
        std::string text;
        std::string nextLineId; // empty = ends the dialogue
        std::string setFlag;    // narrative/mission flag set when this choice is taken
    };

    struct DialogueLine
    {
        std::string id;
        DialogueSpeaker speaker {DialogueSpeaker::Narrator};
        std::string npcName;      // used only when speaker == Npc
        std::string text;         // default line
        std::string textForEmma;  // overrides text when the selected protagonist is Emma
        std::string textForMagga; // overrides text when the selected protagonist is Magga
        std::string requiredFlag; // empty = always eligible
        std::string nextLineId;   // empty if terminal or if choices are present
        std::vector<DialogueChoice> choices;
    };

    // Data-driven dialogue graph. Resolves protagonist-variant text and speaker
    // identity (the unselected protagonist becomes "OtherProtagonist").
    class DialogueGraph
    {
    public:
        void addLine(DialogueLine line);
        const DialogueLine* find(const std::string& id) const;

        std::string resolveText(const std::string& lineId, CharacterId selected) const;
        std::string resolveSpeakerName(const std::string& lineId, CharacterId selected) const;

    private:
        std::vector<DialogueLine> m_lines;
    };

} // namespace cybercba
