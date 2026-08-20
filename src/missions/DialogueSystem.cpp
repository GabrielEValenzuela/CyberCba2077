#include "cybercba/missions/DialogueSystem.hpp"

namespace cybercba::missions
{

void DialogueSystem::begin(const DialogueScript& script)
{
    m_pScript      = &script;
    m_currentIndex = 0;
}

const DialogueLine& DialogueSystem::currentLine() const
{
    return m_pScript->pLines[m_currentIndex];
}

bool DialogueSystem::hasChoices() const
{
    return !isFinished() && currentLine().choiceCount > 0;
}

bool DialogueSystem::isFinished() const
{
    return m_pScript == nullptr || m_currentIndex >= m_pScript->lineCount;
}

bool DialogueSystem::advance()
{
    if (isFinished() || hasChoices())
    {
        return false;
    }
    ++m_currentIndex;
    return true;
}

bool DialogueSystem::selectChoice(std::size_t choiceIndex)
{
    if (isFinished() || !hasChoices() || choiceIndex >= currentLine().choiceCount)
    {
        return false;
    }
    m_currentIndex = currentLine().pChoices[choiceIndex].nextLineIndex;
    return true;
}

} // namespace cybercba::missions
