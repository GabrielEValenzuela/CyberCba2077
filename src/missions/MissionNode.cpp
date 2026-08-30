#include "cybercba/missions/IUnlockCondition.hpp"

#include <string>
#include <vector>
//
// Created by aguss on 30/8/2026.
//
MissionNode::MissionNode(int id, const std::string& nombre)
{
    m_id = id;
    m_nombre = nombre;
    return;
}
MissionNode::MissionNode() : m_id(00000), nombre("") {};

int MissionNode::id() const
{
    return m_id;
}

const std::string& MissionNode::nombre() const
{
    return m_nombre;
}
void agregarCondicion(const IUnlockCondition& condicion)
{
    m_condiciones.push_back(condicion);
    return;
}

const structures::DynamicArray<const IUnlockCondition*& condiciones() const
{
    return m_condiciones;
}