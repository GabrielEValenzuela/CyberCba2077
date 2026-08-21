#ifndef CYBERCBA_STRUCTURES_STACK_H
#define CYBERCBA_STRUCTURES_STACK_H

#include <cstddef>

namespace cybercba::structures
{

/// Student exercise: implement this ADT.
//
// Contract:
//   - Owns its storage. No STL containers.
//   - LIFO semantics: push/pop/top operate on the same end.
//   - Must define destructor, copy constructor, copy assignment, move constructor,
//     and move assignment per the Rule of Five.
//   - pop()/top() on an empty stack is a precondition violation; document the
//     chosen behavior (assert vs. defined error) in the implementation.
//
// Used by cybercba_core for: undo history in "descifrado de transmisión" puzzles
// and bounded state/navigation history.
//
// Ownership: a Stack<T> owns every element it stores. top() returns an observer
// reference only, invalidated after the corresponding pop().
template <typename T> class Stack final
{
  public:
    Stack();
    ~Stack();

    Stack(const Stack& other);
    Stack& operator=(const Stack& other);

    Stack(Stack&& other) noexcept;
    Stack& operator=(Stack&& other) noexcept;

    void push(const T& value);
    void pop();
    void clear();

    T& top();
    const T& top() const;

    std::size_t size() const;
    bool isEmpty() const;
};

} // namespace cybercba::structures

#endif // CYBERCBA_STRUCTURES_STACK_H
