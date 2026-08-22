#ifndef CYBERCBA_STRUCTURES_DIRECTEDGRAPH_H
#define CYBERCBA_STRUCTURES_DIRECTEDGRAPH_H

#include <cstddef>

namespace cybercba::structures
{

using NodeId = std::size_t;

constexpr NodeId INVALID_NODE_ID = static_cast<NodeId>(-1);

/// Student exercise: implement this ADT.
//
// Contract:
//   - Owns every node payload it stores. No STL containers (no adjacency list
//     built on std::vector/std::unordered_map).
//   - Nodes are identified by a stable NodeId returned from addNode(); the graph
//     assigns and owns these IDs, callers never construct their own.
//   - Directed edges: addEdge(from, to) creates a one-way connection; hasEdge and
//     neighbor traversal must respect direction.
//   - Must define destructor, copy constructor, copy assignment, move constructor,
//     and move assignment per the Rule of Five.
//   - Consumers (MissionSystem, PuzzleSystem "redirección de energía", VS-001
//     §6.1) depend only on this public API — never on internal node/edge storage.
//
// Used by cybercba_core for the central-map mission-unlock graph and the
// energy-redirection puzzle family.
template <typename T> class DirectedGraph final
{
  public:
    DirectedGraph();
    ~DirectedGraph();

    DirectedGraph(const DirectedGraph& other);
    DirectedGraph& operator=(const DirectedGraph& other);

    DirectedGraph(DirectedGraph&& other) noexcept;
    DirectedGraph& operator=(DirectedGraph&& other) noexcept;

    NodeId addNode(const T& value);
    void addEdge(NodeId from, NodeId to);
    bool hasEdge(NodeId from, NodeId to) const;

    T& nodeValue(NodeId id);
    const T& nodeValue(NodeId id) const;

    std::size_t nodeCount() const;
    std::size_t edgeCount() const;
};

} // namespace cybercba::structures

#endif // CYBERCBA_STRUCTURES_DIRECTEDGRAPH_H
