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
    return static_cast<SkillLevel>(m_levels[static_cast<int>(id)]);
}

bool SkillProgress::upgrade(SkillId id)
{
    const auto index = static_cast<int>(id);
    if (m_levels[index] != static_cast<int>(SkillLevel::LevelI))
    {
        return false;
    }
    m_levels[index] = static_cast<int>(SkillLevel::LevelII);
    return true;
}

bool ProgressionSystem::canUpgrade(const SkillProgress& progress, SkillId id) const
{
    return progress.levelOf(id) == SkillLevel::LevelI;
}

bool ProgressionSystem::applyUpgrade(SkillProgress& progress, SkillId id) const
{
    if (!canUpgrade(progress, id))
    {
        return false;
    }
    return progress.upgrade(id);
}

} // namespace cybercba::progression
