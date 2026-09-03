#ifndef CYBERCBA_PROGRESSION_PROGRESSIONSYSTEM_H
#define CYBERCBA_PROGRESSION_PROGRESSIONSYSTEM_H

#include <cstdint>
#include <string_view>

#include "cybercba/progression/SkillId.hpp"

namespace cybercba::progression
{

// Level for a skill within a campaign (VS-001 §9). Skills start at Level I;
// this slice only enforces I -> II mechanically.
enum class SkillLevel : std::uint8_t
{
    LevelI   = 1,
    LevelII  = 2,
    LevelIII = 3
};

/// Tracks skill levels for one campaign in a fixed-size array indexed by SkillId.
//
// Ownership: value type, embedded in CampaignState's owner (Game).
class SkillProgress final
{
  public:
    SkillProgress();

    SkillLevel levelOf(SkillId id) const;
    bool upgrade(SkillId id);

  private:
    int m_levels[SKILL_COUNT];
};

/// Applies progression rules: award a skill point at
// mission close, let the player upgrade one available skill by one level.
//
// This system does not decide *which* skill the player picks (that is a UI/
// input concern) — it only validates and applies the upgrade.

class ISkillEffect
{
  public:
    virtual ~ISkillEffect() = default;
    virtual void apply(SkillProgress& progress) const = 0;
};


class ResourceEffect : public ISkillEffect
{
  public:
    ResourceEffect(SkillId id, SkillLevel targetLevel)
        : m_id(id), m_targetLevel(targetLevel) {}

    void apply(SkillProgress& progress) const override
    {
        const SkillDefinition& def = skillDefinition(m_id);
        std::string_view description = getDescriptionForLevel(def, m_targetLevel);
        (void)description;
    }

    std::string_view getDescription() const
    {
        return getDescriptionForLevel(skillDefinition(m_id), m_targetLevel);
    }

  private:
    SkillId m_id;
    SkillLevel m_targetLevel;

    static std::string_view getDescriptionForLevel(const SkillDefinition& def, SkillLevel level)
    {
        const bool isLevel1 = (level == SkillLevel::LevelI);
        if (isLevel1)
        {
            return def.levelIDescription;
        }

        const bool isLevel2 = (level == SkillLevel::LevelII);
        if (isLevel2)
        {
            return def.levelIIDescription;
        }

        const bool isLevel3 = (level == SkillLevel::LevelIII);
        if (isLevel3)
        {
            return def.levelIIIDescription;
        }

        return {};
    }
};

class ProgressionSystem final
{
  public:
    // Returns true if `id` can be upgraded (currently Level I, VS-001 §9 does
    // not require enforcing II -> III logic in this slice).
    bool canUpgrade(const SkillProgress& progress, SkillId id) const;

    // Applies the upgrade; returns false without modifying state if
    // canUpgrade() would have returned false.
    bool applyUpgrade(SkillProgress& progress, SkillId id) const;
};
struct PrerequisiteInfos
    {
    SkillLevel level;
    SkillId id;
    bool hasPrerequisites;
    };
} // namespace cybercba::progression

#endif // CYBERCBA_PROGRESSION_PROGRESSIONSYSTEM_H
