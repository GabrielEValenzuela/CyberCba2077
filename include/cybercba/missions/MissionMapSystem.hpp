#ifndef CYBERCBA2077_MISSIONMAPSYSTEM_HPP
#define CYBERCBA2077_MISSIONMAPSYSTEM_HPP
#include "MissionNode.hpp"

// misionesDisponibles(progreso actual) : lista

namespace cybercba::missions {

class MissionMapSystem {
private:
    structures::DynamicArray<MissionNode> misiones;

public:
    MissionMapSystem();

    void agregarMision(const MissionNode& mision);
    structures::DynamicArray<MissionNode> misionesDisponibles() const;
    bool estaDesbloqueada(int idMision) const;
};

}
#endif // CYBERCBA2077_MISSIONMAPSYSTEM_HPP
