#include "cybercba/Campaign.hpp"

#include <algorithm>

namespace cybercba
{
    namespace
    {
        constexpr CharacterDefinition EMMA {CharacterId::Emma,
                                            "EMMANUEL \"EMMA\" VÁZQUEZ",
                                            "Santa María",
                                            CharacterSpecialty::Hacking,
                                            "INTRUSIÓN DE ENLACE"};
        constexpr CharacterDefinition MAGGA {
            CharacterId::Magga, "MAGALÍ \"MAGGA\" TEMERRO", "San Justo", CharacterSpecialty::Combat, "IMPACTO TÁCTICO"};
        constexpr CharacterDefinition UNKNOWN {CharacterId::None, "", "", CharacterSpecialty::Hacking, ""};
    } // namespace
    const CharacterDefinition& characterDefinition(CharacterId id)
    {
        return id == CharacterId::Emma ? EMMA : id == CharacterId::Magga ? MAGGA : UNKNOWN;
    }
    CharacterId otherCharacter(CharacterId id)
    {
        return id == CharacterId::Emma ? CharacterId::Magga : CharacterId::Emma;
    }
    const char* characterName(CharacterId id)
    {
        return characterDefinition(id).displayName;
    }
    const char* characterAbility(CharacterId id)
    {
        return characterDefinition(id).abilityName;
    }
    bool AbilitySystem::hasHacking(CharacterId character)
    {
        return character == CharacterId::Emma;
    }
    bool AbilitySystem::hasCombat(CharacterId character)
    {
        return character == CharacterId::Magga;
    }
    AbilityResult AbilitySystem::hack(PlayerState& p, bool available, float)
    {
        if (!available)
            return AbilityResult::InvalidTarget;
        if (p.abilityCooldown > 0.0F)
            return AbilityResult::CoolingDown;
        p.abilityCooldown = 1.5F;
        return AbilityResult::Success;
    }
    AbilityResult AbilitySystem::strike(PlayerState& p, bool available, bool heavy)
    {
        if (!available)
            return AbilityResult::InvalidTarget;
        const float cost = heavy ? 35.0F : 18.0F;
        if (p.stamina < cost)
            return AbilityResult::InsufficientStamina;
        p.stamina -= cost;
        p.abilityCooldown = heavy ? 0.55F : 0.25F;
        return AbilityResult::Success;
    }
    bool AbilitySystem::applyDamage(PlayerState& p, float damage)
    {
        if (p.invulnerability > 0.0F || p.health <= 0.0F)
            return false;
        p.health = std::max(0.0F, p.health - std::max(0.0F, damage));
        p.invulnerability = 0.8F;
        return true;
    }
    void AbilitySystem::update(PlayerState& p, float dt)
    {
        p.abilityCooldown = std::max(0.0F, p.abilityCooldown - dt);
        p.invulnerability = std::max(0.0F, p.invulnerability - dt);
        p.stamina = std::min(100.0F, p.stamina + 22.0F * dt);
    }
    bool MissionSystem::startPrologue(CampaignProgress& p, CharacterId c)
    {
        if (c == CharacterId::None)
            return false;
        p = CampaignProgress {};
        p.selectedCharacter = c;
        p.prologue = MissionState::Active;
        p.objective = ObjectiveState::Active;
        p.prologueStarted = true;
        return true;
    }
    bool MissionSystem::advance(CampaignProgress& p,
                                PrologueStage expected,
                                PrologueStage next,
                                const std::string& checkpoint)
    {
        if (p.prologue != MissionState::Active || p.stage != expected)
            return false;
        p.stage = next;
        if (!checkpoint.empty())
        {
            p.checkpoint = checkpoint;
            p.checkpointReached = true;
        }
        return true;
    }
    bool MissionSystem::complete(CampaignProgress& p)
    {
        if (p.prologue != MissionState::Active || p.stage != PrologueStage::Epilogue)
            return false;
        p.stage = PrologueStage::Complete;
        p.prologue = MissionState::Completed;
        p.objective = ObjectiveState::Completed;
        p.prologueCompleted = true;
        p.neometroUnlocked = true;
        p.checkpoint = "neometro_gate";
        return true;
    }
} // namespace cybercba
