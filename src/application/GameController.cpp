#include "cybercba/GameController.hpp"
namespace cybercba {
GameController::GameController(GameSession& session):m_session(session){}
ChallengeFeedback GameController::dispatch(GameCommand command) {
    switch(command) { case GameCommand::NewGame: m_session.startNewGame(); m_queue.restart(); return {true,false,"Nueva partida iniciada."}; case GameCommand::ContinueGame: return {m_session.progress().hasSave,false,"Partida cargada."}; case GameCommand::QueuePredictHospital:return m_queue.predict("Hospital"); case GameCommand::QueuePredictGuemes:return m_queue.predict("Estacion Guemes"); case GameCommand::QueueExecute:return m_queue.executeNext(); case GameCommand::QueueHint:return m_queue.requestHint(); case GameCommand::QueueUndo:return {m_queue.undo(),false,"Estado anterior restaurado."}; case GameCommand::QueueRestart:m_queue.restart(); return {true,false,"Mision reiniciada."}; case GameCommand::QueueAdvanceDebrief: return m_queue.executeNext(); } return {false,false,"Comando desconocido."};
}
QueueChallenge& GameController::queueChallenge(){return m_queue;} const QueueChallenge& GameController::queueChallenge() const{return m_queue;}
} // namespace cybercba
