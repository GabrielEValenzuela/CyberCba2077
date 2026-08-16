#ifndef CYBERCBA_MISSIONS_PERSUASIONSTRATEGY_H
#define CYBERCBA_MISSIONS_PERSUASIONSTRATEGY_H

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/missions/DialogueSystem.hpp"

namespace cybercba::missions
{

// Strategy pattern for resolving whether an NPC contact cooperates (VS-001
// §9.5.1, §5.1). MissionSystem selects the concrete strategy once, based on
// CampaignState::player, instead of branching on the protagonist inline
// throughout NpcContact.
//
// Ownership: strategies are stateless behavior objects (see
// PersuasionStrategies below), held as observer references by callers —
// never owned per-NPC-instance.
class IPersuasionStrategy
{
  public:
    virtual ~IPersuasionStrategy() = default;

    // VS-001 §5.1: both routes succeed functionally (Ratón cooperates); this
    // exists so future content can make persuasion fallible without changing
    // callers.
    virtual bool attempt(const game::CampaignState& state) const = 0;

    // The line(s) to show for this route's flavor (Hackeo/Criptografía vs.
    // Intimidación, VS-001 §5.1).
    virtual const DialogueScript& resultScript() const = 0;
};

// Emma's route: offers proof of good faith via a cipher channel (GPD §9,
// VS-001 §5.1).
class HackerProofStrategy final : public IPersuasionStrategy
{
  public:
    explicit HackerProofStrategy(const DialogueScript& script);

    bool attempt(const game::CampaignState& state) const override;
    const DialogueScript& resultScript() const override;

  private:
    const DialogueScript* m_pScript;
};

// Magga's route: direct pressure (GPD §9, VS-001 §5.1).
class IntimidationStrategy final : public IPersuasionStrategy
{
  public:
    explicit IntimidationStrategy(const DialogueScript& script);

    bool attempt(const game::CampaignState& state) const override;
    const DialogueScript& resultScript() const override;

  private:
    const DialogueScript* m_pScript;
};

// Picks the strategy matching CampaignState::player (VS-001 §5.1).
const IPersuasionStrategy& selectPersuasionStrategy(game::PlayerCharacter character,
                                                    const HackerProofStrategy& emmaStrategy,
                                                    const IntimidationStrategy& maggaStrategy);

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_PERSUASIONSTRATEGY_H
