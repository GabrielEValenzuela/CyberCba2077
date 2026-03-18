#ifndef CYBERCBA2077_HACKCOMMAND_HPP
#define CYBERCBA2077_HACKCOMMAND_HPP

#include "controller/command.hpp"
#include <string>

namespace CyberpunkCba
{
    // Cuatro valores para mapear los cuatro AlertLevel del modelo
    enum class HackDifficulty { EASY, MEDIUM, HARD, CRITICAL };

    class HackCommand final : public Command
    {
    public:
        HackCommand() = default;
        ~HackCommand() override = default;

        // execute() debe ser público — CommandRegistry lo llama desde afuera
        void execute(GameModel& model) override;

        std::string name()        const override;
        std::string description() const override;
        std::string category()    const override;

    private:
        // Recibe el nivel como parámetro en vez de leer un miembro,
        // así no necesitamos guardar estado entre llamadas
        std::string difficultyBar(HackDifficulty difficulty) const;

        // Centraliza el mapeo AlertLevel → HackDifficulty
        HackDifficulty difficultyFromModel(const GameModel& model) const;
    };

} // namespace CyberpunkCba

#endif // CYBERCBA2077_HACKCOMMAND_HPP
