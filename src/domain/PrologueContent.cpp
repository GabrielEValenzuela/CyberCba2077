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
        journal.registerItem({"insignia", "Simbolo de Las Luciernagas",
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
                       "En 2077, Neo-Cordoba no vive gracias a sus calles. Vive gracias a La Red. Enciende los "
                       "hospitales, abre las puertas, autoriza los viajes, confirma los nombres. Cuando La Red "
                       "falla, una ciudad se detiene. Cuando decide ignorarte, dejas de existir mucho antes de morir.",
                       "", "", "", "opening_fireflies", {}});
            g.addLine({"opening_fireflies", DialogueSpeaker::Narrator, "",
                       "Los cortes no eran errores. Eran prioridades. Entonces aparecieron Las Luciernagas. No "
                       "prometian salvar la ciudad. Solo se negaban a aceptar que algunas personas fueran "
                       "descartables. Emma y Magga fueron parte de ellas.",
                       "", "", "", "opening_blackout", {}});
            g.addLine({"opening_blackout", DialogueSpeaker::Narrator, "",
                       "La noche del apagon, Las Luciernagas fueron expuestas. Varios miembros desaparecieron. "
                       "Emma y Magga recibieron pruebas de que el otro habia traicionado al grupo. Nunca volvieron "
                       "a verse.",
                       "Una voz identica a la de Magga confirmaba la ubicacion del refugio. La clave era suya. Una "
                       "clave no es una persona, pero en ese momento eso no alcanzo.",
                       "Una orden firmada con la clave de Emma redirigio a parte del equipo hacia una emboscada. "
                       "No sabias quien la habia escrito. Sabias quien la habia firmado.",
                       "", "", {}});
            g.addLine({"evidence_photo", DialogueSpeaker::Protagonist, "", "Una fotografia deteriorada.",
                       "La guarde para recordar que no siempre desconfie de ella. En el reverso: \"Cuando todo se "
                       "apague, buscame donde todavia haya luz.\" Magga odiaba escribir cosas asi. Decia que "
                       "sonaban falsas.",
                       "Podria haberla tirado. Lo intente.", "", "", {}});
            g.addLine({"evidence_insignia", DialogueSpeaker::Narrator, "",
                       "El simbolo de Las Luciernagas: una luciernaga trazada sobre un circuito.",
                       "Luma decia que el simbolo no representaba una luz. Representaba a alguien respondiendo "
                       "desde la oscuridad.",
                       "La gente nos pedia milagros. Nosotros llegabamos con cables, cinta y baterias vencidas.",
                       "", "", {}});
            g.addLine({"evidence_network_map", DialogueSpeaker::Narrator, "",
                       "Mapa de acceso de La Red: Santa Maria, San Justo, y un acceso clausurado al Neometro.",
                       "Energia, transporte, salud, identidad. Todo conectado. Exodus decia que unirlo hacia a la "
                       "ciudad mas fuerte. Nunca aclararon que pasaba cuando podian desconectarte de todo.",
                       "Cada linea era un servicio. Cada color, una prioridad. Los barrios ricos siempre aparecian "
                       "primero.",
                       "", "", {}});
            g.addLine({"evidence_unsent_message", DialogueSpeaker::Protagonist, "", "Un mensaje nunca enviado.",
                       "Nunca envie ese mensaje. Decia demasiado.", "Lo escribi mil veces y lo borre mil veces.", "",
                       "", {}});
            g.addLine({"evidence_train41_record", DialogueSpeaker::Narrator, "", "Estado oficial: destruido.",
                       "Linea Oeste. Unidad cuarenta y uno. Destruccion confirmada.",
                       "El tren 41 no esta perdido. Esta muerto.", "", "", {}});
            g.addLine({"transmission_fragment", DialogueSpeaker::OtherProtagonist, "",
                       "...tren cuarenta y uno... no fue... accidente... no confies...",
                       "Magga... si escuchas... tren cuarenta y uno... no fue... accidente... no confies... en lo "
                       "que recordas...",
                       "Emma... tren cuarenta y uno... volvio a La Red... los registros... fueron alterados... no "
                       "confies... en mi todavia...",
                       "", "", {}});
            g.addLine({"transmission_complete", DialogueSpeaker::OtherProtagonist, "",
                       "No se cuanto tiempo va a durar este enlace. El Tren 41 volvio a aparecer en la red. Eso "
                       "significa que empezaron de nuevo. No confies en lo que recordas de mi. Encontrame.",
                       "No se cuanto tiempo va a seguir abierto este enlace. El tren 41 aparecio otra vez. Eso "
                       "significa que Exodus empezo de nuevo. No confies en lo que recordas de mi, porque yo "
                       "tampoco confio en lo que recuerdo de vos. Encontrame.",
                       "La señal no va a durar. El tren 41 volvio a aparecer dentro de La Red. Ese tren fue "
                       "destruido, dice el registro. Que no confies en lo que recordas de esa noche. Llegas cinco "
                       "años tarde, lo se. Encontrame.",
                       "", "choice_prompt", {}});
            g.addLine({"choice_prompt", DialogueSpeaker::Narrator, "", "Que haces con la señal?", "", "", "", "",
                       {{"reply", "Estoy en camino.", "", "chose_reply"},
                        {"analyze", "Primero voy a comprobar que seas vos.", "", "chose_analyze"},
                        {"disconnect", "No voy a seguir otra voz fabricada por La Red.", "", "chose_disconnect"}}});
            g.addLine({"neometro_ending", DialogueSpeaker::Narrator, "", "Esto no estaba aca. Alguien volvio. O nunca se fue.",
                       "Esta linea fue desconectada hace cinco años. Y esto fue pintado hoy.",
                       "Esa marca no estaba aca. Alguien volvio. O nunca se fue.", "", "", {}});
            return g;
        }();
        return graph;
    }

} // namespace cybercba
