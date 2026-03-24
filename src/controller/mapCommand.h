

#ifndef CYBERCBA2077_MAPCOMMAND_H
#define CYBERCBA2077_MAPCOMMAND_H

#include "controller/command.hpp"
#include <string>
#include <vector>

namespace CyberpunkCba
{
    /**
     * @brief Estructura para representar una zona del mapa.
     * Contiene el nombre y una lista de nombres de zonas adyacentes.
     */
    struct ZoneNode
    {
        std::string name;
        std::vector<std::string> connections;
    };

    /**
     * @class MapCommand
     * @brief Comando encargado de renderizar la interfaz visual del mapa.
     * Uso 'final' para evitar herencia de este comando y optimizar el devirtualizer.
     */
    class MapCommand final : public Command
    {
    public:
        // Constructor y destructor por defecto (usando RAII).
        MapCommand() = default;
        ~MapCommand() override = default;

    private:
        // Métodos obligatorios de la interfaz Command, marcados con override.
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

        /**
         * @brief Diccionario de datos del mapa de Neo-Córdoba.
         * Es static para que todas las instancias de MapCommand compartan la misma
         * estructura y no ocupar memoria extra en el stack.
         */
        static const std::vector<ZoneNode> MAP_ZONES;

        /**
         * @brief Método privado de búsqueda para validar la posición del runner.
         * @return Puntero constante a la zona o nullptr si hay error de señal (zona no existente).
         */
        const ZoneNode* findZone(const std::string& zoneName) const noexcept;
    };

} // namespace CyberpunkCba

#endif // CYBERCBA2077_MAPCOMMAND_H