#include "cybercba/missions/PuzzleSystem.hpp"

namespace cybercba::missions
{

EnergyRedirectionPuzzle::EnergyRedirectionPuzzle(int nodeCount, int maxMoves) : m_maxMoves(maxMoves)
{
    for (int i = 0; i < nodeCount; ++i)
    {
        const structures::NodeId node = m_graph.addNode(i);
        if (i == 0)
        {
            m_sourceNode = node;
        }
        if (i == nodeCount - 1)
        {
            m_destinationNode = node;
        }
    }
}

bool EnergyRedirectionPuzzle::connect(structures::NodeId from, structures::NodeId to)
{
    if (isFailed() || isSolved())
    {
        return false;
    }
    m_graph.addEdge(from, to);
    ++m_movesUsed;
    return true;
}

bool EnergyRedirectionPuzzle::isSolved() const
{
    return m_graph.hasEdge(m_sourceNode, m_destinationNode);
}

bool EnergyRedirectionPuzzle::isFailed() const
{
    return !isSolved() && m_maxMoves > 0 && m_movesUsed >= m_maxMoves;
}

TransmissionDecodingPuzzle::TransmissionDecodingPuzzle(int symbolCount, int maxTurns) : m_maxTurns(maxTurns)
{
    for (int i = 0; i < symbolCount; ++i)
    {
        m_symbols.pushBack(i);
    }
}

bool TransmissionDecodingPuzzle::applyMove(int fromIndex, int toIndex)
{
    if (isFailed() || isSolved())
    {
        return false;
    }
    const auto from = static_cast<std::size_t>(fromIndex);
    const auto to   = static_cast<std::size_t>(toIndex);
    const int moved = m_symbols[from];
    m_symbols[from] = m_symbols[to];
    m_symbols[to]   = moved;
    m_moveHistory.push(fromIndex);
    ++m_turnsUsed;
    return true;
}

bool TransmissionDecodingPuzzle::undo()
{
    if (m_moveHistory.isEmpty())
    {
        return false;
    }
    m_moveHistory.pop();
    return true;
}

bool TransmissionDecodingPuzzle::isSolved() const
{
    for (std::size_t i = 0; i < m_symbols.size(); ++i)
    {
        if (m_symbols[i] != static_cast<int>(i))
        {
            return false;
        }
    }
    return true;
}

bool TransmissionDecodingPuzzle::isFailed() const
{
    return !isSolved() && m_maxTurns > 0 && m_turnsUsed >= m_maxTurns;
}

SurveillanceInterferencePuzzle::SurveillanceInterferencePuzzle(int eventCount, int maxAttempts)
    : m_maxAttempts(maxAttempts)
{
    for (int i = 0; i < eventCount; ++i)
    {
        m_events.enqueue(i);
    }
}

bool SurveillanceInterferencePuzzle::neutralizeNext()
{
    if (isFailed() || isSolved())
    {
        return false;
    }
    m_events.dequeue();
    ++m_attemptsUsed;
    return true;
}

bool SurveillanceInterferencePuzzle::isSolved() const
{
    return m_events.isEmpty();
}

bool SurveillanceInterferencePuzzle::isFailed() const
{
    return !isSolved() && m_maxAttempts > 0 && m_attemptsUsed >= m_maxAttempts;
}

} // namespace cybercba::missions
