#pragma once

#include <string>
#include <vector>

namespace cybercba
{

    enum class EncounterType
    {
        Combat,
        Hacking,
        Negotiation,
        Investigation,
        Survival,
        SystemRecovery
    };

    enum class EncounterOutcome
    {
        InProgress,
        Victory,
        Defeat,
        Escape,
        NarrativeResolution
    };

    struct StatusEffect
    {
        std::string id;
        int remainingTurns {0};
        int magnitude {0};
    };

    struct ActionCost
    {
        std::string resource; // e.g. "link", "momentum", "energy"; empty = no cost
        int amount {0};
    };

    struct ActionDefinition
    {
        std::string id;
        std::string displayName;
        ActionCost cost;
        int power {0};             // damage/effect magnitude, meaning depends on encounter config
        std::string appliesStatus; // empty = none
        int statusDuration {0};
    };

    struct EncounterParticipant
    {
        std::string id;
        std::string displayName;
        bool isPlayerControlled {true};
        int integrity {100};
        int maxIntegrity {100};
        int resourcePool {0};
        std::string resourceName {"resource"};
        int initiative {0};
        bool defeated {false};
        std::vector<StatusEffect> statuses;
    };

    struct ActionResult
    {
        bool valid {false};
        std::string reason;
        int amountApplied {0};
        std::string statusApplied;
    };

    // Reusable turn-based encounter engine shared by combat, hacking, and
    // system-recovery configurations. Raylib-free; drives state only.
    class Encounter
    {
    public:
        Encounter(EncounterType type, std::vector<EncounterParticipant> participants,
                  std::vector<ActionDefinition> actions);

        EncounterType type() const;
        const std::vector<EncounterParticipant>& participants() const;
        const EncounterParticipant* participant(const std::string& id) const;
        const std::vector<ActionDefinition>& actions() const;

        int tension() const;
        void addTension(int amount);

        // Computes initiative-descending turn order and starts round 1.
        void start();
        bool started() const;
        int round() const;

        const std::vector<std::string>& turnOrder() const;
        const std::string& activeParticipantId() const;

        // Validates cost/target, applies power to target integrity (or ally
        // healing if power is negative), applies the status effect if any,
        // and advances to the next turn. Rounds progress and per-turn status
        // durations decrement automatically.
        ActionResult performAction(const std::string& actorId, const std::string& actionId,
                                    const std::string& targetId);

        EncounterOutcome outcome() const;

    private:
        EncounterType m_type;
        std::vector<EncounterParticipant> m_participants;
        std::vector<ActionDefinition> m_actions;
        std::vector<std::string> m_turnOrder;
        std::size_t m_turnIndex {0};
        int m_round {1};
        int m_tension {0};
        bool m_started {false};

        EncounterParticipant* findMutable(const std::string& id);
        const ActionDefinition* findAction(const std::string& id) const;
        void computeTurnOrder();
        void advanceTurn();
        void tickStatuses(EncounterParticipant& participant);
        bool anyPlayerAlive() const;
        bool anyEnemyAlive() const;
    };

} // namespace cybercba
