
#include "controller/commandCredits.hpp"
#include "model/gameModel.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>

/**
 * @file commandCredits.cpp
 * @brief Implementación de CommandCredits.
 *
 * @details
 * Muestra el equipo de desarrollo con padding dinámico de columnas
 * y métricas de sesión leídas desde GameModel.
 *
 */

namespace CyberpunkCba
{
    CommandCredits::CommandCredits(std::vector<TeamMember> team, std::string teamName)
        : m_team(std::move(team))
        , m_teamName(std::move(teamName))
    {
    }

    void CommandCredits::execute(GameModel& model)
    {
        if (m_team.empty())
        {
            std::cout << "ERROR: No hay miembros en el equipo de creditos.\n";
            return;
        }
        if (m_team.size() < 4)
        {
            std::cout << "ADVERTENCIA: Equipo Incompleto.\n";
        }
        else
        {
            std::cout << "=== EQUIPO DE DESARROLLO: " << m_teamName << " ===\n";
        }
        size_t maxRoleWidth = 0;
        for (const auto& member : m_team)
        {
            maxRoleWidth = std::max(maxRoleWidth, member.role.length());
        }
        std::ios::fmtflags oldFlags = std::cout.flags();
        char oldFill = std::cout.fill();

        for (const auto& member : m_team)
        {
            std::cout << std::left << std::setw(static_cast<int>(maxRoleWidth)) << member.role << " | " << member.name
                      << "\n";
        }

        std::cout.flags(oldFlags);
        std::cout.fill(oldFill);

        std::cout << "\n--- METRICAS DE LA SESION ---\n";
        std::cout << "Sprints completados: " << model.sprintsCompleted() << "\n";
        std::cout << "PRs fusionados:      " << model.mergedPRs() << "\n";
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
} // namespace CyberpunkCba
