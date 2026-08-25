#include "cybercba/game/Resources.hpp"

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

using namespace cybercba::game;
using namespace cybercba::structures;

ResourceRuleEngine::ResourceRuleEngine(CampaignState* campaign_state, DynamicArray<IResourceRule>* rules)
{
    this->campaign_state = campaign_state;

    this->rules_container_emp      = DynamicArray<ReglaDeCargaEMP>();
    this->rules_container_coverage = DynamicArray<ReglaDeBonusDeCobertura>();

    for (int i = 0; i < rules->size(); i++)
    {
        if (rules[i].type() == ResourceType::EMP_CHARGE)
        {
            this->rules_container_emp.pushBack((ReglaDeCargaEMP)rules[i]);
        }

        if (rules[i].type() == ResourceType::COVER_BONUS)
        {
            this->rules_container_emp.pushBack((ReglaDeBonusDeCobertura)rules[i]);
        }
    }
}

bool ResourceRuleEngine::consultar(ResourceType resource_type, int amount)
{
    if (amount == 0)
    {
        return true;
    }

    bool result            = true;
    bool isConsuming       = amount > 0;
    unsigned int absAmount = amount > 0 ? amount : -amount;

    if (resource_type == ResourceType::EMP_CHARGE)
    {
        for (int i = 0; i < this->rules_container_emp.size(); i++)
        {
            ReglaDeCargaEMP rule = this->rules_container_emp[i];

            if (isConsuming && !rule.puedeConsumir(this->campaign_state->empCharges, absAmount))
            {
                return false;
            }

            if (!isConsuming && !rule.puedeOtorgar(this->campaign_state->empCharges, absAmount))
            {
                return false;
            }
        }
    }

    if (resource_type == ResourceType::COVER_BONUS)
    {
        // igual que arriba pero para covertura
    }

    return result;
}

bool ReglaDeCargaEMP::puedeConsumir(int currentProgress, int amount)
{
    return true;
}

bool ReglaDeCargaEMP::puedeOtorgar(int currentProgress, int amount)
{
    return true;
}

ResourceType ReglaDeCargaEMP::type()
{
    return ResourceType::EMP_CHARGE;
}

bool ReglaDeBonusDeCobertura::puedeConsumir(int currentProgress, int amount)
{
    return false;
}

bool ReglaDeBonusDeCobertura::puedeOtorgar(int currentProgress, int amount)
{
    return false;
}

ResourceType ReglaDeBonusDeCobertura::type()
{
    return ResourceType::COVER_BONUS;
}