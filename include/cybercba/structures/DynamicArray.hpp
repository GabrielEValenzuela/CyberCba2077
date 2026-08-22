#ifndef CYBERCBA_STRUCTURES_DYNAMICARRAY_H
#define CYBERCBA_STRUCTURES_DYNAMICARRAY_H

#include <cstddef>

namespace cybercba::structures
{

/// @brief Growable, contiguous, custom sequence container (no STL containers used).
///
/// `DynamicArray` owns a single contiguous heap buffer of `TData` and grows
/// it (capacity doubling, starting at 4) whenever `pushBack` would overflow
/// the current capacity. It exists so gameplay code never needs
/// `std::vector` (docs/GPD.md §51, docs/TAD-001.md §10-11).
///
/// ## Ownership
/// A `DynamicArray<TData>` owns every element it stores. References
/// returned by `operator[]` are observers only: they are invalidated by any
/// operation that reallocates the buffer (a `pushBack` past capacity,
/// assignment, etc.) — see docs/TAD-001.md §12-14 for the project's
/// ownership vocabulary (owner / observer).
///
/// ## Rule of Five
/// Copy and move are both defined explicitly (docs/TAD-001.md §14): copy
/// performs a deep copy of the buffer (never two objects sharing one
/// `m_pBufferData`), move steals the pointer and leaves the source empty so
/// its destructor is a safe no-op.
///
/// ## Preconditions
/// `operator[](index)` requires `0 <= index < size()`; violating it is
/// undefined behavior, matching the contract this ADT was specified under
/// (docs/GPD.md §55).
///
/// @tparam TData Element type stored by the array. Must be copy-assignable
///     (used by growth/copy) and default-constructible (each slot in a
///     freshly-allocated `TData[]` buffer is default-constructed before
///     being overwritten).
template <typename TData> class DynamicArray final
{
  public:
    /// @brief Constructs an empty array.
    ///
    /// No allocation happens until the first `pushBack`; `m_pBufferData`
    /// stays `nullptr` and `size()`/`capacity()` are both `0`.
    DynamicArray() : m_pBufferData(nullptr), m_size(0), m_capacity(0) {}

    /// @brief Releases the owned buffer.
    ///
    /// Safe to call on a moved-from array because `delete[] nullptr` is a
    /// guaranteed no-op.
    ~DynamicArray()
    {
        delete[] m_pBufferData;
    }

    /// @brief Copy constructor: deep-copies `other`'s buffer.
    ///
    /// Never shares `m_pBufferData` with `other` — mutating one array after
    /// the copy never affects the other.
    ///
    /// @param other Array to copy from. Left unmodified.
    DynamicArray(const DynamicArray& other)
        : m_pBufferData(other.m_capacity == 0 ? nullptr : new TData[other.m_capacity]), m_size(other.m_size),
          m_capacity(other.m_capacity)
    {
        for (std::size_t i = 0; i < m_size; ++i)
        {
            m_pBufferData[i] = other.m_pBufferData[i];
        }
    }

    /// @brief Copy assignment: deep-copies `other`'s buffer.
    ///
    /// Guards against self-assignment (`a = a;`) and only releases the
    /// current buffer after the new one is fully allocated and copied, so a
    /// failed allocation never leaves `*this` without a valid buffer.
    ///
    /// @param other Array to copy from. Left unmodified.
    /// @return Reference to `*this`, to allow chained assignment.
    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this == &other)
        {
            return *this;
        }

        TData* pNewBufferData = other.m_capacity == 0 ? nullptr : new TData[other.m_capacity];
        for (std::size_t i = 0; i < other.m_size; ++i)
        {
            pNewBufferData[i] = other.m_pBufferData[i];
        }

