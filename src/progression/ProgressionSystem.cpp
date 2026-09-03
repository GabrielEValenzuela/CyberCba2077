#include "cybercba/progression/ProgressionSystem.hpp"

namespace cybercba::progression
{

SkillProgress::SkillProgress()
{
    for (int& level : m_levels)
    {
        level = static_cast<int>(SkillLevel::LevelI);
    }
}

SkillLevel SkillProgress::levelOf(SkillId id) const
{
    const auto index = static_cast<int>(id);
    if (index >= SKILL_COUNT)
    {
        return SkillLevel::LevelI;
    }
    return static_cast<SkillLevel>(m_levels[static_cast<int>(id)]);
}

bool SkillProgress::upgrade(SkillId id)
{
    const auto index = static_cast<int>(id);
    if (m_levels[index] >= static_cast<int>(SkillLevel::LevelIII))
    {
        return false;
    }
    m_levels[index] ++;
    return true;
}

bool ProgressionSystem::canUpgrade(const SkillProgress& progress, SkillId id) const
{
    if (progress.levelOf(id) == SkillLevel::LevelIII)
    {
        return false;
    }
    else
    {
        return true;
    }
}

PrerequisiteInfo getPrerequisite(SkillId id, SkillLevel level)
{
    if ( id == SkillId::MeleeCombat && level == SkillLevel::LevelIII)return {SkillId::Resistance, SkillLevel::LevelII, true } ;
    else if ( id == SkillId::Drones && level == SkillLevel::LevelIII)return{SkillId::Engineering, SkillLevel::LevelIII, true } ;
    else if ( id == SkillId::Stealth && level == SkillLevel::LevelIII)return{ SkillId::Perception, SkillLevel::LevelII, true } ;
    else
    {
        if (level == SkillLevel::LevelI)
        {
            return { id, level , false };
        }
        else
        {
            return {id,static_cast<SkillLevel>(static_cast<int>(level) - 1),true};
        }
    }
}
bool ProgressionSystem::applyUpgrade(SkillProgress& progress, SkillId id) const
{
    if (canUpgrade(progress, id) == false)
    {
        return false;
    }
    SkillId currentId = id;
    SkillLevel currentLevel = static_cast<SkillLevel>(static_cast<int>(progress.levelOf(id)) + 1 ) ;


    while (true)
    {
        PrerequisiteInfos prerequisite = getPrerequisite(currentId, currentLevel);

        if (prerequisite.hasPrerequisite == false)
        {
            break;
        }

        else if (prerequisite.hasPrerequisite == true)
        {
            if (progress.levelOf(prerequisite.id) < prerequisite.level)
            {
                return false;
            }
        }
        currentId = prerequisite.id;
        currentLevel = prerequisite.level;
    }
    return progress.upgrade(id);
}

bool ProgressionSystem::applyUpgrade(SkillProgress& progress, SkillId id) const
{
    const bool canBeUpgraded = canUpgrade(progress, id);
    if (!canBeUpgraded)
    {
        return false;
    }

    const bool upgradeSucceeded = progress.upgrade(id);
    if (!upgradeSucceeded)
    {
        return false;
    }

    const SkillLevel updatedLevel = progress.levelOf(id);
    ResourceEffect effect(id, updatedLevel);
    effect.apply(progress);

    return true;
}

} // namespace cybercba::progression
