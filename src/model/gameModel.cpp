#include "model/gameModel.hpp"

#include "common/inventoryUtils.hpp"
#include "common/types.hpp"

#include <cassert>
#include <stdexcept>

namespace CyberpunkCba
{

    // =============================================================================
    // Construcción
    // =============================================================================

    GameModel::GameModel(std::string playerName)
        : m_playerName {std::move(playerName)}
        , m_hp {INITIAL_HP}
        , m_maxHp {INITIAL_HP}
        , m_credits {INITIAL_CREDITS}
        , m_currentZone {"Sector 7"}
        , m_alertLevel {AlertLevel::Low}
        , m_simulationDay {INITIAL_DAY}
        , m_currentHour {INITIAL_HOUR}
        , m_currentMinute {INITIAL_MINUTE}
        , m_totalZoneCount {TOTAL_ZONE_COUNT}
        , m_inventoryCapacity {INVENTORY_CAPACITY}
        , m_completedMissions {0}
        , m_failedMissions {0}
        , m_hackAttempts {MAX_HACK_ATTEMPTS}
        , m_maxHackAttempts {MAX_HACK_ATTEMPTS}
        , m_hackCost {HACK_COST}
        , m_commandCount {0}
        , m_sessionStart {std::chrono::steady_clock::now()}
        , m_running {true}
    {
        assert(!m_playerName.empty());

        // Reputación inicial
        m_factionRep.fill(INITIAL_REP);

        // Zonas adyacentes al Sector 7
        m_adjacentZones.emplace_back("Distrito Industrial");
        m_adjacentZones.emplace_back("Favela Norte");
        m_adjacentZones.emplace_back("Puerto Libre");

        m_visitedZones.emplace_back(m_currentZone);

        // Entidades iniciales en el sector
        m_worldEntities.push_back({"Corporativo Militech", EntityDisposition::Hostile, 120});
        m_worldEntities.push_back({"Vendedor ambulante", EntityDisposition::Neutral, 35});
        m_worldEntities.push_back({"Netrunner aliado", EntityDisposition::Friendly, 80});

        // Punteros observadores — construidos después de llenar el vector
        for (const auto& entity : m_worldEntities)
        {
            m_nearbyEntities.emplace_back(&entity);
        }

        // Misión inicial
        m_spActiveMission = std::make_unique<Mission>(Mission {"Infiltración Militech",
                                                               "Extraer datos del servidor corporativo del Sector 7",
                                                               MissionStatus::InProgress,
                                                               500,
                                                               15});

        // Ítem inicial en inventario
        m_inventory.push_back({"Deck de hackeo básico", ItemType::Tech, 80, 1});

        logAction("Sistema iniciado. Bienvenido, " + m_playerName + ".");
    }

    // =============================================================================
    // Runner — lectura
    // =============================================================================

    const std::string& GameModel::playerName() const noexcept
    {
        return m_playerName;
    }
    int GameModel::hp() const noexcept
    {
        return m_hp;
    }
    int GameModel::maxHp() const noexcept
    {
        return m_maxHp;
    }
    int GameModel::credits() const noexcept
    {
        return m_credits;
    }

    bool GameModel::isCriticalHp() const noexcept
    {
        return m_hp <= static_cast<int>(static_cast<float>(m_maxHp) * CRITICAL_HP_RATIO);
    }

    // =============================================================================
    // Mundo — lectura
    // =============================================================================

    const std::string& GameModel::currentZone() const noexcept
    {
        return m_currentZone;
    }
    AlertLevel GameModel::alertLevel() const noexcept
    {
        return m_alertLevel;
    }
    int GameModel::simulationDay() const noexcept
    {
        return m_simulationDay;
    }
    int GameModel::currentHour() const noexcept
    {
        return m_currentHour;
    }
    int GameModel::currentMinute() const noexcept
    {
        return m_currentMinute;
    }
    int GameModel::totalZoneCount() const noexcept
    {
        return m_totalZoneCount;
    }
    const std::string& GameModel::lastAlertCause() const noexcept
    {
        return m_lastAlertCause;
    }

