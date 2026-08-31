//
// Created by ludmila on 31/8/26.
//
#include "../../include/cybercba/game/CheckpointPuzzlePolicy.h"

namespace cybercba::game
{

bool CheckpointPuzzlePolicy::correspondeGuardar(GameStateId estado) const
{
    return estado == GameStateId::Puzzle;
}

} // namespace cybercba::game