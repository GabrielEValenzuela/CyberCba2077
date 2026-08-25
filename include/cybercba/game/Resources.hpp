#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::game
{
enum ResourceType
{
    EMP_CHARGE,
    COVER_BONUS
};

class ResourceRuleEngine final
{
  public:
    bool consultar(ResourceType resource_type, int amount);
    ResourceRuleEngine(CampaignState* campaign_state,
                       cybercba::structures::DynamicArray<IResourceRule>* rules_container);
    ~ResourceRuleEngine();
    cybercba::structures::DynamicArray<ReglaDeCargaEMP> rules_container_emp;
    cybercba::structures::DynamicArray<ReglaDeBonusDeCobertura> rules_container_coverage;
    CampaignState* campaign_state;
};

class IResourceRule
{
  public:
    virtual bool puedeConsumir(int currentProgress, int amount);
    virtual bool puedeOtorgar(int currentProgress, int amount);
    virtual ResourceType type();
    virtual cybercba::structures::DynamicArray<IResourceRule> dependsOn();
};

class ReglaDeCargaEMP : public IResourceRule
{
  public:
    bool puedeConsumir(int currentProgress, int amount);
    bool puedeOtorgar(int currentProgress, int amount);
    ResourceType type();
    cybercba::structures::DynamicArray<IResourceRule> dependsOn();
};

class ReglaDeBonusDeCobertura : public IResourceRule
{
  public:
    bool puedeConsumir(int currentProgress, int amount);
    bool puedeOtorgar(int currentProgress, int amount);
    ResourceType type();
    cybercba::structures::DynamicArray<IResourceRule> dependsOn();
};

} // namespace cybercba::game

#endif
