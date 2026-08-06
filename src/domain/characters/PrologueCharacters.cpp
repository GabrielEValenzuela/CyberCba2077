#include "cybercba/CharacterModule.hpp"

#include "cybercba/characters/DronSeguridadCharacter.hpp"
#include "cybercba/characters/LumaCharacter.hpp"
#include "cybercba/characters/OrdenFalsaCharacter.hpp"
#include "cybercba/characters/PatrullaSeguridadCharacter.hpp"
#include "cybercba/characters/ReleInterferenciaCharacter.hpp"
#include "cybercba/characters/SistemaCharacter.hpp"
#include "cybercba/characters/SistemaVigilanciaCharacter.hpp"
#include "cybercba/characters/TomoCharacter.hpp"
#include "cybercba/characters/VozFalsaMaggaCharacter.hpp"

namespace cybercba
{

    CharacterRoster buildPrologueCharacterRoster()
    {
        CharacterRoster roster;
        roster.add(std::make_unique<characters::LumaCharacter>());
        roster.add(std::make_unique<characters::SistemaCharacter>());
        roster.add(std::make_unique<characters::VozFalsaMaggaCharacter>());
        roster.add(std::make_unique<characters::OrdenFalsaCharacter>());
        roster.add(std::make_unique<characters::ReleInterferenciaCharacter>());
        roster.add(std::make_unique<characters::DronSeguridadCharacter>());
        roster.add(std::make_unique<characters::SistemaVigilanciaCharacter>());
        roster.add(std::make_unique<characters::PatrullaSeguridadCharacter>());
        roster.add(std::make_unique<characters::TomoCharacter>());
        return roster;
    }

} // namespace cybercba
