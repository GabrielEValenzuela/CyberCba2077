#ifndef CYBERCBA_STRUCTURES_QUEUE_H
#define CYBERCBA_STRUCTURES_QUEUE_H

#include <cstddef>

namespace cybercba::structures
{

/// Student exercise: implement this ADT.
//
// Contract:
//   - Owns its storage (linked nodes or a ring buffer — implementation detail).
//   - FIFO semantics: enqueue at the back, dequeue from the front.
//   - Must define destructor, copy constructor, copy assignment, move constructor,
//     and move assignment per the Rule of Five.
//   - dequeue()/front() on an empty queue is a precondition violation; document
//     the chosen behavior (assert vs. defined error) in the implementation.
//
// Used by cybercba_core for: surveillance-event puzzles (VS-001 §6.3), deferred
// dialogue/mission events.
//
// Ownership: a Queue<T> owns every element it stores. front() returns an observer
// reference only, invalidated after the corresponding dequeue().
template <typename T> class Queue final
{
  public:
    Queue();
    ~Queue();

    Queue(const Queue& other);
    Queue& operator=(const Queue& other);

    Queue(Queue&& other) noexcept;
    Queue& operator=(Queue&& other) noexcept;

    void enqueue(const T& value);
    void dequeue();
    void clear();

    T& front();
    const T& front() const;

    std::size_t size() const;
    bool isEmpty() const;
};

} // namespace cybercba::structures

#endif // CYBERCBA_STRUCTURES_QUEUE_H
