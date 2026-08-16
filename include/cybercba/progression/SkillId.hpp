#ifndef CYBERCBA_PROGRESSION_SKILLID_H
#define CYBERCBA_PROGRESSION_SKILLID_H

#include <cstdint>

namespace cybercba::progression
{

// The 15 skills defined by the campaign (GPD §11-14). Only 10 are presented
// to the player per campaign: the 5 Common ones plus whichever 5 match the
// chosen PlayerCharacter.
enum class SkillId : std::uint8_t
{
    // Common (GPD §12).
    Perception,
    Persuasion,
    Stealth,
    Resistance,
    Improvisation,

    // Emma-exclusive (GPD §13).
    Hacking,
    Cryptography,
    Engineering,
    Drones,
    Intrusion,

    // Magga-exclusive (GPD §14).
    Marksmanship,
    MeleeCombat,
    Tactics,
    Intimidation,
    Survival,
};

constexpr int SKILL_COUNT = 15;

} // namespace cybercba::progression

#endif // CYBERCBA_PROGRESSION_SKILLID_H
