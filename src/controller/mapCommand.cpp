#include "mapCommand.h"
#include "model/gameModel.hpp"
#include "common/types.hpp"
#include <iostream>
#include <algorithm>

namespace CyberpunkCba
{
    // Definición estática del mapa: zonas y sus conexiones directas.
    // Lo armo como vector de estructuras para cumplir con el requisito de no usar strings fijos.
    const std::vector<ZoneNode> MapCommand::MAP_ZONES
    {
        { "Sector 7",            { "Distrito Industrial", "Favela Norte", "Puerto Libre" } },
        { "Distrito Industrial", { "Sector 7",            "Zona Muerta"                 } },
        { "Favela Norte",        { "Sector 7",            "Mercado Negro"               } },
        { "Puerto Libre",        { "Sector 7",            "Zona Muerta", "Mercado Negro"} },
        { "Zona Muerta",         { "Distrito Industrial", "Puerto Libre"                } },
        { "Mercado Negro",       { "Favela Norte",        "Puerto Libre"                } },
    };

    // Función auxiliar para buscar una zona por nombre dentro del vector estático.
    // Retorna un puntero a la zona o nullptr si el runner está en una zona "fantasma".
    const ZoneNode* MapCommand::findZone(const std::string& zoneName) const noexcept
    {
        for (const auto& zone : MAP_ZONES)
        {
            if (zone.name == zoneName)
            {
                return &zone;
            }
        }
        return nullptr;
    }

    // Identificador del comando para el CommandRegistry.
    std::string MapCommand::name() const
    {
        return "map";
    }

    // Descripción breve que se muestra cuando el usuario tipea 'help'.
    std::string MapCommand::description() const
    {
        return "Muestra el mapa de Neo-Córdoba.";
    }

    // Categoría para agrupar el comando en la interfaz de ayuda.
    std::string MapCommand::category() const
    {
        return "mundo";
    }

    // Lógica principal de ejecución del comando.
    void MapCommand::execute(GameModel& model)
    {
        // Obtengo el estado actual desde el modelo (Posición y Nivel de Alerta).
        const std::string& zonaActual { model.currentZone() };
        const bool bloqueado { model.alertLevel() >= AlertLevel::High };

        std::cout << "\n  === MAPA DE NEO-CÓRDOBA ===\n\n";

        // Verifico si la zona actual existe en mi base de datos del mapa.
        // Si no existe, muestro el error de señal degradada según pide el issue.
        const ZoneNode* pZonaActual { findZone(zonaActual) };
        if (pZonaActual == nullptr)
        {
            std::cout << "  [!] SEÑAL DEGRADADA — posición no identificada.\n\n";
            return;
        }

        // Imprimo la info de cabecera: posición actual y el progreso de exploración.
        std::cout << "  Posición : ★ " << zonaActual << "\n";
        std::cout << "  Visitadas: " << model.visitedZones().size()
                  << " / " << model.totalZoneCount() << "\n\n";

        // Itero sobre todas las zonas definidas para dibujar el mapa dinámico.
        for (const auto& zone : MAP_ZONES)
        {
            // Chequeo si la zona que estoy iterando es donde está parado el jugador.
            const bool esCurrent { zone.name == zonaActual };

            // Busco si la zona ya fue visitada anteriormente consultando el modelo.
            // Esto evita el warning de 'unused variable' y le da feedback visual al usuario.
            const auto& visitadas { model.visitedZones() };
            const bool esVisitada {
                std::find(visitadas.begin(), visitadas.end(), zone.name) != visitadas.end()
            };

            // Lógica de iconos:
            // Estrella para posición actual, punto para visitada, signo de pregunta para desconocida.
            if (esCurrent)
                std::cout << "  ★ ";
            else if (esVisitada)
                std::cout << "  · ";
            else
                std::cout << "  ? ";

            std::cout << zone.name;

            // Si el nivel de alerta es High o Maximum, marco las otras zonas como bloqueadas.
            if (bloqueado && !esCurrent)
            {
                std::cout << " [BLOQUEADA]";
            }

            std::cout << "\n";
        }
        std::cout << "\n";
    }

} // namespace CyberpunkCba