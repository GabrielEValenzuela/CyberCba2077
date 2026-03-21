/**
 * @file commandCredits.cpp
 * @brief Implementación de la clase CommandCredits.
 */
#include "controller/commandCredits.hpp"
#include "model/gameModel.hpp" 
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace CyberpunkCba
{
    CommandCredits::CommandCredits(std::vector<TeamMember> team, std::string teamName)
        : m_team(std::move(team)), m_teamName(std::move(teamName)) 
    {
    }

    void CommandCredits::execute(GameModel& model) 
    {
        if (m_team.empty()) {
            std::cout << "ERROR: No hay miembros en el equipo de creditos.\n";
            return;
        }
        if (m_team.size() < 4) {
            std::cout << "ADVERTENCIA: Equipo Incompleto.\n";
        } else {
            std::cout << "=== EQUIPO DE DESARROLLO: " << m_teamName << " ===\n";
        }
        size_t maxRoleWidth = 0;
        for (const auto& member : m_team) {
            maxRoleWidth = std::max(maxRoleWidth, member.role.length());
        }
        for (const auto& member : m_team) {
            std::cout << std::left << std::setw(static_cast<int>(maxRoleWidth)) 
                      << member.role << " | " << member.name << "\n";
        }
        std::cout << "\n--- ESTADISTICAS DE LA SESION ---\n";
        std::cout << "Sprints Finalizados: " << model.sprintsCompleted() << "\n";
        std::cout << "PRs Mergeados:      " << model.mergedPRs() << "\n";
    }

    std::string CommandCredits::name() const 
    { 
        return "credits"; 
    }

    std::string CommandCredits::description() const 
    { 
        return "Muestra los creditos del equipo y metricas de desarrollo."; 
    }

    std::string CommandCredits::category() const 
    { 
        return "sistema"; 
    }
}
