#include "common/types.hpp"

namespace CyberpunkCba
{

    // =============================================================================
    // AlertLevel
    // =============================================================================

    std::string alertLevelToString(const AlertLevel level)
    {
        switch (level)
        {
            case AlertLevel::None: return "NINGUNA";
            case AlertLevel::Low: return "BAJA";
            case AlertLevel::Medium: return "MEDIA";
            case AlertLevel::High: return "ALTA";
            case AlertLevel::Maximum: return "MÁXIMA";
            default: return "DESCONOCIDA";
        }
    }

    AlertLevel incrementAlertLevel(const AlertLevel level) noexcept
    {
        if (level == AlertLevel::Maximum)
        {
            return AlertLevel::Maximum;
        }
        return static_cast<AlertLevel>(static_cast<uint8_t>(level) + 1);
    }

    // =============================================================================
    // TimeOfDay
    // =============================================================================

    std::string timeOfDayToString(const TimeOfDay tod)
    {
        switch (tod)
        {
            case TimeOfDay::Dawn: return "AMANECER";
            case TimeOfDay::Day: return "DÍA";
            case TimeOfDay::Dusk: return "ATARDECER";
            case TimeOfDay::Night: return "NOCHE";
            default: return "DESCONOCIDO";
        }
    }

    TimeOfDay timeOfDayFromHour(const int hour) noexcept
    {
        if (hour >= DAWN_START_HOUR && hour < DAWN_END_HOUR)
        {
            return TimeOfDay::Dawn;
        }
        if (hour >= DAWN_END_HOUR && hour < DAY_END_HOUR)
        {
            return TimeOfDay::Day;
        }
        if (hour >= DAY_END_HOUR && hour < DUSK_END_HOUR)
        {
            return TimeOfDay::Dusk;
        }
        return TimeOfDay::Night;
    }

    int minutesUntilNextTurn(const int hour, const int minute) noexcept
    {
        const int currentMins {(hour * 60) + minute};

        if (currentMins < DAWN_END_MINUTES)
        {
            return DAWN_END_MINUTES - currentMins;
        }
        if (currentMins < DAY_END_MINUTES)
        {
            return DAY_END_MINUTES - currentMins;
        }
        if (currentMins < DUSK_END_MINUTES)
        {
            return DUSK_END_MINUTES - currentMins;
        }
        // Turno Night: próximo amanecer a las 04:00 del día siguiente
        constexpr int NEXT_DAWN_MINUTES {(DAWN_START_HOUR * 60) + DAY_TOTAL_MINUTES};
        return NEXT_DAWN_MINUTES - currentMins;
    }

    // =============================================================================
    // Faction
    // =============================================================================

    std::string factionToString(const Faction faction)
    {
        switch (faction)
        {
            case Faction::Corporations: return "CORPORACIONES";
            case Faction::Streeters: return "CALLEJEROS";
            case Faction::NetRunners: return "NETRUNNERS";
            default: return "DESCONOCIDA";
        }
    }

    // =============================================================================
    // RepLevel
    // =============================================================================

    std::string repLevelToString(const RepLevel level)
    {
        switch (level)
        {
            case RepLevel::Hostile: return "HOSTIL";
            case RepLevel::Neutral: return "NEUTRAL";
            case RepLevel::Allied: return "ALIADO";
            case RepLevel::Legendary: return "LEGENDARIO";
            default: return "DESCONOCIDO";
        }
    }

    RepLevel repLevelFromValue(const int value) noexcept
    {
        if (value < REP_NEUTRAL_THRESHOLD)
        {
            return RepLevel::Hostile;
        }
        if (value < REP_ALLIED_THRESHOLD)
        {
            return RepLevel::Neutral;
        }
        if (value < REP_LEGENDARY_THRESHOLD)
        {
            return RepLevel::Allied;
        }
        return RepLevel::Legendary;
    }

    // =============================================================================
    // MissionStatus
    // =============================================================================

    std::string missionStatusToString(const MissionStatus status)
    {
        switch (status)
        {
            case MissionStatus::Pending: return "PENDIENTE";
            case MissionStatus::InProgress: return "EN CURSO";
            case MissionStatus::Completed: return "COMPLETADA";
            case MissionStatus::Failed: return "FALLIDA";
            default: return "DESCONOCIDO";
        }
    }

    // =============================================================================
    // ItemType
    // =============================================================================

    std::string itemTypeToString(const ItemType type)
    {
        switch (type)
        {
            case ItemType::Weapon: return "ARMA";
            case ItemType::Consumable: return "CONSUMIBLE";
            case ItemType::Value: return "VALOR";
            case ItemType::Tech: return "TECH";
            default: return "DESCONOCIDO";
        }
    }

    // =============================================================================
    // EntityDisposition
    // =============================================================================

    std::string entityDispositionToString(const EntityDisposition disposition)
    {
        switch (disposition)
        {
            case EntityDisposition::Friendly: return "AMIGABLE";
            case EntityDisposition::Neutral: return "NEUTRAL";
            case EntityDisposition::Hostile: return "HOSTIL";
            default: return "DESCONOCIDA";
        }
    }

} // namespace CyberpunkCba
