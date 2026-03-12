//
// Created by gvalenzuela on 3/11/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_AGENT_HPP
#define CYBERPUNK_CORDOBA_2077_AGENT_HPP

#include <iostream>
#include <string>
#include <utility>

/**
 * @class Agent
 * @brief Clase que modela a un agente genérico del juego
 */
class Agent
{
public:
    /**
     * @brief Construye un nuevo agente genérico
     * @param name Nombre del agente
     * @param health Salud del agente
     */
    Agent(std::string name, float health) : m_name(std::move(name)), m_health(health) {
    }

    /**
     * Destructor virtual porque uso herencia
     */
    virtual ~Agent() = default;

    /**
     * @brief Atacar a un enemigo
     */
    virtual void attack() const = 0;

    /**
     * @brief Mostrar información sobre el agente
     */
    virtual void showMeUp() const;

protected:
    std::string m_name; ///< Nombre del agente
    float m_health; ///< Salud del agente
};

#endif // CYBERPUNK_CORDOBA_2077_AGENT_HPP
