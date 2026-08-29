#ifndef CYBERCBA_MISSIONS_MISSIONNODE_H
#define CYBERCBA_MISSIONS_MISSIONNODE_H

#include "IUnlockCondition.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::missions
{
class MissionNode
{
public:
    MissionNode();
    MissionNode(int id, const std::string& nombre);

    int id() const;
    const std::string& nombre() const;
    void agregarCondicion(const IUnlockCondition& condicion);
    const structures::DynamicArray<const IUnlockCondition*>& condiciones() const;

private:
    int m_id;
    std::string m_nombre;
    structures::DynamicArray<const IUnlockCondition*> m_condiciones;
};

}

#endif // CYBERCBA_MISSIONS_MISSIONNODE_H
