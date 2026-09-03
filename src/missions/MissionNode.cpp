#include "cybercba/missions/MissionNode.hpp"
//
// Created by aguss on 30/8/2026.
//

namespace cybercba::missions
{
MissionNode::MissionNode(int id, const std::string& nombre)
{
    m_id     = id;
    m_nombre = nombre;
}
MissionNode::MissionNode() : m_id(00000), m_nombre("") {}

int MissionNode::getId() const
{
    return m_id;
}

const std::string& MissionNode::getNombre() const
{
    return m_nombre;
}
void MissionNode::agregarCondicion(const IUnlockCondition& condicion)
{
    m_condiciones.pushBack(&condicion);
}

const structures::DynamicArray<const IUnlockCondition*>& MissionNode::condiciones() const
{
    return m_condiciones;
}
} // namespace cybercba::missions