        delete[] m_pBufferData;
        m_pBufferData = pNewBufferData;
        m_size        = other.m_size;
        m_capacity    = other.m_capacity;
        return *this;
    }

    /// @brief Move constructor: steals `other`'s buffer.
    ///
    /// `other` is left empty (`nullptr`, size 0, capacity 0) so its
    /// destructor releases nothing and any subsequent use of `other` behaves
    /// like a freshly-constructed empty array rather than undefined
    /// behavior.
    ///
    /// @param other Array to move from. Left empty; only safe to destroy,
    ///     reassign, or query (`size()`/`isEmpty()`) afterward.
    DynamicArray(DynamicArray&& other) noexcept
        : m_pBufferData(other.m_pBufferData), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.m_pBufferData = nullptr;
        other.m_size        = 0;
        other.m_capacity    = 0;
    }

    /// @brief Move assignment: releases this array's own buffer, then
    /// steals `other`'s.
    ///
    /// @param other Array to move from. Left empty, same postcondition as
    ///     the move constructor.
    /// @return Reference to `*this`, to allow chained assignment.
    DynamicArray& operator=(DynamicArray&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        delete[] m_pBufferData;
        m_pBufferData = other.m_pBufferData;
        m_size        = other.m_size;
        m_capacity    = other.m_capacity;

        other.m_pBufferData = nullptr;
        other.m_size        = 0;
        other.m_capacity    = 0;
        return *this;
    }

    /// @brief Appends `value` at the end, growing the buffer if needed.
    ///
    /// Growth doubles capacity (from a base of 4 on the first allocation)
    /// when the array is full. The new buffer is always allocated and
    /// populated before the old one is freed, so a failure mid-copy never
    /// leaves this array without a valid buffer.
    ///
    /// @param value Element to append. Copied into the array.
    void pushBack(const TData& value)
    {
        if (isFull())
        {
            const auto newCapacity = m_capacity == 0 ? 4 : m_capacity * 2;
            auto newBufferData     = new TData[newCapacity]; // Un owner temporal interno a la función, NO AL OBJ

            for (std::size_t i = 0; i < m_size; ++i)
            {
                newBufferData[i] = m_pBufferData[i];
            }
            delete[] m_pBufferData;
            m_pBufferData = newBufferData;
            m_capacity    = newCapacity;
        }
        m_pBufferData[m_size] = value;
        ++m_size;
    }

    /// @brief Removes the last element. No-op on an empty array.
    void popBack()
    {
        if (m_size > 0)
        {
            --m_size;
        }
    }

    /// @brief Removes every element without releasing the buffer.
    ///
    /// Capacity is kept as-is so subsequent `pushBack` calls don't need to
    /// reallocate.
    void clear()
    {
        m_size = 0;
    }

    /// @brief Reverses the array in place.
    ///
    /// Implemented as a recursive two-pointer swap (docs/squads/issues/#216:
    /// the recursion requirement for this task). Safe on empty arrays and
    /// single-element arrays — the recursive helper's base case handles
    /// both without recursing.
    void reverse()
    {
        if (m_size == 0)
        {
            return;
        }
        reverseRecursive(m_pBufferData, m_pBufferData + m_size - 1);
    }

    /// @brief Element access, mutable.
    ///
    /// @param index Zero-based position. Must satisfy `0 <= index <
    ///     size()`; out-of-range access is undefined behavior (docs/GPD.md
    ///     §55) — this operator does not bounds-check.
    /// @return Reference to the element at `index`. Invalidated by any
    ///     operation that reallocates the buffer (see class-level Ownership
    ///     notes).
    TData& operator[](std::size_t index)
    {
        return m_pBufferData[index];
    }

    /// @brief Element access, read-only. See the mutable overload for the
    /// full contract (bounds precondition, invalidation on reallocation).
    ///
    /// @param index Zero-based position, `0 <= index < size()`.
    /// @return Const reference to the element at `index`.
    const TData& operator[](std::size_t index) const
    {
        return m_pBufferData[index];
    }

    /// @brief Number of elements currently stored.
    /// @return Element count; always `<= capacity()`.
    std::size_t size() const
    {
        return m_size;
    }

    /// @brief Number of elements the current buffer can hold without
    /// reallocating.
    /// @return Capacity in elements; `0` if no buffer has been allocated
    ///     yet.
    std::size_t capacity() const
    {
        return m_capacity;
    }

    /// @brief Whether the array holds no elements.
    /// @return `true` iff `size() == 0`.
    bool isEmpty() const
    {
        return m_size == 0;
    }

  private:
    /// Owned heap buffer of `m_capacity` elements, or `nullptr` if nothing
    /// has been allocated yet. This is the only owner of this memory.
    TData* m_pBufferData;
    /// Number of elements actually stored (`<= m_capacity`).
    std::size_t m_size;
    /// Number of elements `m_pBufferData` can hold before a `pushBack`
    /// triggers a reallocation.
    std::size_t m_capacity;

    /// @brief Whether the next `pushBack` would need to grow the buffer.
    /// @return `true` iff `m_size == m_capacity`.
    bool isFull() const
    {
        return m_size == m_capacity;
    }

    /// @brief Recursive base of `reverse()`.
    ///
    /// Swaps `*pFirst` and `*pLast`, then recurses one step inward.
    /// `pFirst >= pLast` is the base case — using `>=` (not `==`) is what
    /// makes it correct for both even- and odd-sized ranges, since an
    /// even-sized range has its two pointers cross rather than coincide.
    ///
    /// @param pFirst Pointer to the current leftmost element of the
    ///     remaining range to reverse.
    /// @param pLast Pointer to the current rightmost element of the
    ///     remaining range to reverse.
    static void reverseRecursive(TData* pFirst, TData* pLast)
    {
        if (pFirst >= pLast)
        {
            return;
        }

        const TData temp = *pFirst;
        *pFirst          = *pLast;
        *pLast           = temp;

        reverseRecursive(pFirst + 1, pLast - 1);
    }
};

} // namespace cybercba::structures

#endif // CYBERCBA_STRUCTURES_DYNAMICARRAY_H
