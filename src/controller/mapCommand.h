#ifndef CYBERCBA2077_MAPCOMMAND_H
#define CYBERCBA2077_MAPCOMMAND_H

#include "controller/command.hpp"
#include <string>
#include <vector>

/**
 * @file mapCommand.h
 * @brief Definición del comando de mapeo del entorno.
 * @author [Tu Nombre/Legajo]
 */

namespace CyberpunkCba
{
    /**
     * @struct ZoneNode
     * @brief Representación de un nodo dentro del grafo de navegación de Neo-Córdoba.
     */
    struct ZoneNode
    {
        std::string name;                ///< Identificador único de la zona.
        std::vector<std::string> connections; ///< Lista de adyacencia de zonas conectadas.
    };

    /**
     * @class MapCommand
     * @brief Comando encargado de renderizar la interfaz visual del mapa y estado de exploración.
     * * @details
     * Procesa la información del GameModel para proyectar un mapa dinámico que incluye:
     * - Marcador de posición actual (★).
     * - Estado de alerta y bloqueos de red.
     * - Registro de zonas visitadas vs. totales.
     * * ## Invariantes
     * - Operación de solo lectura (const-correctness) sobre el modelo.
     * - El diccionario MAP_ZONES es inmutable post-compilación.
     */
    class MapCommand final : public Command
    {
    public:
        MapCommand() = default;
        ~MapCommand() override = default;

        /**
         * @brief Ejecuta el renderizado del mapa en la salida estándar.
         * @param model Referencia al GameModel (fuente de verdad).
         * @pre model debe estar inicializado y poseer una zona válida.
         */
        void execute(GameModel& model) override;

        /// @brief Identificador único del comando.
        std::string name() const override;

        /// @brief Descripción para el sistema de ayuda.
        std::string description() const override;

        /// @brief Categoría de agrupación del comando.
        std::string category() const override;

    private:
        /**
         * @brief Base de datos estática de la topología del mundo.
         * @note Compartida por todas las instancias de la clase.
         */
        static const std::vector<ZoneNode> MAP_ZONES;

        /**
         * @brief Localiza un nodo de zona por su nombre.
         * @param zoneName Nombre de la zona a buscar.
         * @return Puntero constante al nodo o nullptr si hay error de señal.
         */
        const ZoneNode* findZone(const std::string& zoneName) const noexcept;
    };

} // namespace CyberpunkCba

#endif // CYBERCBA2077_MAPCOMMAND_H