#include <gtest/gtest.h>

#include "cybercba/Dialogue.hpp"

TEST(Dialogue, ProtagonistSpeakerResolvesToSelectedCharacterName)
{
    cybercba::DialogueGraph graph;
    graph.addLine({"intro", cybercba::DialogueSpeaker::Protagonist, "", "Tengo que salir.", "", "", "", "", {}});

    EXPECT_EQ(graph.resolveSpeakerName("intro", cybercba::CharacterId::Emma),
              cybercba::characterName(cybercba::CharacterId::Emma));
    EXPECT_EQ(graph.resolveSpeakerName("intro", cybercba::CharacterId::Magga),
              cybercba::characterName(cybercba::CharacterId::Magga));
}

TEST(Dialogue, UnselectedCharacterBecomesTransmissionSource)
{
    cybercba::DialogueGraph graph;
    graph.addLine({"transmission", cybercba::DialogueSpeaker::OtherProtagonist, "", "...no confies...", "", "", "",
                    "", {}});

    // If the player selected Emma, the voice on the transmission must be Magga.
    EXPECT_EQ(graph.resolveSpeakerName("transmission", cybercba::CharacterId::Emma),
              cybercba::characterName(cybercba::CharacterId::Magga));
    EXPECT_EQ(graph.resolveSpeakerName("transmission", cybercba::CharacterId::Magga),
              cybercba::characterName(cybercba::CharacterId::Emma));
}

TEST(Dialogue, ProtagonistVariantTextOverridesDefault)
{
    cybercba::DialogueGraph graph;
    graph.addLine({"reflect", cybercba::DialogueSpeaker::Narrator, "", "Texto por defecto.", "Version de Emma.",
                    "Version de Magga.", "", "", {}});

    EXPECT_EQ(graph.resolveText("reflect", cybercba::CharacterId::Emma), "Version de Emma.");
    EXPECT_EQ(graph.resolveText("reflect", cybercba::CharacterId::Magga), "Version de Magga.");
    EXPECT_EQ(graph.resolveText("reflect", cybercba::CharacterId::None), "Texto por defecto.");
}

TEST(Dialogue, UnknownLineResolvesToEmptyStrings)
{
    cybercba::DialogueGraph graph;
    EXPECT_EQ(graph.resolveText("missing", cybercba::CharacterId::Emma), "");
    EXPECT_EQ(graph.resolveSpeakerName("missing", cybercba::CharacterId::Emma), "");
    EXPECT_EQ(graph.find("missing"), nullptr);
}

TEST(Dialogue, DuplicateLineIdIsIgnored)
{
    cybercba::DialogueGraph graph;
    graph.addLine({"a", cybercba::DialogueSpeaker::Narrator, "", "primero", "", "", "", "", {}});
    graph.addLine({"a", cybercba::DialogueSpeaker::Narrator, "", "segundo", "", "", "", "", {}});
    EXPECT_EQ(graph.resolveText("a", cybercba::CharacterId::None), "primero");
}
