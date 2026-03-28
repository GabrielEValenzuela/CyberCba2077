#include "mapCommand.h"
#include "model/gameModel.hpp"
#include "common/types.hpp"
#include <iostream>
#include <algorithm>

namespace CyberpunkCba
{
    /**
     * @brief Inicialización de la topografía de Neo-Córdoba.
     */
    const std::vector<ZoneNode> MapCommand::MAP_ZONES
    {
        { "Sector 7",            { "Distrito Industrial", "Favela Norte", "Puerto Libre" } },
        { "Distrito Industrial", { "Sector 7",            "Zona Muerta"                 } },
        { "Favela Norte",        { "Sector 7",            "Mercado Negro"               } },
        { "Puerto Libre",        { "Sector 7",            "Zona Muerta", "Mercado Negro"} },
        { "Zona Muerta",         { "Distrito Industrial", "Puerto Libre"                } },
        { "Mercado Negro",       { "Favela Norte",        "Puerto Libre"                } },
    };

    const ZoneNode* MapCommand::findZone(const std::string& zoneName) const noexcept
    {
        for (const auto& zone : MAP_ZONES)
        {
            if (zone.name == zoneName) return &zone;
        }
        return nullptr;
    }

    std::string MapCommand::name() const { return "map"; }

    std::string MapCommand::description() const { return "Muestra el mapa de Neo-Córdoba."; }

    std::string MapCommand::category() const { return "mundo"; }

    void MapCommand::execute(GameModel& model)
    {
        const std::string& zonaActual { model.currentZone() };
        // El bloqueo se activa con alertas High o Maximum.
        const bool bloqueado { model.alertLevel() >= AlertLevel::High };

        std::cout << "\n  === [map] MAPA DE NEO-CÓRDOBA ===\n\n";

        const ZoneNode* pZonaActual { findZone(zonaActual) };
        if (pZonaActual == nullptr)
        {
            std::cout << "  [!] SEÑAL DEGRADADA — posición no identificada.\n\n";
            return;
        }

        std::cout << "  Posición : ★ " << zonaActual << "\n";
        std::cout << "  Visitadas: " << model.visitedZones().size()
                  << " / " << model.totalZoneCount() << "\n\n";

        for (const auto& zone : MAP_ZONES)
        {
            const bool esCurrent { zone.name == zonaActual };
            const auto& visitadas { model.visitedZones() };

            // Verificación de exploración previa.
            const bool esVisitada {
                std::find(visitadas.begin(), visitadas.end(), zone.name) != visitadas.end()
            };

            // Renderizado de glifos de estado.
            if (esCurrent)      std::cout << "  ★ ";
            else if (esVisitada) std::cout << "  · ";
            else                std::cout << "  ? ";

            std::cout << zone.name;

            // Feedback de restricción de red.
            if (bloqueado && !esCurrent)
            {
                std::cout << " [BLOQUEADA]";
            }

            std::cout << "\n";
        }
        std::cout << "\n";
    }

} // namespace CyberpunkCba