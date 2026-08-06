#include "cybercba/QueueChallenge.hpp"
#include <algorithm>

namespace cybercba {
QueueChallenge::QueueChallenge() { restart(); }
ChallengePhase QueueChallenge::phase() const { return m_phase; }
int QueueChallenge::size() const { return m_size; }
int QueueChallenge::capacity() const { return static_cast<int>(m_items.size()); }
int QueueChallenge::frontIndex() const { return m_front; }
int QueueChallenge::rearIndex() const { return (m_front + m_size - 1) % capacity(); }
const std::string& QueueChallenge::at(int logicalIndex) const { return m_items[(m_front + logicalIndex) % capacity()]; }
std::string QueueChallenge::expectedDequeue() const { return m_size > 0 ? at(0) : ""; }
void QueueChallenge::snapshot() { if (m_historySize < static_cast<int>(m_history.size())) m_history[m_historySize++] = {m_items, m_front, m_size, m_phase, m_correct, m_attempts, m_hints}; }
void QueueChallenge::advancePhase() {
    if (m_phase == ChallengePhase::Intuition) m_phase = ChallengePhase::Prediction;
    else if (m_phase == ChallengePhase::Prediction) m_phase = ChallengePhase::Guided;
    else if (m_phase == ChallengePhase::Guided) m_phase = ChallengePhase::Independent;
    else if (m_phase == ChallengePhase::Independent) m_phase = ChallengePhase::Debrief;
    else if (m_phase == ChallengePhase::Debrief) m_phase = ChallengePhase::Complete;
}
ChallengeFeedback QueueChallenge::predict(const std::string& answer) {
    snapshot(); ++m_attempts; const bool correct = answer == expectedDequeue(); if (correct) ++m_correct;
    if (m_phase == ChallengePhase::Intuition) advancePhase();
    if (correct && m_phase == ChallengePhase::Prediction) advancePhase();
    return {correct, false, correct ? "Correcto: dequeue() retira el elemento del frente." : "Revisá el FRONT: una Queue respeta FIFO."};
}
ChallengeFeedback QueueChallenge::executeNext() {
    snapshot(); if (m_size == 0) { if (m_phase == ChallengePhase::Debrief) advancePhase(); return {true, false, "La cola está vacía; no hay solicitud para despachar."}; }
    const std::string served = expectedDequeue(); m_front = (m_front + 1) % capacity(); --m_size;
    if (m_phase == ChallengePhase::Intuition) advancePhase();
    else if (m_phase == ChallengePhase::Guided && m_size <= 2) advancePhase();
    else if (m_phase == ChallengePhase::Independent && m_size == 0) advancePhase();
    return {true, false, "dequeue() despachó " + served + " desde el FRONT."};
}
ChallengeFeedback QueueChallenge::requestHint() { ++m_hints; return {false, true, "Pista: el primer pedido que entra es el primero que sale (FIFO)."}; }
bool QueueChallenge::undo() { if (m_historySize == 0) return false; const QueueSnapshot& s = m_history[--m_historySize]; m_items=s.values; m_front=s.front; m_size=s.size; m_phase=s.phase; m_correct=s.correct; m_attempts=s.attempts; m_hints=s.hints; return true; }
void QueueChallenge::restart() { m_items = {"Hospital", "Estacion Guemes", "Nodo Alberdi", "Terminal"}; m_front=0; m_size=4; m_phase=ChallengePhase::Intuition; m_correct=0; m_attempts=0; m_hints=0; m_historySize=0; }
bool QueueChallenge::completed() const { return m_phase == ChallengePhase::Complete; }
ChallengeResult QueueChallenge::result() const { const int accuracy=m_attempts ? (100*m_correct)/m_attempts : 0; return {m_correct,m_attempts,m_hints,accuracy, std::max(25, 100 - m_hints*10)}; }
} // namespace cybercba
