#ifndef CYBERCBA2077_MISSIONNODE_HPP
#define CYBERCBA2077_MISSIONNODE_HPP
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::missions
{


class MissionNode {
private:
    int id;
    std::string nombre;
    structures::DynamicArray<int>  misionesRequisito;

public:
    MissionNode(int id, const std::string& nombre);

    int getId() const;
    std::string getNombre() const;

    const structures::DynamicArray<int>& getMisionesRequisito() const;

    void agregarMisionRequisito(int idMision);
};


}
#endif // CYBERCBA2077_MISSIONNODE_HPP
