#ifndef CYBERCBA_MISSIONS_PLAYERPROGRESS_H
#define CYBERCBA_MISSIONS_PLAYERPROGRESS_H

#include "MissionNode.hpp"
#include "cybercba/structures/DynamicArray.hpp"

#include <algorithm>
#include <string>

namespace cybercba::missions
{
struct PlayerProgress
{
    structures::DynamicArray<MissionNode> misionesCompletadas;
    structures::DynamicArray<MissionNode> flagsActivos;

    bool misionCompletada(int idMision) const
    {
        return std::find(misionesCompletadas.begin(), misionesCompletadas.end(), idMision)
               != misionesCompletadas.end();
    }

    bool flagActivo(const std::string& flag) const
    {
        return std::find(flagsActivos.begin(), flagsActivos.end(), flag)
               != flagsActivos.end();
    }
};
};

#endif // CYBERCBA2077_PLAYERPROGRESS_HPP
