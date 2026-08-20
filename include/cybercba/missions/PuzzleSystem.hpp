#ifndef CYBERCBA_MISSIONS_PUZZLESYSTEM_H
#define CYBERCBA_MISSIONS_PUZZLESYSTEM_H

#include <cstddef>
#include <cstdint>

#include "cybercba/structures/DirectedGraph.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include "cybercba/structures/Queue.hpp"
#include "cybercba/structures/Stack.hpp"

namespace cybercba::missions
{

// The three puzzle families used across VS-001 (§6). No fourth family is
// introduced without updating the spec (VS-001 §6).
enum class PuzzleFamily : std::uint8_t
{
    EnergyRedirection,        // §6.1 — DirectedGraph
    TransmissionDecoding,     // §6.2 — DynamicArray + Stack (undo)
    SurveillanceInterference, // §6.3 — Queue
};

// Common interface so MissionSystem (§12.1) can hold "whichever puzzle is
// currently active" polymorphically instead of switching on PuzzleFamily.
// Family-specific interaction (connect/applyMove/neutralizeNext) stays on
// each concrete class — only the state MissionSystem actually needs to poll
// every frame (solved/failed) belongs on the interface.
//
// Ownership: MissionSystem holds an IPuzzle* as an observer; the concrete
// puzzle instance is owned by whichever scene/system constructs it.
class IPuzzle
{
  public:
    virtual ~IPuzzle() = default;

    virtual bool isSolved() const = 0;
    virtual bool isFailed() const = 0;
};

// NOTE (VS-001 §12.2): these puzzle classes depend on the custom structures
// (DynamicArray/Queue/Stack/DirectedGraph), whose implementations are a
// student exercise and are intentionally not provided in this pass. This
// header/its .cpp compile against the structures' public API; the resulting
// object code will not link into a runnable executable until those bodies
// exist. This is expected per VS-001 §12.2/§15 and is not a defect of this
// pass.

// §6.1: rotate/connect nodes to form a path from source to destination.
class EnergyRedirectionPuzzle final : public IPuzzle
{
  public:
    EnergyRedirectionPuzzle(int nodeCount, int maxMoves);

    bool connect(structures::NodeId from, structures::NodeId to);

  private:
    bool isSolved() const override;
    bool isFailed() const override;

    structures::DirectedGraph<int> m_graph;
    int m_maxMoves;
    int m_movesUsed{};
    structures::NodeId m_sourceNode{};
    structures::NodeId m_destinationNode{};
};

// §6.2: reorder/match a symbol sequence within a limited number of turns;
// Stack<Symbol> backs an undo history.
class TransmissionDecodingPuzzle final : public IPuzzle
{
  public:
    TransmissionDecodingPuzzle(int symbolCount, int maxTurns);

    bool applyMove(int fromIndex, int toIndex);
    bool undo();

  private:
    bool isSolved() const override;
    bool isFailed() const override;

    structures::DynamicArray<int> m_symbols;
    structures::Stack<int> m_moveHistory;
    int m_maxTurns;
    int m_turnsUsed{};
};

// §6.3: drain a queue of surveillance events in order before attempts run
// out.
class SurveillanceInterferencePuzzle final : public IPuzzle
{
  public:
    SurveillanceInterferencePuzzle(int eventCount, int maxAttempts);

    bool neutralizeNext();

  private:
    bool isSolved() const override;
    bool isFailed() const override;

    structures::Queue<int> m_events;
    int m_maxAttempts;
    int m_attemptsUsed{};
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_PUZZLESYSTEM_H
