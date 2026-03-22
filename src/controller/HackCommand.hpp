#ifndef CYBERCBA2077_HACKCOMMAND_HPP
#define CYBERCBA2077_HACKCOMMAND_HPP
#include "controller/command.hpp"
#include <string>

namespace CyberpunkCba
{

    /**
    * @class HackCommand
    * @brief Verifica disponibilidad de hackeo.
    *
    * @details
    * Verifica si el jugador tiene créditos e intentos de hackeo disponibles.
    * Muestra mensajes específicos para cada caso: sin créditos, sin intentos, o ambos.
    * No modifica el modelo de juego.
    * Complejidad: O(1).
    */

    class HackCommand final : public Command
    {
    public:
        /**
         * @brief Constructor por defecto.
         * @details No recibe parámetros, ya que HackCommand no tiene estado.
         */

        HackCommand() = default;
        ~HackCommand() override = default;

        /**
         * @brief Verifica disponibilidad de hackeo y muestra mensajes específicos.
         * @details Si el jugador no tiene créditos ni intentos, muestra "SIN CREDITOS NI INTENTOS".
         *          Si solo le faltan créditos, muestra "SIN CREDITOS".
         *          Si solo le faltan intentos, muestra "SIN INTENTOS".
         *          Si tiene ambos, muestra "OK".
         * @param model No modificado por este comando.
         */
        void execute(GameModel& model) override;
        std::string name()        const override;
        std::string description() const override;
        std::string category()    const override;

    private:
        /// @brief Dificultad de hackeo para representación visual.
        enum class HackDifficulty { EASY, MEDIUM, HARD };
        /**
         * @brief Representación visual de la dificultad de hackeo.
         * @details Devuelve una barra de dificultad basada en el enum HackDifficulty.
         */

        std::string difficultyBar(HackDifficulty difficulty) const;

    };

} // namespace CyberpunkCba
#endif // HACKCOMMAND_HPP

