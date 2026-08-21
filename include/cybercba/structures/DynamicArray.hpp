#ifndef CYBERCBA_STRUCTURES_DYNAMICARRAY_H
#define CYBERCBA_STRUCTURES_DYNAMICARRAY_H

#include <cstddef>

namespace cybercba::structures
{

/// Growable, contiguous, custom sequence container (no STL containers used).
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
template <typename TData> class DynamicArray final
{
  public:
    /// Constructs an empty array: no allocation happens until the first
    /// `pushBack`.
    DynamicArray() : m_pBufferData(nullptr), m_size(0), m_capacity(0)
    {
    }

    /// Releases the owned buffer. Safe to call on a moved-from array because
    /// `delete[] nullptr` is a guaranteed no-op.
    ~DynamicArray()
    {
        delete[] m_pBufferData;
    }

    /// Deep-copies `other`'s buffer; never shares `m_pBufferData` with it.
    DynamicArray(const DynamicArray& other)
        : m_pBufferData(other.m_capacity == 0 ? nullptr : new TData[other.m_capacity]), m_size(other.m_size),
          m_capacity(other.m_capacity)
    {
        for (std::size_t i = 0; i < m_size; ++i)
        {
            m_pBufferData[i] = other.m_pBufferData[i];
        }
    }

    /// Deep-copy assignment. Guards against self-assignment (`a = a;`) and
    /// only releases the current buffer after the new one is fully copied.
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

    /// Steals `other`'s buffer; `other` is left empty so its destructor
    /// releases nothing.
    DynamicArray(DynamicArray&& other) noexcept
        : m_pBufferData(other.m_pBufferData), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.m_pBufferData = nullptr;
        other.m_size        = 0;
        other.m_capacity    = 0;
    }

    /// Move assignment. Releases this array's own buffer, then steals
    /// `other`'s.
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

    /// Appends `value`, growing the buffer (capacity doubling from a base of
    /// 4) when it is full. The new buffer is always allocated and populated
    /// before the old one is freed, so a failure mid-copy never leaves this
    /// array without a valid buffer.
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

    /// Removes the last element. No-op on an empty array.
    void popBack()
    {
        if (m_size > 0)
        {
            --m_size;
        }
    }

    /// Removes every element without releasing the buffer (capacity is kept
    /// so subsequent `pushBack` calls don't need to reallocate).
    void clear()
    {
        m_size = 0;
    }

    /// Reverses the array in place using a recursive two-pointer swap
    /// (docs/squads/issues/#216: the recursion requirement for this task).
    /// Safe on empty arrays and single-element arrays (the recursive helper's
    /// base case handles both).
    void reverse()
    {
        if (m_size == 0)
        {
            return;
        }
        reverseRecursive(m_pBufferData, m_pBufferData + m_size - 1);
    }

    TData& operator[](std::size_t index)
    {
        return m_pBufferData[index];
    }

    const TData& operator[](std::size_t index) const
    {
        return m_pBufferData[index];
    }

    std::size_t size() const
    {
        return m_size;
    }

    std::size_t capacity() const
    {
        return m_capacity;
    }

    bool isEmpty() const
    {
        return m_size == 0;
    }

  private:
    TData* m_pBufferData;
    std::size_t m_size;
    std::size_t m_capacity;

    bool isFull() const
    {
        return m_size == m_capacity;
    }

    /// Recursive base of `reverse()`: swaps the outer pair and recurses one
    /// step inward. `pFirst >= pLast` is the base case — using `>=` (not
    /// `==`) is what makes it correct for both even- and odd-sized ranges,
    /// since an even-sized range has its two pointers cross rather than
    /// coincide.
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
