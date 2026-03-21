#ifndef _CREDITS_COMMAND_HPP
#define _CREDITS_COMMAND_HPP

/**
 * @file commandCredits.hpp
 * @brief Define el comando encargado de mostrar la información del equipo de desarrollo y las métricas de la sesión.
 */
#include "controller/command.hpp" 
#include <vector>
#include <string>

namespace CyberpunkCba
{
    class GameModel;

    /**
     * @struct TeamMember
     * @brief Representa a un miembro del equipo de desarrollo.
     */
    struct TeamMember {
        std::string name;
        std::string role;
    };

    /**
     * @class CommandCredits
     * @brief Comando que muestra los créditos del equipo de desarrollo y el estado actual del juego.
     */
    class CommandCredits final : public Command 
    {
    public:
        /**
         * @brief Constructor de la clase CommandCredits.
         * Inicializa el comando almacenando la lista de los miembros del equipo 
         * y el nombre del grupo de desarrollo.
         * @param team Vector que contiene a los miembros del equipo
         * @param teamName Nombre asignado al equipo de desarrollo
         */
        CommandCredits(std::vector<TeamMember> team, std::string teamName);

        /**
         * @brief Destructor por defecto.
         */
        ~CommandCredits() override = default;

    private: 
        // Métodos con override heredan de la clase Command
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

        std::vector<TeamMember> m_team;
        std::string m_teamName;
    };
}

#endif