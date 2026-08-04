#include "cybercba/Dialogue.hpp"

namespace cybercba
{

    void DialogueGraph::addLine(DialogueLine line)
    {
        if (find(line.id) != nullptr)
            return;
        m_lines.push_back(std::move(line));
    }

    const DialogueLine* DialogueGraph::find(const std::string& id) const
    {
        for (const auto& line : m_lines)
        {
            if (line.id == id)
                return &line;
        }
        return nullptr;
    }

    std::string DialogueGraph::resolveText(const std::string& lineId, CharacterId selected) const
    {
        const DialogueLine* line = find(lineId);
        if (!line)
            return "";
        if (selected == CharacterId::Emma && !line->textForEmma.empty())
            return line->textForEmma;
        if (selected == CharacterId::Magga && !line->textForMagga.empty())
            return line->textForMagga;
        return line->text;
    }

    std::string DialogueGraph::resolveSpeakerName(const std::string& lineId, CharacterId selected) const
    {
        const DialogueLine* line = find(lineId);
        if (!line)
            return "";
        switch (line->speaker)
        {
            case DialogueSpeaker::Narrator:
                return "";
            case DialogueSpeaker::Protagonist:
                return characterName(selected);
            case DialogueSpeaker::OtherProtagonist:
                return characterName(otherCharacter(selected));
            case DialogueSpeaker::Npc:
                return line->npcName;
        }
        return "";
    }

} // namespace cybercba
