//
// Created by gvalenzuela on 3/11/26.
//

#include "../../include/model/agent.hpp"

void Agent::showMeUp() const
{
    std::cout << "[Agente] nombre = " << m_name
              << " | salud=" << m_health << std::endl;
}
