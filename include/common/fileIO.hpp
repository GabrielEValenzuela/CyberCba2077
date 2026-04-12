#ifndef _FILE_IO_HPP
#define _FILE_IO_HPP

#include "common/types.hpp"

#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <vector>

/**
 * @file fileIO.hpp
 * @brief Structs de dominio  y funciones de lectura/escritura de archivos.
 *
 * @details
 * Este archivo define:
 * - Los structs nuevos introducidos en  (Transaction, SessionEntry, etc.).
 * - Las funciones `load*()` y `append*()`/`save*()` que los equipos usan
 *   para persistir y recuperar datos del juego.
 *
 * ## Política de errores
 * Todas las funciones de carga lanzan `FileIOException` si el archivo
 * existe pero no puede abrirse (permisos, corrupción). Si el archivo
 * no existe, el comportamiento se documenta por función.
 * Las líneas con formato inválido se ignoran silenciosamente — se registra
 * un aviso en `stderr` pero no se interrumpe la carga.
 *
 * ## Política de archivos de datos
 * Los archivos viven en el directorio `data/` relativo al ejecutable.
 * Usar las constantes `FileIO::Paths::*` para los paths canónicos.
 *
 * ## Formato general de todos los archivos
 * @code
 * # Líneas que empiezan con '#' son comentarios — se ignoran.
 * # Líneas vacías se ignoran.
 * # Campos separados por '|'.
 * # Encoding: UTF-8.
 * @endcode
 *
 * @author Exodus Systems Inc.
 * @version 0.2.0
 */

namespace CyberpunkCba
{

    // =============================================================================
    // Excepción de I/O
    // =============================================================================

    /**
     * @class FileIOException
     * @brief Excepción lanzada cuando una operación de I/O falla.
     *
     * @details
     * Se lanza cuando el archivo existe pero no puede abrirse o escribirse.
     * No se lanza cuando el archivo simplemente no existe — ese caso
     * se documenta por función.
     */
    class FileIOException : public std::runtime_error
    {
    public:
        /**
         * @brief Construye la excepción con mensaje descriptivo.
         * @param path Path del archivo que causó el error.
         * @param reason Motivo del fallo (ej: "permission denied").
         */
        explicit FileIOException(const std::filesystem::path& path, const std::string& reason)
            : std::runtime_error {"FileIO error [" + path.string() + "]: " + reason}
        {
        }
    };

    // =============================================================================
    // Structs de dominio
    // =============================================================================

    /**
     * @struct Transaction
     * @brief Transacción del historial de créditos.
     *
     * @details
     * ## Formato en wallet.txt
     * @code
     * # tipo|monto|concepto
     * ingreso|500|misión completada
     * gasto|80|deck de hackeo
     * @endcode
     *
     * @invariant !type.empty()
     * @invariant amount >= 0
     * @invariant !description.empty()
     */
    struct Transaction
    {
        std::string type;        ///< "ingreso" o "gasto"
        int amount;              ///< Monto en créditos. >= 0.
        std::string description; ///< Descripción de la transacción.
    };

    /**
     * @struct SessionEntry
     * @brief Registro de una sesión de juego.
     *
     * @details
     * ## Formato en session.txt
     * @code
     * # fecha|duracion_segundos|comandos_ejecutados
     * 2077-01-01|1820|34
     * 2077-01-02|3610|67
     * @endcode
     *
     * @invariant !date.empty()
     * @invariant durationSeconds >= 0
     * @invariant commandCount >= 0
     */
    struct SessionEntry
    {
        std::string date;    ///< Fecha en formato YYYY-MM-DD.
        int durationSeconds; ///< Duración de la sesión. >= 0.
        int commandCount;    ///< Comandos ejecutados. >= 0.
    };

    /**
     * @struct ZoneEntry
     * @brief Estado de una zona del mapa.
     *
     * @details
     * ## Formato en map.txt
     * @code
     * # nombre|visitada|desbloqueada
     * Sector 7|true|true
     * Distrito Industrial|false|true
     * Puerto Libre|false|false
     * @endcode
     *
     * @invariant !name.empty()
     */
    struct ZoneEntry
    {
        std::string name; ///< Nombre de la zona.
        bool visited;     ///< true si el runner ya la visitó.
        bool unlocked;    ///< true si está disponible para visitar.
    };

