#include "cybercba/game/Resources.hpp"

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

using namespace cybercba::game;
using namespace cybercba::structures;

ResourceRuleEngine::ResourceRuleEngine(CampaignState* campaign_state, DynamicArray<IResourceRule>* rules)
{
    this->campaign_state = campaign_state;

    DynamicArray<ReglaDeCargaEMP> emp;
    DynamicArray<ReglaDeBonusDeCobertura> coverage;

    this->rules_container_emp      = emp;
    this->rules_container_coverage = coverage;

    for (std::size_t i = 0; i < rules->size(); i++)
    {
        IResourceRule& rule = (*rules)[i];

        if (rule.type() == ResourceType::EMP_CHARGE)
        {
            // chequeamos que podamos convertir IResourceRule en su clase derivada ReglaDeCargaEMP
            if (auto* empRule = dynamic_cast<ReglaDeCargaEMP*>(&rule))
            {
                this->rules_container_emp.pushBack(*empRule);
            }
        }

        if (rule.type() == ResourceType::COVER_BONUS)
        {
            // chequeamos que podamos convertir IResourceRule en su clase derivada ReglaDeBonusDeCobertura
            if (auto* coverageRule = dynamic_cast<ReglaDeBonusDeCobertura*>(&rule))
            {
                this->rules_container_coverage.pushBack(*coverageRule);
            }
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