    TimeOfDay GameModel::timeOfDay() const noexcept
    {
        return timeOfDayFromHour(m_currentHour);
    }

    const std::vector<std::string>& GameModel::adjacentZones() const noexcept
    {
        return m_adjacentZones;
    }

    const std::vector<std::string>& GameModel::visitedZones() const noexcept
    {
        return m_visitedZones;
    }

    const std::vector<const WorldEntity*>& GameModel::nearbyEntities() const noexcept
    {
        return m_nearbyEntities;
    }

    // =============================================================================
    // Inventario — lectura
    // =============================================================================

    const std::vector<Item>& GameModel::inventory() const noexcept
    {
        return m_inventory;
    }
    int GameModel::inventoryCapacity() const noexcept
    {
        return m_inventoryCapacity;
    }

    bool GameModel::isInventoryFull() const noexcept
    {
        return static_cast<int>(m_inventory.size()) >= m_inventoryCapacity;
    }

    int GameModel::inventoryTotalValue() const
    {
        return CyberPunkCba::inventoryValueRec(m_inventory,0);
    }

    // =============================================================================
    // Reputación — lectura
    // =============================================================================

    int GameModel::repValue(const Faction faction) const noexcept
    {
        return m_factionRep.at(static_cast<std::size_t>(faction));
    }

    Faction GameModel::dominantFaction() const noexcept
    {
        auto dominant {Faction::Corporations};
        auto maxRep {m_factionRep[0]};

        for (std::size_t i {1}; i < FACTION_COUNT; ++i)
        {
            if (m_factionRep[i] > maxRep)
            {
                maxRep = m_factionRep[i];
                dominant = static_cast<Faction>(i);
            }
        }
        return dominant;
    }

    // =============================================================================
    // Misiones — lectura
    // =============================================================================

    const Mission* GameModel::activeMission() const noexcept
    {
        return m_spActiveMission.get();
    }
    int GameModel::completedMissions() const noexcept
    {
        return m_completedMissions;
    }
    int GameModel::failedMissions() const noexcept
    {
        return m_failedMissions;
    }
    const Mission* GameModel::lastFailedMission() const noexcept
    {
        return m_spLastFailed.get();
    }

    // =============================================================================
    // Hackeo — lectura
    // =============================================================================

    int GameModel::hackAttempts() const noexcept
    {
        return m_hackAttempts;
    }
    int GameModel::maxHackAttempts() const noexcept
    {
        return m_maxHackAttempts;
    }
    int GameModel::hackCost() const noexcept
    {
        return m_hackCost;
    }

    // =============================================================================
    // Sesión — lectura
    // =============================================================================

    int GameModel::commandCount() const noexcept
    {
        return m_commandCount;
    }

    std::chrono::seconds GameModel::sessionDuration() const noexcept
    {
        const auto now {std::chrono::steady_clock::now()};
        return std::chrono::duration_cast<std::chrono::seconds>(now - m_sessionStart);
    }

    const std::vector<LogEntry>& GameModel::actionLog() const noexcept
    {
        return m_actionLog;
    }
    bool GameModel::isRunning() const noexcept
    {
        return m_running;
    }

    // =============================================================================
    // Mutadores
    // =============================================================================

    void GameModel::incrementAlert(std::string cause)
    {
        assert(!cause.empty());
        m_alertLevel = incrementAlertLevel(m_alertLevel);
        m_lastAlertCause = std::move(cause);
    }

    void GameModel::spendCredits(const int amount)
    {
        assert(amount > 0);
        assert(m_credits >= amount);
        m_credits -= amount;
    }

    void GameModel::logAction(std::string message)
    {
        assert(!message.empty());
        m_actionLog.push_back({m_currentHour, m_currentMinute, std::move(message)});
    }

    void GameModel::incrementCommandCount() noexcept
    {
        ++m_commandCount;
    }

    void GameModel::consumeHackAttempt() noexcept
    {
        assert(m_hackAttempts > 0);
        --m_hackAttempts;
    }

    void GameModel::quit() noexcept
    {
        m_running = false;
    }

} // namespace CyberpunkCba
