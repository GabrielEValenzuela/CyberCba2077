#ifndef CYBERCBA_MISSIONS_MISSIONNODE_H
#define CYBERCBA_MISSIONS_MISSIONNODE_H

#include "cybercba/missions/IUnlockCondition.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::missions
{
class MissionNode
{
  public:
    MissionNode();
    MissionNode(int id, const std::string& nombre);
    int getId() const;
    const std::string& getNombre() const;
    void agregarCondicion(const IUnlockCondition& condicion);
    const structures::DynamicArray<const IUnlockCondition*>& condiciones() const;

  private:
    int m_id;
    std::string m_nombre;
    structures::DynamicArray<const IUnlockCondition*> m_condiciones;
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_MISSIONNODE_H
