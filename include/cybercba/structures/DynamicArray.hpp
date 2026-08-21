#ifndef CYBERCBA_STRUCTURES_DYNAMICARRAY_H
#define CYBERCBA_STRUCTURES_DYNAMICARRAY_H

#include <cstddef>

namespace cybercba::structures
{

/// Student exercise: implement this ADT.
//
// Contract:
//   - Owns a contiguous, dynamically-resized buffer of T. No STL containers.
//   - Growable random-access sequence: pushBack/popBack/operator[]/size/capacity.
//   - Must define destructor, copy constructor, copy assignment, move constructor,
//     and move assignment per the Rule of Five — deep-copies owned
//     memory, no accidental shallow copy of a raw buffer.
//   - Index access is a precondition: 0 <= index < size(). Out-of-range access is
//     undefined behavior unless the implementation documents otherwise.
//   - Growth strategy (capacity doubling, etc.) is an implementation detail; only
//     the public API below is a contract for the rest of the codebase.
//
// Ownership: a DynamicArray<T> owns every element it stores. Callers receiving
// references via operator[] are observers only; those references are invalidated
// by any operation that reallocates the buffer (pushBack past capacity, etc.).
template <typename T> class DynamicArray final
{
  public:
    DynamicArray();
    ~DynamicArray();

    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);

    DynamicArray(DynamicArray&& other) noexcept;
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    void pushBack(const T& value);
    void popBack();
    void clear();

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    std::size_t size() const;
    std::size_t capacity() const;
    bool isEmpty() const;
};

} // namespace cybercba::structures

#endif // CYBERCBA_STRUCTURES_DYNAMICARRAY_H
