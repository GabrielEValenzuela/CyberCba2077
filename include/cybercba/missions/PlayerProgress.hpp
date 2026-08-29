#ifndef CYBERCBA_MISSIONS_PLAYERPROGRESS_H
#define CYBERCBA_MISSIONS_PLAYERPROGRESS_H

#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::missions
{
struct PlayerProgress
{
    structures::DynamicArray<int> misionesCompletadas;
    structures::DynamicArray<std::string> flagsActivos;


    bool misionCompletada(int idMision) const;

    bool flagActivo(const std::string& flag) const;
};
}

#endif // CYBERCBA_MISSIONS_PLAYERPROGRESS_H
