#include "cybercba/Encounter.hpp"

#include <algorithm>

namespace cybercba
{

    Encounter::Encounter(EncounterType type, std::vector<EncounterParticipant> participants,
                         std::vector<ActionDefinition> actions)
        : m_type(type)
        , m_participants(std::move(participants))
        , m_actions(std::move(actions))
    {
    }

    EncounterType Encounter::type() const
    {
        return m_type;
    }

    const std::vector<EncounterParticipant>& Encounter::participants() const
    {
        return m_participants;
    }

    const EncounterParticipant* Encounter::participant(const std::string& id) const
    {
        for (const auto& p : m_participants)
        {
            if (p.id == id)
                return &p;
        }
        return nullptr;
    }

    const std::vector<ActionDefinition>& Encounter::actions() const
    {
        return m_actions;
    }

    int Encounter::tension() const
    {
        return m_tension;
    }

    void Encounter::addTension(int amount)
    {
        m_tension = std::max(0, m_tension + amount);
    }

    void Encounter::computeTurnOrder()
    {
        m_turnOrder.clear();
        std::vector<const EncounterParticipant*> ordered;
        ordered.reserve(m_participants.size());
        for (const auto& p : m_participants)
            ordered.push_back(&p);
        std::stable_sort(ordered.begin(), ordered.end(), [](const EncounterParticipant* a, const EncounterParticipant* b)
        {
            return a->initiative > b->initiative;
        });
        for (const auto* p : ordered)
            m_turnOrder.push_back(p->id);
    }

    void Encounter::start()
    {
        computeTurnOrder();
        m_turnIndex = 0;
        m_round = 1;
        m_tension = 0;
        m_started = true;
    }

    bool Encounter::started() const
    {
        return m_started;
    }

    int Encounter::round() const
    {
        return m_round;
    }

    const std::vector<std::string>& Encounter::turnOrder() const
    {
        return m_turnOrder;
    }

    const std::string& Encounter::activeParticipantId() const
    {
        static const std::string empty;
        if (!m_started || m_turnOrder.empty())
            return empty;
        return m_turnOrder[m_turnIndex];
    }

    EncounterParticipant* Encounter::findMutable(const std::string& id)
    {
        for (auto& p : m_participants)
        {
            if (p.id == id)
                return &p;
        }
        return nullptr;
    }

    const ActionDefinition* Encounter::findAction(const std::string& id) const
    {
        for (const auto& a : m_actions)
        {
            if (a.id == id)
                return &a;
        }
        return nullptr;
    }

    bool Encounter::anyPlayerAlive() const
    {
        for (const auto& p : m_participants)
        {
            if (p.isPlayerControlled && !p.defeated)
                return true;
        }
        return false;
    }

    bool Encounter::anyEnemyAlive() const
    {
        for (const auto& p : m_participants)
        {
            if (!p.isPlayerControlled && !p.defeated)
                return true;
        }
        return false;
    }

    void Encounter::tickStatuses(EncounterParticipant& p)
    {
        std::vector<StatusEffect> remaining;
        remaining.reserve(p.statuses.size());
        for (auto status : p.statuses)
        {
            status.remainingTurns -= 1;
            if (status.remainingTurns > 0)
                remaining.push_back(status);
        }
        p.statuses = std::move(remaining);
    }

    void Encounter::advanceTurn()
    {
        if (m_turnOrder.empty())
            return;
        const std::size_t startIndex = m_turnIndex;
        do
        {
            m_turnIndex = (m_turnIndex + 1) % m_turnOrder.size();
            if (m_turnIndex == 0)
            {
                m_round += 1;
                for (auto& p : m_participants)
                    tickStatuses(p);
            }
            const EncounterParticipant* candidate = participant(m_turnOrder[m_turnIndex]);
            if (candidate && !candidate->defeated)
                return;
        } while (m_turnIndex != startIndex);
    }

    ActionResult Encounter::performAction(const std::string& actorId, const std::string& actionId,
                                          const std::string& targetId)
    {
        ActionResult result;
        if (!m_started || outcome() != EncounterOutcome::InProgress)
        {
            result.reason = "encounter not active";
            return result;
        }
        if (actorId != activeParticipantId())
        {
            result.reason = "not this participant's turn";
            return result;
        }
        const ActionDefinition* action = findAction(actionId);
        if (!action)
        {
            result.reason = "unknown action";
            return result;
        }
        EncounterParticipant* actor = findMutable(actorId);
        EncounterParticipant* target = findMutable(targetId);
        if (!actor || !target)
        {
            result.reason = "unknown participant";
            return result;
        }
        if (!action->cost.resource.empty() && actor->resourcePool < action->cost.amount)
        {
            result.reason = "insufficient " + action->cost.resource;
            return result;
        }
        if (!action->cost.resource.empty())
            actor->resourcePool -= action->cost.amount;

        target->integrity = std::clamp(target->integrity - action->power, 0, target->maxIntegrity);
        if (target->integrity == 0)
            target->defeated = true;

        if (!action->appliesStatus.empty())
            target->statuses.push_back(StatusEffect {action->appliesStatus, action->statusDuration, action->power});

        result.valid = true;
        result.amountApplied = action->power;
        result.statusApplied = action->appliesStatus;

        advanceTurn();
        return result;
    }

    EncounterOutcome Encounter::outcome() const
    {
        if (!m_started)
            return EncounterOutcome::InProgress;
        if (!anyPlayerAlive())
            return EncounterOutcome::Defeat;
        if (!anyEnemyAlive())
            return EncounterOutcome::Victory;
        return EncounterOutcome::InProgress;
    }

} // namespace cybercba