    /**
     * @struct MissionEntry
     * @brief Registro de una misión en el historial.
     *
     * @details
     * ## Formato en missions.txt
     * @code
     * # nombre|estado|recompensa|progreso
     * Infiltración Militech|Completed|500|100
     * Rescate en Favela|Failed|300|45
     * Entrega urgente|Pending|150|0
     * @endcode
     *
     * Estados válidos: `Pending`, `InProgress`, `Completed`, `Failed`.
     *
     * @invariant !name.empty()
     * @invariant reward >= 0
     * @invariant progressPercent en [0, 100]
     */
    struct MissionEntry
    {
        std::string name;     ///< Nombre de la misión.
        MissionStatus status; ///< Estado actual.
        int reward;           ///< Recompensa en créditos. >= 0.
        int progressPercent;  ///< Progreso en [0, 100].
    };

    /**
     * @struct EntityEntry
     * @brief Entidad detectada en un scan y persistida.
     *
     * @details
     * ## Formato en entities.txt
     * @code
     * # nombre|disposicion|distancia
     * Corporativo Militech|Hostile|120
     * Vendedor ambulante|Neutral|35
     * Netrunner aliado|Friendly|80
     * @endcode
     *
     * Disposiciones válidas: `Friendly`, `Neutral`, `Hostile`.
     *
     * @invariant !name.empty()
     * @invariant distanceMeters >= 0
     */
    struct EntityEntry
    {
        std::string name;              ///< Nombre de la entidad.
        EntityDisposition disposition; ///< Actitud hacia el runner.
        int distanceMeters;            ///< Distancia en metros. >= 0.
    };

    /**
     * @struct RepEntry
     * @brief Registro histórico de reputación con una facción.
     *
     * @details
     * ## Formato en reputation.txt
     * @code
     * # faccion|valor|fecha
     * Corporations|40|2077-01-01
     * Streeters|55|2077-01-02
     * NetRunners|40|2077-01-01
     * @endcode
     *
     * Facciones válidas: `Corporations`, `Streeters`, `NetRunners`.
     *
     * @invariant value en [0, 100]
     * @invariant !date.empty()
     */
    struct RepEntry
    {
        Faction faction;  ///< Facción.
        int value;        ///< Valor de reputación en [0, 100].
        std::string date; ///< Fecha del registro. Formato YYYY-MM-DD.
    };

    /**
     * @struct PurchaseEntry
     * @brief Registro de una compra en la tienda.
     *
     * @details
     * ## Formato en shop.txt
     * @code
     * # nombre|precio|cantidad|fecha
     * Stim|15|3|2077-01-01
     * Deck de hackeo|80|1|2077-01-02
     * @endcode
     *
     * @invariant !name.empty()
     * @invariant price >= 0
     * @invariant quantity >= 1
     * @invariant !date.empty()
     */
    struct PurchaseEntry
    {
        std::string name; ///< Nombre del ítem comprado.
        int price;        ///< Precio unitario en créditos. >= 0.
        int quantity;     ///< Cantidad comprada. >= 1.
        std::string date; ///< Fecha de la compra. Formato YYYY-MM-DD.
    };

    // =============================================================================
    // Paths canónicos
    // =============================================================================

    /**
     * @namespace FileIO
     * @brief Paths canónicos y funciones de I/O para todos los archivos de .
     */
    namespace FileIO
    {

        /**
         * @namespace Paths
         * @brief Paths canónicos de los archivos de datos del juego.
         *
         * @details
         * Todos los paths son relativos al directorio de ejecución del ejecutable.
         * En CI y en desarrollo local, el ejecutable corre desde `build/`, por lo
         * que los archivos viven en `build/data/`.
         *
         * Usar siempre estas constantes — nunca hardcodear paths en el código.
         */
        namespace Paths
        {

            static constexpr const char LOG[] {"data/log.txt"};
            static constexpr const char INVENTORY[] {"data/inventory.txt"};
            static constexpr const char MAP[] {"data/map.txt"};
            static constexpr const char MISSIONS[] {"data/missions.txt"};
            static constexpr const char ENTITIES[] {"data/entities.txt"};
            static constexpr const char REPUTATION[] {"data/reputation.txt"};
            static constexpr const char WALLET[] {"data/wallet.txt"};
            static constexpr const char SESSION[] {"data/session.txt"};
            static constexpr const char SHOP[] {"data/shop.txt"};

        } // namespace Paths

