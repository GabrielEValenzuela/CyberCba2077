#include "controller/RepCommand.hpp"
#include "model/gameModel.hpp"
#include "common/types.hpp"
#include <iostream>

namespace CyberpunkCba {

    void RepCommand::execute(GameModel& model) {
        // 1. REQUISITO: Estado sin reputación inicializada (todos en 0)
        bool allZero = true;
        for (int i = 0; i < FACTION_COUNT; ++i) {
            if (model.repValue(static_cast<CyberpunkCba::Faction>(i)) > 0) {
                allZero = false;
                break;
            }
        }

        if (allZero) {
            std::cout << "Sin reputación inicializada." << std::endl;
            return; // Cortamos acá, no mostramos barras vacías
        }

        std::cout << "\n--- REPUTACION DE FACCIONES ---" << std::endl;

        bool hasHostile = false;

        // 2. REQUISITO: Iteración sobre facciones via índice y cast
        for (int i = 0; i < FACTION_COUNT; ++i) {
            CyberpunkCba::Faction f = static_cast<CyberpunkCba::Faction>(i);

            int currentVal = model.repValue(f);

            // 3. REQUISITO: Derivar RepLevel via repLevelFromValue()
            RepLevel level = repLevelFromValue(currentVal);

            if (level == RepLevel::Hostile) {
                hasHostile = true; // Guardamos si hay algún hostil para el final
            }

            // Llamamos a nuestros métodos privados reutilizados
            std::string label = renderLabel(level);
            std::string bar = renderBar(currentVal, 100, 15);

            std::cout << "Faccion " << i << " [" << label << "]: "
                      << bar << " " << currentVal << "/100" << std::endl;
        }

        // 4. REQUISITO: La facción dominante se obtiene de dominantFaction()
        CyberpunkCba::Faction dominante = model.dominantFaction();
        std::cout << "\nFaccion Dominante actual: " << static_cast<int>(dominante) << std::endl;

        // 5. REQUISITO: Advertencia visible si alguna facción es Hostile
        if (hasHostile) {
            std::cout << "[ADVERTENCIA] Tienes una o más facciones en estado HOSTIL." << std::endl;
        }

        std::cout << std::endl;
    }

    // Metodo privado reutilizado para dibujar la barra
    std::string RepCommand::renderBar(int value, int maxValue, int width) const {
        if (maxValue <= 0) return ""; // Prevención de división por 0

        int filled = (value * width) / maxValue;

        // Límites de seguridad por las dudas
        if (filled > width) filled = width;
        if (filled < 0) filled = 0;

        std::string bar = "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) bar += "#";
            else bar += "-";
        }
        bar += "]";

        return bar;
    }

    // Metodo privado reutilizado para las etiquetas (Requisito: no switch en execute)
    std::string RepCommand::renderLabel(RepLevel level) const {
        switch (level) {
            case RepLevel::Hostile: return "Hostil";
            case RepLevel::Neutral: return "Neutral";
            case RepLevel::Allied: return "Aliado";
            default: return "Desconocido";
        }
    }

} // namespace CyberpunkCba