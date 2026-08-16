#ifndef CYBERCBA_PROGRESSION_SKILLDEFINITION_H
#define CYBERCBA_PROGRESSION_SKILLDEFINITION_H

#include <string_view>

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/progression/SkillId.hpp"

namespace cybercba::progression
{

// Static, immutable skill catalog data (TAD-001 §21: content as immutable C++
// structures, no external format for VS-001). Levels start at I (VS-001 §9);
// only I->II is mechanically enforced by ProgressionSystem in this slice.
// POD data holder: fields intentionally have no m_ prefix (ADR-001 exception).
struct SkillDefinition
{
    SkillId id;
    std::string_view name;
    std::string_view levelIDescription;
    std::string_view levelIIDescription;
    std::string_view levelIIIDescription;
};

// Returns the definition for a given skill id.
const SkillDefinition& skillDefinition(SkillId id);

// Returns true if this skill is presented to a player who chose `character`
// (the 5 Common skills plus that character's 5 exclusive skills, GPD §11).
bool isSkillAvailableTo(SkillId id, cybercba::game::PlayerCharacter character);

} // namespace cybercba::progression

#endif // CYBERCBA_PROGRESSION_SKILLDEFINITION_H