        // =============================================================================
        // Funciones de carga
        // =============================================================================

        /**
         * @brief Carga el log de actividad desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # hora|minuto|mensaje
         * 23|41|Sistema iniciado. Bienvenido, Ghost_47.
         * 23|43|alerta incrementada: Corporativo detectado
         * @endcode
         *
         * Si el archivo no existe, retorna vector vacío sin lanzar excepción.
         *
         * @param path Path al archivo. Por defecto `Paths::LOG`.
         * @return Vector de LogEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<LogEntry> loadLog(const std::filesystem::path& path = Paths::LOG);

        /**
         * @brief Carga el inventario desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # nombre|tipo|precio|cantidad
         * Deck de hackeo|Tech|80|1
         * Stim|Consumable|15|3
         * @endcode
         *
         * Tipos válidos: `Weapon`, `Consumable`, `Value`, `Tech`.
         * Ítems con precio o cantidad negativos se ignoran con aviso en stderr.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::INVENTORY`.
         * @return Vector de Item en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<Item> loadInventory(const std::filesystem::path& path = Paths::INVENTORY);

        /**
         * @brief Carga el mapa de zonas desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # nombre|visitada|desbloqueada
         * Sector 7|true|true
         * Puerto Libre|false|false
         * @endcode
         *
         * Valores booleanos válidos: `true`, `false` (case-insensitive).
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::MAP`.
         * @return Vector de ZoneEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<ZoneEntry> loadMap(const std::filesystem::path& path = Paths::MAP);

        /**
         * @brief Carga el historial de misiones desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # nombre|estado|recompensa|progreso
         * Infiltración Militech|Completed|500|100
         * Entrega urgente|Pending|150|0
         * @endcode
         *
         * Estados válidos: `Pending`, `InProgress`, `Completed`, `Failed`.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::MISSIONS`.
         * @return Vector de MissionEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<MissionEntry> loadMissions(const std::filesystem::path& path = Paths::MISSIONS);

        /**
         * @brief Carga el historial de entidades detectadas desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # nombre|disposicion|distancia
         * Corporativo Militech|Hostile|120
         * Netrunner aliado|Friendly|80
         * @endcode
         *
         * Disposiciones válidas: `Friendly`, `Neutral`, `Hostile`.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::ENTITIES`.
         * @return Vector de EntityEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<EntityEntry> loadEntities(const std::filesystem::path& path = Paths::ENTITIES);

        /**
         * @brief Carga el historial de reputación desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # faccion|valor|fecha
         * Corporations|40|2077-01-01
         * Streeters|55|2077-01-02
         * @endcode
         *
         * Facciones válidas: `Corporations`, `Streeters`, `NetRunners`.
         * Entradas con valor fuera de [0, 100] se ignoran con aviso en stderr.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::REPUTATION`.
         * @return Vector de RepEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<RepEntry> loadReputation(const std::filesystem::path& path = Paths::REPUTATION);

        /**
         * @brief Carga el historial de transacciones de créditos desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # tipo|monto|concepto
         * ingreso|500|misión completada
         * gasto|80|deck de hackeo
         * @endcode
         *
         * Tipos válidos: `ingreso`, `gasto`.
         * Entradas con monto negativo se ignoran con aviso en stderr.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::WALLET`.
         * @return Vector de Transaction en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<Transaction> loadWallet(const std::filesystem::path& path = Paths::WALLET);

        /**
         * @brief Carga el historial de sesiones desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # fecha|duracion_segundos|comandos_ejecutados
         * 2077-01-01|1820|34
         * 2077-01-02|3610|67
         * @endcode
         *
         * Entradas con duración o comandos negativos se ignoran con aviso en stderr.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::SESSION`.
         * @return Vector de SessionEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<SessionEntry> loadSessions(const std::filesystem::path& path = Paths::SESSION);

        /**
         * @brief Carga el historial de compras desde archivo.
         *
         * @details
         * ## Formato esperado
         * @code
         * # nombre|precio|cantidad|fecha
         * Stim|15|3|2077-01-01
         * Deck de hackeo|80|1|2077-01-02
         * @endcode
         *
         * Entradas con precio negativo o cantidad < 1 se ignoran con aviso en stderr.
         * Si el archivo no existe, retorna vector vacío.
         *
         * @param path Path al archivo. Por defecto `Paths::SHOP`.
         * @return Vector de PurchaseEntry en el orden del archivo.
         * @throws FileIOException si el archivo existe pero no puede abrirse.
         */
        [[nodiscard]] std::vector<PurchaseEntry> loadShop(const std::filesystem::path& path = Paths::SHOP);

