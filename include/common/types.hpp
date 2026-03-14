#ifndef _TYPES_HPP
#define _TYPES_HPP

#include <cstdint>
#include <string>

/**
 * @file Types.hpp
 * @brief Tipos del dominio de Cyberpunk Córdoba 2077.
 *
 * @details
 * Vocabulario compartido del sistema. Enums, structs de valor y free
 * functions de conversión y derivación. Sin dependencias externas.
 *
 * ## Política
 * READ ONLY para equipos de alumnos.
 *
 * @author Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    // =============================================================================
    // AlertLevel
    // =============================================================================

    /**
     * @enum AlertLevel
     * @brief Nivel de alerta del mundo en la zona actual.
     *
     * @details
     * Representa presencia y actividad de fuerzas hostiles.
     * Afecta disponibilidad de acciones, NPCs y eventos del mundo.
     * Valores nunca negativos — uso de uint8_t explícito.
     */
    enum class AlertLevel : uint8_t
    {
        None = 0,   ///< Zona completamente despejada.
        Low = 1,    ///< Actividad menor — movimiento libre.
        Medium = 2, ///< Patrullas activas — precaución recomendada.
        High = 3,   ///< Presencia hostil confirmada.
        Maximum = 4 ///< Combate inminente — movimiento bloqueado.
    };

    /// @brief Cantidad de niveles de alerta válidos.
    constexpr uint8_t ALERT_LEVEL_COUNT {5};

    /**
     * @brief Convierte AlertLevel a string legible.
     * @param level Nivel a convertir.
     * @return String no vacío. "DESCONOCIDO" para valores fuera de rango.
     */
    std::string alertLevelToString(AlertLevel level);

    /**
     * @brief Incrementa AlertLevel en un paso, saturando en Maximum.
     * @param level Nivel actual.
     * @return Nivel incrementado o Maximum si ya estaba en Maximum.
     */
    AlertLevel incrementAlertLevel(AlertLevel level) noexcept;

    // =============================================================================
    // TimeOfDay
    // =============================================================================

    /**
     * @enum TimeOfDay
     * @brief Turno del día en la simulación.
     */
    enum class TimeOfDay : uint8_t
    {
        Dawn = 0, ///< 04:00 – 06:00
        Day = 1,  ///< 06:00 – 18:00
        Dusk = 2, ///< 18:00 – 20:00
        Night = 3 ///< 20:00 – 04:00
    };

    /**
     * @brief Convierte TimeOfDay a string legible.
     * @param tod Turno a convertir.
     * @return String no vacío.
     */
    std::string timeOfDayToString(TimeOfDay tod);

    /**
     * @brief Deriva TimeOfDay desde la hora actual.
     * @param hour Hora en rango [0, 23].
     * @pre hour en [0, 23].
     * @return TimeOfDay correspondiente.
     */
    TimeOfDay timeOfDayFromHour(int hour) noexcept;

    /**
     * @brief Calcula minutos hasta el próximo cambio de turno.
     * @param hour Hora actual [0, 23].
     * @param minute Minuto actual [0, 59].
     * @return Minutos hasta el próximo cambio de TimeOfDay.
     */
    int minutesUntilNextTurn(int hour, int minute) noexcept;

    // Constantes de hora para TimeOfDay
    constexpr int DAWN_START_HOUR {4};
    constexpr int DAWN_END_HOUR {6};
    constexpr int DAY_END_HOUR {18};
    constexpr int DUSK_END_HOUR {20};

    // Constantes de minutos para cálculo de turnos
    constexpr int DAWN_END_MINUTES {6 * 60};   // 06:00
    constexpr int DAY_END_MINUTES {18 * 60};   // 18:00
    constexpr int DUSK_END_MINUTES {20 * 60};  // 20:00
    constexpr int DAY_TOTAL_MINUTES {24 * 60}; // 1440

    // =============================================================================
    // Faction
    // =============================================================================

    /**
     * @enum Faction
     * @brief Facciones del mundo de Neo-Córdoba.
     */
    enum class Faction : uint8_t
    {
        Corporations = 0, ///< Megacorporaciones.
        Streeters = 1,    ///< Callejeros.
        NetRunners = 2    ///< NetRunners.
    };

    /// @brief Cantidad de facciones en el juego.
    constexpr uint8_t FACTION_COUNT {3};

    /**
     * @brief Convierte Faction a string legible.
     * @param faction Facción a convertir.
     * @return String no vacío.
     */
    std::string factionToString(Faction faction);

    // =============================================================================
    // RepLevel
    // =============================================================================

    /**
     * @enum RepLevel
     * @brief Nivel de reputación derivado de un valor [0, 100].
     */
    enum class RepLevel : uint8_t
    {
        Hostile = 0,  ///< [0,  29]
        Neutral = 1,  ///< [30, 59]
        Allied = 2,   ///< [60, 84]
        Legendary = 3 ///< [85, 100]
    };

    /**
     * @brief Convierte RepLevel a string legible.
     * @param level Nivel a convertir.
     * @return String no vacío.
     */
    std::string repLevelToString(RepLevel level);

    /**
     * @brief Deriva RepLevel desde un valor numérico.
     * @param value Valor en rango [0, 100].
     * @pre value en [0, 100].
     * @return RepLevel correspondiente.
     */
    RepLevel repLevelFromValue(int value) noexcept;

    // Umbrales de reputación
    constexpr int REP_NEUTRAL_THRESHOLD {30};
    constexpr int REP_ALLIED_THRESHOLD {60};
    constexpr int REP_LEGENDARY_THRESHOLD {85};

    // =============================================================================
    // MissionStatus
    // =============================================================================

    /**
     * @enum MissionStatus
     * @brief Estado de una misión.
     */
    enum class MissionStatus : uint8_t
    {
        Pending = 0,    ///< Aceptada pero no iniciada.
        InProgress = 1, ///< En ejecución activa.
        Completed = 2,  ///< Finalizada con éxito.
        Failed = 3      ///< Fallida.
    };

    /**
     * @brief Convierte MissionStatus a string legible.
     * @param status Estado a convertir.
     * @return String no vacío.
     */
    std::string missionStatusToString(MissionStatus status);

    // =============================================================================
    // ItemType
    // =============================================================================

    /**
     * @enum ItemType
     * @brief Tipo de ítem del inventario.
     */
    enum class ItemType : uint8_t
    {
        Weapon = 0,     ///< Arma.
        Consumable = 1, ///< Consumible.
        Value = 2,      ///< Solo para vender.
        Tech = 3        ///< Habilita acciones especiales.
    };

    /**
     * @brief Convierte ItemType a string legible.
     * @param type Tipo a convertir.
     * @return String no vacío.
     */
    std::string itemTypeToString(ItemType type);

    // =============================================================================
    // Structs de valor — POD: sin m_ prefix (Wazuh convention)
    // =============================================================================

    /**
     * @struct Item
     * @brief Ítem del inventario del runner.
     *
     * @invariant name no vacío.
     * @invariant price >= 0.
     * @invariant quantity >= 1.
     */
    struct Item
    {
        std::string name; ///< Nombre del ítem.
        ItemType type;    ///< Categoría.
        int price;        ///< Precio en créditos. >= 0.
        int quantity;     ///< Cantidad en stack. >= 1.
    };

    /**
     * @enum EntityDisposition
     * @brief Disposición de una entidad hacia el runner.
     */
    enum class EntityDisposition : uint8_t
    {
        Friendly = 0,
        Neutral = 1,
        Hostile = 2
    };

    /**
     * @brief Convierte EntityDisposition a string legible.
     * @param disposition Disposición a convertir.
     * @return String no vacío.
     */
    std::string entityDispositionToString(EntityDisposition disposition);

    /**
     * @struct WorldEntity
     * @brief Entidad detectable en la zona actual.
     *
     * @invariant name no vacío.
     * @invariant distanceMeters >= 0.
     */
    struct WorldEntity
    {
        std::string name;              ///< Identificador.
        EntityDisposition disposition; ///< Actitud hacia el runner.
        int distanceMeters;            ///< Distancia en metros. >= 0.
    };

    /**
     * @struct LogEntry
     * @brief Entrada del registro de actividad.
     *
     * @invariant hour en [-1, 23]. -1 indica timestamp no disponible.
     * @invariant minute en [0, 59] cuando hour != -1.
     * @invariant message no vacío.
     */
    struct LogEntry
    {
        int hour;            ///< Hora. -1 si no disponible → "--:--".
        int minute;          ///< Minuto.
        std::string message; ///< Descripción del evento.
    };

    /**
     * @struct Mission
     * @brief Misión del runner.
     *
     * @invariant progressPercent en [0, 100].
     * @invariant rewardCredits >= 0.
     * @invariant name no vacío.
     */
    struct Mission
    {
        std::string name;      ///< Nombre.
        std::string objective; ///< Descripción del objetivo.
        MissionStatus status;  ///< Estado actual.
        int rewardCredits;     ///< Recompensa. >= 0.
        int progressPercent;   ///< Avance [0–100].
    };

} // namespace CyberpunkCba

#endif // _TYPES_HPP
