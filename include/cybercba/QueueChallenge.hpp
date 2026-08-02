#pragma once

#include <array>
#include <string>

namespace cybercba {

enum class ChallengePhase { Intuition, Prediction, Guided, Independent, Debrief, Complete };
enum class ChallengeAction { Enqueue, Dequeue };
struct ChallengeFeedback { bool correct {false}; bool hintUsed {false}; std::string message; };
struct ChallengeResult { int correctPredictions {0}; int attempts {0}; int hintsUsed {0}; int accuracyPercent {0}; int credits {0}; };
struct QueueSnapshot { std::array<std::string, 4> values {}; int front {0}; int size {0}; ChallengePhase phase {ChallengePhase::Intuition}; int correct {0}; int attempts {0}; int hints {0}; };

class QueueChallenge {
public:
    QueueChallenge();
    ChallengePhase phase() const;
    int size() const;
    int capacity() const;
    int frontIndex() const;
    int rearIndex() const;
    const std::string& at(int logicalIndex) const;
    std::string expectedDequeue() const;
    ChallengeFeedback predict(const std::string& answer);
    ChallengeFeedback executeNext();
    ChallengeFeedback requestHint();
    bool undo();
    void restart();
    bool completed() const;
    ChallengeResult result() const;
private:
    void snapshot();
    void advancePhase();
    std::array<std::string, 4> m_items;
    int m_front {0};
    int m_size {4};
    ChallengePhase m_phase {ChallengePhase::Intuition};
    int m_correct {0};
    int m_attempts {0};
    int m_hints {0};
    std::array<QueueSnapshot, 16> m_history {};
    int m_historySize {0};
};
} // namespace cybercba
