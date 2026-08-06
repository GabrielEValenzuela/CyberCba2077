#include "cybercba/EncounterContent.hpp"

namespace cybercba
{

    Encounter buildEmmaHackingEncounter()
    {
        std::vector<EncounterParticipant> participants;
        participants.push_back({"emma", "Emma", true, 100, 100, 60, "link", 10, false, {}});
        participants.push_back({"relay", "Rele de interferencia", false, 60, 60, 0, "trace", 6, false, {}});

        std::vector<ActionDefinition> actions;
        actions.push_back({"analyze", "Analizar nodo", {"", 0}, 0, "", 0});
        actions.push_back({"isolate", "Aislar ruido", {"link", 10}, 15, "", 0});
        actions.push_back({"redirect", "Redirigir senal", {"link", 20}, 25, "", 0});
        actions.push_back({"hide", "Ocultar rastro", {"link", 15}, 5, "hidden", 1});
        actions.push_back({"extract", "Extraer fragmento", {"link", 25}, 30, "", 0});
        actions.push_back({"relay_pulse", "Pulso del rele", {"", 0}, 10, "", 0});

        return Encounter(EncounterType::Hacking, participants, actions);
    }

    Encounter buildMaggaCombatEncounter()
    {
        std::vector<EncounterParticipant> participants;
        participants.push_back({"magga", "Magga", true, 100, 100, 60, "momentum", 10, false, {}});
        participants.push_back({"drone", "Dron de seguridad", false, 50, 50, 0, "energy", 8, false, {}});

        std::vector<ActionDefinition> actions;
        actions.push_back({"strike", "Golpe", {"momentum", 15}, 20, "", 0});
        actions.push_back({"guard", "Guardia", {"", 0}, 0, "guarding", 1});
        actions.push_back({"dodge", "Esquivar", {"momentum", 10}, 0, "evasive", 1});
        actions.push_back({"tactical_impact", "Impacto tactico", {"momentum", 30}, 30, "guard_broken", 2});
        actions.push_back({"charge_shot", "Disparo cargado", {"", 0}, 15, "", 0});
        actions.push_back({"rush", "Embestida", {"", 0}, 25, "off_balance", 1});

        return Encounter(EncounterType::Combat, participants, actions);
    }

} // namespace cybercba
