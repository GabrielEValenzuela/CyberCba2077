#ifndef CYBERCBA_MISSIONS_MISSIONMAPSYSTEM_H
#define CYBERCBA_MISSIONS_MISSIONMAPSYSTEM_H
#include "MissionNode.hpp"

namespace cybercba::missions
{

class MissionMapSystem
{
public:
  MissionMapSystem();

  void agregarMision(const MissionNode& mision);
  structures::DynamicArray<MissionNode> misionesDisponibles() const;
  bool estaDesbloqueada(int idMision) const;

private:
  structures::DynamicArray<MissionNode> m_misiones;
};

}
#endif // CYBERCBA_MISSIONS_MISSIONMAPSYSTEM_H
