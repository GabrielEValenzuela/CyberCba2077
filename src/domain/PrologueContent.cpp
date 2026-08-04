#include "cybercba/PrologueContent.hpp"

namespace cybercba
{

    MissionGraph buildPrologueMissionGraph()
    {
        MissionGraph graph;
        graph.addNode({"refuge", MissionNodeType::Scene, false,
                       {{"rest_area", ""}, {"workshop", ""}, {"comms_room", ""}, {"archive", ""}, {"blackout", ""}}});
        graph.addNode({"rest_area", MissionNodeType::Investigation, true, {{"refuge", ""}}});
        graph.addNode({"workshop", MissionNodeType::Investigation, true, {{"refuge", ""}}});
        graph.addNode({"comms_room", MissionNodeType::Investigation, true, {{"refuge", ""}}});
        graph.addNode({"archive", MissionNodeType::Investigation, true, {{"refuge", ""}}});
        graph.addNode({"blackout", MissionNodeType::Scene, false, {{"power_recovery", ""}}});
        graph.addNode({"power_recovery", MissionNodeType::Encounter, false,
                       {{"fragmented_transmission", "power_restored"}}});
        graph.addNode({"fragmented_transmission", MissionNodeType::Dialogue, false,
                       {{"protagonist_encounter", ""}}});
        graph.addNode({"protagonist_encounter", MissionNodeType::Encounter, false,
                       {{"narrative_choice", "encounter_resolved"}}});
        graph.addNode({"narrative_choice", MissionNodeType::Decision, false, {{"exterior_route", "choice_made"}}});
        graph.addNode({"exterior_route", MissionNodeType::Scene, false, {{"security_control", ""}}});
        graph.addNode({"security_control", MissionNodeType::Decision, false,
                       {{"neometro_entrance", "obstacle_cleared"}}});
        graph.addNode({"neometro_entrance", MissionNodeType::Scene, false, {{"prologue_complete", ""}}});
        graph.addNode({"prologue_complete", MissionNodeType::Transition, false, {}});
        graph.setStart("refuge");
        return graph;
    }

    EvidenceJournal buildPrologueEvidenceCatalog()
    {
        EvidenceJournal journal;
        journal.registerItem({"photo", "Fotografia deteriorada", "Emma y Magga antes del apagon. Todavia se los ve como pareja.",
                               "img.evidence.photo", "refuge", {"relationship"}, "trust_established"});
        journal.registerItem({"insignia", "Simbolo de La Luciernaga",
                               "Una luciernaga trazada sobre un circuito. Marcaba rutas seguras fuera de la vigilancia de La Red.",
                               "img.evidence.insignia", "workshop", {"fireflies"}, ""});
        journal.registerItem({"network_map", "Mapa de acceso de La Red",
                               "Santa Maria, San Justo y un acceso clausurado al Neometro por la Linea Oeste.",
                               "img.evidence.network_map", "comms_room", {"network"}, ""});
        journal.registerItem({"unsent_message", "Mensaje nunca enviado",
                               "Un borrador incompleto, reescrito muchas veces y nunca despachado.", "img.evidence.unsent_message",
                               "archive", {"relationship"}, ""});
        journal.registerItem({"train41_record", "Registro del Tren 41",
                               "Registro oficial: el Tren 41 fue destruido la noche del apagon. No deberia existir señal en esa linea.",
                               "img.evidence.train41_record", "archive", {"mystery"}, "train41_known"});
        return journal;
    }

    const DialogueGraph& prologueDialogue()
    {
        static const DialogueGraph graph = []
        {
            DialogueGraph g;
            g.addLine({"opening_network", DialogueSpeaker::Narrator, "",
                       "Neo-Cordoba sobrevive gracias a La Red. Controla la energia, el transporte, la identidad, la "
                       "seguridad y las comunicaciones. Cuando falla, distritos enteros desaparecen.",
                       "", "", "", "opening_fireflies", {}});
            g.addLine({"opening_fireflies", DialogueSpeaker::Narrator, "",
                       "La Luciernaga fue una red clandestina de ayuda mutua. Restauraba servicios que La Red "
                       "abandonaba. Emma y Magga fueron parte de ella.",
                       "", "", "", "opening_blackout", {}});
            g.addLine({"opening_blackout", DialogueSpeaker::Narrator, "",
                       "La noche del apagon, La Luciernaga fue expuesta. Varios miembros desaparecieron. Emma y "
                       "Magga llegaron a puntos de evacuacion distintos, y cada una recibio pruebas de que la otra "
                       "la habia traicionado.",
                       "", "", "", "", {}});
            g.addLine({"evidence_photo", DialogueSpeaker::Protagonist, "", "Una fotografia deteriorada.",
                       "Magga todavia miraba a camara. Yo ya sabia que la fecha importaba.",
                       "Emma fingia que no estaba cansado. La fecha fue eliminada del archivo.", "", "", {}});
            g.addLine({"evidence_insignia", DialogueSpeaker::Narrator, "",
                       "El simbolo de La Luciernaga: una luciernaga trazada sobre un circuito.", "", "", "", "", {}});
            g.addLine({"evidence_network_map", DialogueSpeaker::Narrator, "",
                       "Mapa de acceso de La Red: Santa Maria, San Justo, y un acceso clausurado al Neometro.", "", "",
                       "", "", {}});
            g.addLine({"evidence_unsent_message", DialogueSpeaker::Protagonist, "", "Un mensaje nunca enviado.",
                       "Nunca envie ese mensaje. Decia demasiado.", "Lo escribi mil veces y lo borre mil veces.", "",
                       "", {}});
            g.addLine({"evidence_train41_record", DialogueSpeaker::Narrator, "",
                       "Registro oficial: el Tren 41 fue destruido la noche del apagon. No deberia existir señal "
                       "alguna en esa linea.",
                       "", "", "", "", {}});
            g.addLine({"transmission_fragment", DialogueSpeaker::OtherProtagonist, "",
                       "...Emma... ...Magga... ...Tren Cuarenta y Uno... ...no fue un accidente... ...no confies...",
                       "", "", "", "", {}});
            g.addLine({"transmission_complete", DialogueSpeaker::OtherProtagonist, "",
                       "No se cuanto tiempo va a durar este enlace. El Tren 41 volvio a aparecer en la red. Eso "
                       "significa que empezaron de nuevo. No confies en lo que recordas de mi. Encontrame.",
                       "", "", "", "choice_prompt", {}});
            g.addLine({"choice_prompt", DialogueSpeaker::Narrator, "", "Que haces con la señal?", "", "", "", "",
                       {{"reply", "Responder de inmediato", "", "chose_reply"},
                        {"analyze", "Analizar la señal", "", "chose_analyze"},
                        {"disconnect", "Desconectar el transmisor", "", "chose_disconnect"}}});
            g.addLine({"neometro_ending", DialogueSpeaker::Narrator, "",
                       "Esto no estaba aca. Alguien volvio. O nunca se fue.", "", "", "", "", {}});
            return g;
        }();
        return graph;
    }

} // namespace cybercba
