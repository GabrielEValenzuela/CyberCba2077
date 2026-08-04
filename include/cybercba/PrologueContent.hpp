#pragma once

#include "cybercba/Dialogue.hpp"
#include "cybercba/Evidence.hpp"
#include "cybercba/MissionGraph.hpp"

namespace cybercba
{

    // Content for the v0.1.0 prologue ("La ultima transmision"). Populates the
    // Phase 1 domain foundation (MissionGraph/DialogueGraph/EvidenceJournal)
    // with the actual narrative beats: The Network, The Fireflies, the Night
    // of the Blackout, Emma/Magga's relationship, and Train 41.
    //
    // MissionGraph/EvidenceJournal carry per-session discovery/progress state,
    // so callers get a fresh instance per new game. DialogueGraph is static
    // narrative content shared by every session.
    MissionGraph buildPrologueMissionGraph();
    EvidenceJournal buildPrologueEvidenceCatalog();
    const DialogueGraph& prologueDialogue();

} // namespace cybercba
