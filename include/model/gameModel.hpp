#ifndef _GAME_MODEL_HPP
#define _GAME_MODEL_HPP

#include "common/types.hpp"

#include <array>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

/**
 * @file GameModel.hpp
 * @brief Estado completo de la simulación — capa Model del MVC.
 *
 * @details
 * GameModel es la única fuente de verdad del estado del juego.
 * Todo Command recibe una referencia a GameModel y opera sobre él
 * a través de su interfaz pública.
 *
 * ## Invariantes de clase
 * - m_playerName    : no vacío post-construcción.
 * - m_hp            : en [0, m_maxHp] siempre.
 * - m_credits       : >= 0 siempre.
 * - m_alertLevel    : valor válido de AlertLevel siempre.
 * - m_hackAttempts  : en [0, m_maxHackAttempts] siempre.
 * - m_inventory     : size() <= m_inventoryCapacity siempre.
 * - m_commandCount  : monótonamente creciente.
 * - m_running       : una vez false, no vuelve a true.
 * - m_factionRep    : cada valor en [0, 100] siempre.
 *
 * ## Ownership
 * - m_inventory        : owned por valor.
 * - m_nearbyEntities   : punteros observadores a m_worldEntities.
 *                        GameModel NO libera estas entidades.
 * - m_spActiveMission  : unique_ptr — owned por GameModel.
 * - m_spLastFailed     : unique_ptr — owned por GameModel.
 *
 * ## Política
 * READ ONLY para equipos de alumnos.
 *
 * @author Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    class GameModel final
    {
    public:
        // =========================================================================
        /// @name Construcción
        // =========================================================================
        /// @{

        /**
         * @brief Construye el modelo con estado inicial de juego nuevo.
         * @param playerName Nombre del runner. No debe ser vacío.
         * @pre !playerName.empty()
         * @post Todos los invariantes satisfechos.
         * @post isRunning() == true
         * @post hp() == maxHp()
         * @post credits() == INITIAL_CREDITS
         */
        explicit GameModel(std::string playerName);

        /// @brief No copiable — entidad única de la simulación.
        GameModel(const GameModel&) = delete;
        /// @brief No copiable.
        GameModel& operator=(const GameModel&) = delete;

        /// @brief Movible para inicialización desde factory.
        GameModel(GameModel&&) = default;
        /// @brief Movible.
        GameModel& operator=(GameModel&&) = default;

        ~GameModel() = default;

        /// @}

        // =========================================================================
        /// @name Estado del runner — lectura
        // =========================================================================
        /// @{

        /// @brief Nombre del runner. Nunca vacío.
        const std::string& playerName() const noexcept;

        /// @brief HP actual. Siempre en [0, maxHp()].
        int hp() const noexcept;

        /// @brief HP máximo.
        int maxHp() const noexcept;

        /// @brief Créditos disponibles. Siempre >= 0.
        int credits() const noexcept;

        /// @brief true si hp() <= maxHp() * CRITICAL_HP_RATIO.
        bool isCriticalHp() const noexcept;

        /// @}

        // =========================================================================
        /// @name Estado del mundo — lectura
        // =========================================================================
        /// @{

        /// @brief Zona actual. Ejemplo: "Sector 7".
        const std::string& currentZone() const noexcept;

        /// @brief Nivel de alerta de la zona actual.
        AlertLevel alertLevel() const noexcept;

        /// @brief Día de simulación. Empieza en 1.
        int simulationDay() const noexcept;

        /// @brief Hora actual [0, 23].
        int currentHour() const noexcept;

        /// @brief Minuto actual [0, 59].
        int currentMinute() const noexcept;

        /// @brief Turno del día derivado de currentHour().
        TimeOfDay timeOfDay() const noexcept;

        /// @brief Zonas adyacentes. Referencia estable durante lifetime del modelo.
        const std::vector<std::string>& adjacentZones() const noexcept;

        /// @brief Zonas visitadas. Referencia estable durante lifetime del modelo.
        const std::vector<std::string>& visitedZones() const noexcept;

        /// @brief Total de zonas en el mapa.
        int totalZoneCount() const noexcept;

        /**
         * @brief Entidades detectables en la zona actual.
         * @details Punteros observadores — no transferir ownership.
         *          Ningún puntero es nullptr.
         * @return Referencia estable durante lifetime del modelo.
         */
        const std::vector<const WorldEntity*>& nearbyEntities() const noexcept;

        /// @brief Causa del último incremento de alerta. Puede ser vacío.
        const std::string& lastAlertCause() const noexcept;

        /// @}

        // =========================================================================
        /// @name Inventario — lectura
        // =========================================================================
        /// @{

        /// @brief Ítems actuales. size() <= inventoryCapacity() siempre.
        const std::vector<Item>& inventory() const noexcept;

        /// @brief Capacidad máxima del inventario.
        int inventoryCapacity() const noexcept;

        /// @brief true si inventory().size() == inventoryCapacity().
        bool isInventoryFull() const noexcept;

        /// @}

        // =========================================================================
        /// @name Reputación — lectura
        // =========================================================================
        /// @{

        /**
         * @brief Valor de reputación con una facción.
         * @param faction Facción a consultar.
         * @return Valor en [0, 100].
         */
        int repValue(Faction faction) const noexcept;

        /// @brief Facción con mayor reputación. En empate, la de menor índice.
        Faction dominantFaction() const noexcept;

        /// @}

        // =========================================================================
        /// @name Misiones — lectura
        // =========================================================================
        /// @{

        /**
         * @brief Misión activa del runner.
         * @return Puntero observador. nullptr si no hay misión activa.
         * @note No transferir ownership.
         */
        const Mission* activeMission() const noexcept;

        /// @brief Misiones completadas en la sesión.
        int completedMissions() const noexcept;

        /// @brief Misiones fallidas en la sesión.
        int failedMissions() const noexcept;

        /**
         * @brief Última misión fallida.
         * @return Puntero observador. nullptr si no hubo fallo.
         */
        const Mission* lastFailedMission() const noexcept;

        /// @}

        // =========================================================================
        /// @name Hackeo — lectura
        // =========================================================================
        /// @{

        /// @brief Intentos de hackeo restantes. En [0, maxHackAttempts()].
        int hackAttempts() const noexcept;

        /// @brief Intentos máximos por turno.
        int maxHackAttempts() const noexcept;

        /// @brief Costo en créditos por intento.
        int hackCost() const noexcept;

        /// @}

        // =========================================================================
        /// @name Sesión — lectura
        // =========================================================================
        /// @{

        /// @brief Comandos ejecutados. Monótonamente creciente.
        int commandCount() const noexcept;

        /// @brief Tiempo transcurrido desde construcción.
        std::chrono::seconds sessionDuration() const noexcept;

        /// @brief Historial de actividad.
        const std::vector<LogEntry>& actionLog() const noexcept;

        /// @brief true si el loop principal debe continuar.
        bool isRunning() const noexcept;

        /// @brief Sprints completados (gestionado por instructor).
        int sprintsCompleted() const noexcept;

        /// @brief PRs mergeados (gestionado por instructor).
        int mergedPRs() const noexcept;

        /// @}

        // =========================================================================
        /// @name Mutadores — solo para Commands autorizados
        // =========================================================================
        /// @{

        /**
         * @brief Incrementa el nivel de alerta en un paso.
         * @param cause Motivo del incremento. No debe ser vacío.
         * @pre !cause.empty()
         * @post alertLevel() incrementado, saturando en Maximum.
         * @post lastAlertCause() == cause
         */
        void incrementAlert(std::string cause);

        /**
         * @brief Descuenta créditos del runner.
         * @param amount Monto a descontar. Debe ser > 0.
         * @pre amount > 0
         * @pre credits() >= amount
         * @post credits() decrementado en amount.
         */
        void spendCredits(int amount);

        /**
         * @brief Registra una entrada en el log de actividad.
         * @param message Descripción del evento. No debe ser vacío.
         * @pre !message.empty()
         * @post actionLog() contiene la nueva entrada.
         */
        void logAction(std::string message);

        /**
         * @brief Incrementa el contador de comandos ejecutados.
         * @post commandCount() incrementado en 1.
         */
        void incrementCommandCount() noexcept;

        /**
         * @brief Decrementa los intentos de hackeo disponibles.
         * @pre hackAttempts() > 0
         * @post hackAttempts() decrementado en 1.
         */
        void consumeHackAttempt() noexcept;

        /**
         * @brief Señaliza al loop principal que debe terminar.
         * @post isRunning() == false. Llamadas subsecuentes son no-op.
         */
        void quit() noexcept;

        /// @}

    private:
        // =========================================================================
        // Estado del runner
        // =========================================================================

        std::string m_playerName; ///< Nombre del runner. Nunca vacío.
        int m_hp;                 ///< HP actual. En [0, m_maxHp].
        int m_maxHp;              ///< HP máximo.
        int m_credits;            ///< Créditos. >= 0.

        // =========================================================================
        // Estado del mundo
        // =========================================================================

        std::string m_currentZone;                ///< Zona actual.
        AlertLevel m_alertLevel;                  ///< Nivel de alerta.
        int m_simulationDay;                      ///< Día de simulación.
        int m_currentHour;                        ///< Hora actual [0–23].
        int m_currentMinute;                      ///< Minuto actual [0–59].
        std::vector<std::string> m_adjacentZones; ///< Zonas adyacentes.
        std::vector<std::string> m_visitedZones;  ///< Zonas visitadas.
        int m_totalZoneCount;                     ///< Total de zonas del mapa.
        std::string m_lastAlertCause;             ///< Causa del último incremento.

        /**
         * @brief Entidades del mundo — owned por valor.
         * @note m_nearbyEntities apunta a elementos de este vector.
         *       El vector no se reordena mientras haya punteros activos.
         */
        std::vector<WorldEntity> m_worldEntities;

        /// @brief Punteros observadores a m_worldEntities. No owned.
        std::vector<const WorldEntity*> m_nearbyEntities;

        // =========================================================================
        // Inventario
        // =========================================================================

        std::vector<Item> m_inventory; ///< Ítems. size() <= m_inventoryCapacity.
        int m_inventoryCapacity;       ///< Capacidad máxima.

        // =========================================================================
        // Reputación
        // =========================================================================

        /// @brief Valores por facción, indexados por Faction. Cada uno en [0, 100].
        std::array<int, FACTION_COUNT> m_factionRep;

        // =========================================================================
        // Misiones
        // =========================================================================

        /// @brief Misión activa. nullptr si no hay. Owned por GameModel.
        std::unique_ptr<Mission> m_spActiveMission;

        /// @brief Última misión fallida. nullptr si no hubo. Owned por GameModel.
        std::unique_ptr<Mission> m_spLastFailed;

        int m_completedMissions; ///< Misiones completadas en la sesión.
        int m_failedMissions;    ///< Misiones fallidas en la sesión.

        // =========================================================================
        // Hackeo
        // =========================================================================

        int m_hackAttempts;    ///< Intentos restantes. En [0, m_maxHackAttempts].
        int m_maxHackAttempts; ///< Intentos máximos por turno.
        int m_hackCost;        ///< Créditos por intento.

        // =========================================================================
        // Sesión
        // =========================================================================

        int m_commandCount;
        std::chrono::time_point<std::chrono::steady_clock> m_sessionStart;
        std::vector<LogEntry> m_actionLog;
        int m_sprintsCompleted;
        int m_mergedPRs;
        bool m_running;

        // =========================================================================
        // Constantes de construcción
        // =========================================================================

        static constexpr int INITIAL_HP {100};
        static constexpr int INITIAL_CREDITS {250};
        static constexpr int INVENTORY_CAPACITY {10};
        static constexpr int MAX_HACK_ATTEMPTS {3};
        static constexpr int HACK_COST {15};
        static constexpr int INITIAL_REP {40};
        static constexpr int TOTAL_ZONE_COUNT {6};
        static constexpr int INITIAL_DAY {1};
        static constexpr int INITIAL_HOUR {23};
        static constexpr int INITIAL_MINUTE {41};
        static constexpr float CRITICAL_HP_RATIO {0.20f};
    };

} // namespace CyberpunkCba

#endif // _GAME_MODEL_HPP
