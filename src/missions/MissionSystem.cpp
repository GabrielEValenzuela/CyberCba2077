#include "cybercba/missions/MissionSystem.hpp"

namespace cybercba::missions
{

void MissionSystem::begin(const MissionDefinition& mission)
{
    m_pMission          = &mission;
    m_currentSceneIndex = 0;
    if (mission.sceneCount > 0)
    {
        enterScene(0);
    }
}

bool MissionSystem::isFinished() const
{
    return m_pMission == nullptr || m_currentSceneIndex >= m_pMission->sceneCount;
}

SceneType MissionSystem::currentSceneType() const
{
    return m_pMission->pScenes[m_currentSceneIndex].type;
}

const DialogueLine& MissionSystem::currentDialogueLine() const
{
    return m_dialogueSystem.currentLine();
}

bool MissionSystem::hasDialogueChoices() const
{
    return m_dialogueSystem.hasChoices();
}

bool MissionSystem::advanceDialogue()
{
    if (isFinished() || currentSceneType() != SceneType::Dialogue)
    {
        return false;
    }
    if (!m_dialogueSystem.advance())
    {
        return false;
    }
    if (m_dialogueSystem.isFinished())
    {
        return advanceToNextScene();
    }
    return true;
}

bool MissionSystem::selectDialogueChoice(std::size_t choiceIndex)
{
    if (isFinished() || currentSceneType() != SceneType::Dialogue)
    {
        return false;
    }
    if (!m_dialogueSystem.selectChoice(choiceIndex))
    {
        return false;
    }
    if (m_dialogueSystem.isFinished())
    {
        return advanceToNextScene();
    }
    return true;
}

bool MissionSystem::completeCurrentScene()
{
    if (isFinished())
    {
        return false;
    }
    const SceneType type = currentSceneType();
    if (type != SceneType::Puzzle && type != SceneType::Combat)
    {
        return false;
    }
    return advanceToNextScene();
}

void MissionSystem::enterScene(std::size_t index)
{
    m_currentSceneIndex          = index;
    const SceneDefinition& scene = m_pMission->pScenes[index];
    if (scene.type == SceneType::Dialogue)
    {
        m_dialogueSystem.begin(*scene.pDialogue);
    }
}

bool MissionSystem::advanceToNextScene()
{
    const std::size_t nextIndex = m_currentSceneIndex + 1;
    if (nextIndex >= m_pMission->sceneCount)
    {
        m_currentSceneIndex = nextIndex;
        return true;
    }
    enterScene(nextIndex);
    return true;
}

} // namespace cybercba::missions
