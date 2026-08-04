#ifndef PARTICIPANT_PANEL_HPP
#define PARTICIPANT_PANEL_HPP

/**
 * @file ParticipantPanel.hpp
 * @brief Reusable encounter participant card (template 7.2): name, integrity
 * bar, resource bar, and active status effects (via StatusPanel). Used for
 * both the player and the opponent in Screen::Encounter.
 */
#include "cybercba/Encounter.hpp"
#include "raylib.h"

class ParticipantPanel
{
public:
    /**
     * @param accent Border/name color (e.g. cyan for the protagonist, amber
     * for the opponent, matching docs/character-design.md).
     * @param active Whether it's currently this participant's turn.
     */
    static void draw(Rectangle bounds, const cybercba::EncounterParticipant& participant, bool active, Color accent, const Font* font);
};

#endif // PARTICIPANT_PANEL_HPP
