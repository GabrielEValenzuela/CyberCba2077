//
// Created by gvalenzuela on 3/11/26.
//

#include "../../include/model/mercenary.hpp"

void Mercenary::showMeUp() const
{
    std::cout << "[Mercenario] nombre="<<m_name
              << "| salud=" << m_health
              << "| fuerza=" << m_strength << std::endl;
}

void Mercenary::attack() const
{
    std::cout << "[Mercenario] nombre="<<m_name
              << "ejecuta ataque fisico con katana tactica"
              << "| fuerza=" << m_strength << std::endl;
}
