#pragma once
#include <string>
#include "cybercba/GameSession.hpp"
#include "cybercba/QueueChallenge.hpp"
namespace cybercba {
enum class GameCommand { NewGame, ContinueGame, QueuePredictHospital, QueuePredictGuemes, QueueExecute, QueueHint, QueueUndo, QueueRestart, QueueAdvanceDebrief };
class GameController { public: explicit GameController(GameSession& session); ChallengeFeedback dispatch(GameCommand command); QueueChallenge& queueChallenge(); const QueueChallenge& queueChallenge() const; private: GameSession& m_session; QueueChallenge m_queue; };
} // namespace cybercba