        // =============================================================================
        // Funciones de escritura
        // =============================================================================

        /**
         * @brief Agrega una entrada al log de actividad.
         *
         * @details
         * Crea el archivo si no existe. Agrega al final (`std::ios::app`).
         * Formato escrito: `hora|minuto|mensaje\n`
         *
         * @param entry  Entrada a agregar.
         * @param path   Path al archivo. Por defecto `Paths::LOG`.
         * @pre !entry.message.empty()
         * @throws FileIOException si el archivo no puede abrirse o escribirse.
         */
        void appendLog(const LogEntry& entry, const std::filesystem::path& path = Paths::LOG);

        /**
         * @brief Guarda el inventario completo (sobreescribe el archivo).
         *
         * @details
         * Sobreescribe el archivo existente. Formato escrito por ítem:
         * `nombre|tipo|precio|cantidad\n`
         *
         * @param inventory Vector de ítems a guardar.
         * @param path      Path al archivo. Por defecto `Paths::INVENTORY`.
         * @throws FileIOException si el archivo no puede abrirse o escribirse.
         */
        void saveInventory(const std::vector<Item>& inventory, const std::filesystem::path& path = Paths::INVENTORY);

        /**
         * @brief Agrega una entrada al historial de sesiones.
         *
         * @details
         * Crea el archivo si no existe. Agrega al final.
         * Formato escrito: `fecha|duracion_segundos|comandos_ejecutados\n`
         *
         * @param entry Entrada a agregar.
         * @param path  Path al archivo. Por defecto `Paths::SESSION`.
         * @pre entry.durationSeconds >= 0
         * @pre entry.commandCount >= 0
         * @throws FileIOException si el archivo no puede abrirse o escribirse.
         */
        void appendSession(const SessionEntry& entry, const std::filesystem::path& path = Paths::SESSION);

        /**
         * @brief Agrega una entidad detectada al historial de entidades.
         *
         * @details
         * Crea el archivo si no existe. Agrega al final.
         * Formato escrito: `nombre|disposicion|distancia\n`
         *
         * @param entry Entidad a agregar.
         * @param path  Path al archivo. Por defecto `Paths::ENTITIES`.
         * @pre !entry.name.empty()
         * @throws FileIOException si el archivo no puede abrirse o escribirse.
         */
        void appendEntity(const EntityEntry& entry, const std::filesystem::path& path = Paths::ENTITIES);

        /**
         * @brief Agrega una transacción al historial de créditos.
         *
         * @details
         * Crea el archivo si no existe. Agrega al final.
         * Formato escrito: `tipo|monto|concepto\n`
         *
         * @param entry Transacción a agregar.
         * @param path  Path al archivo. Por defecto `Paths::WALLET`.
         * @pre entry.amount >= 0
         * @pre !entry.concept.empty()
         * @throws FileIOException si el archivo no puede abrirse o escribirse.
         */
        void appendTransaction(const Transaction& entry, const std::filesystem::path& path = Paths::WALLET);

        /**
         * @brief Agrega una compra al historial de la tienda.
         *
         * @details
         * Crea el archivo si no existe. Agrega al final.
         * Formato escrito: `nombre|precio|cantidad|fecha\n`
         *
         * @param entry Compra a agregar.
         * @param path  Path al archivo. Por defecto `Paths::SHOP`.
         * @pre entry.price >= 0
         * @pre entry.quantity >= 1
         * @throws FileIOException si el archivo no puede abrirse o escribirse.
         */
        void appendPurchase(const PurchaseEntry& entry, const std::filesystem::path& path = Paths::SHOP);

    } // namespace FileIO
} // namespace CyberpunkCba

#endif // _FILE_IO_HPP
