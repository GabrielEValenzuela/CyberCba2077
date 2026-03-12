//
// Created by gvalenzuela on 3/11/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_MERCENARY_HPP
#define CYBERPUNK_CORDOBA_2077_MERCENARY_HPP
#include <utility>

#include "agent.hpp"

class Mercenary final : public Agent
{
public:
    Mercenary(std::string name, float health, int strength) : Agent(std::move(name), health), m_strength(strength) {
    }

    /**
     * @brief El ataque del mercenario utiliza una katana táctica
     */
    void attack() const override;

    /**
     * @brief Presentar mercenario
     */
    void showMeUp() const override;

    int getStrength() const noexcept;

private:
    int m_strength; ///< Fuerza del mercenario
};

#endif // CYBERPUNK_CORDOBA_2077_MERCENARY_HPP
