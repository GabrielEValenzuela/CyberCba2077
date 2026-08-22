#include "cybercba/structures/DynamicArray.hpp"

#include <utility>

#include <gtest/gtest.h>

namespace cybercba::structures
{
namespace
{

TEST(DynamicArrayTest, ConstructedEmpty)
{
    const DynamicArray<int> array;
    EXPECT_TRUE(array.isEmpty());
    EXPECT_EQ(array.size(), 0u);
    EXPECT_EQ(array.capacity(), 0u);
}

TEST(DynamicArrayTest, PushBackSingleElement)
{
    DynamicArray<int> array;
    array.pushBack(42);
    EXPECT_FALSE(array.isEmpty());
    EXPECT_EQ(array.size(), 1u);
    EXPECT_EQ(array[0], 42);
}

TEST(DynamicArrayTest, PushBackGrowsPastInitialCapacity)
{
    DynamicArray<int> array;
    for (int i = 0; i < 10; ++i)
    {
        array.pushBack(i);
    }
    ASSERT_EQ(array.size(), 10u);
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(array[static_cast<std::size_t>(i)], i);
    }
}

TEST(DynamicArrayTest, FrontAndBackReturnEndpoints)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);
    array.pushBack(3);

    EXPECT_EQ(array.front(), 1);
    EXPECT_EQ(array.back(), 3);
}

TEST(DynamicArrayTest, FrontAndBackAreMutable)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);

    array.front() = 10;
    array.back()  = 20;

    EXPECT_EQ(array[0], 10);
    EXPECT_EQ(array[1], 20);
}

TEST(DynamicArrayTest, FrontEqualsBackOnSingleElement)
{
    DynamicArray<int> array;
    array.pushBack(42);

    EXPECT_EQ(array.front(), 42);
    EXPECT_EQ(array.back(), 42);
}

TEST(DynamicArrayTest, PopBackRemovesLastElement)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);
    array.popBack();
    EXPECT_EQ(array.size(), 1u);
    EXPECT_EQ(array[0], 1);
}

TEST(DynamicArrayTest, PopBackOnEmptyArrayIsNoOp)
{
    DynamicArray<int> array;
    array.popBack();
    EXPECT_TRUE(array.isEmpty());
}

TEST(DynamicArrayTest, ClearEmptiesWithoutChangingCapacity)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);
    const std::size_t capacityBeforeClear = array.capacity();
    array.clear();
    EXPECT_TRUE(array.isEmpty());
    EXPECT_EQ(array.capacity(), capacityBeforeClear);
}

TEST(DynamicArrayTest, CopyConstructorDeepCopies)
{
    DynamicArray<int> original;
    original.pushBack(1);
    original.pushBack(2);

    DynamicArray<int> copy(original);
    copy.pushBack(3);

    EXPECT_EQ(original.size(), 2u);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(original[0], 1);
    EXPECT_EQ(copy[0], 1);
}

TEST(DynamicArrayTest, CopyAssignmentDeepCopies)
{
    DynamicArray<int> original;
    original.pushBack(1);
    original.pushBack(2);

    DynamicArray<int> other;
    other.pushBack(99);
    other = original;
    other.pushBack(3);

    EXPECT_EQ(original.size(), 2u);
    EXPECT_EQ(other.size(), 3u);
    EXPECT_EQ(other[0], 1);
}

TEST(DynamicArrayTest, CopyAssignmentHandlesSelfAssignment)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);

    DynamicArray<int>* pArray = &array;
    *pArray                   = *pArray;

    ASSERT_EQ(array.size(), 2u);
    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 2);
}

TEST(DynamicArrayTest, MoveConstructorStealsBufferAndEmptiesSource)
{
    DynamicArray<int> original;
    original.pushBack(1);
    original.pushBack(2);

    DynamicArray<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 2u);
    EXPECT_EQ(moved[0], 1);
    EXPECT_TRUE(original.isEmpty());    // NOLINT(bugprone-use-after-move)
    EXPECT_EQ(original.capacity(), 0u); // NOLINT(bugprone-use-after-move)
}

TEST(DynamicArrayTest, MoveAssignmentStealsBufferAndEmptiesSource)
{
    DynamicArray<int> original;
    original.pushBack(1);
    original.pushBack(2);

    DynamicArray<int> target;
    target.pushBack(99);
    target = std::move(original);

    EXPECT_EQ(target.size(), 2u);
    EXPECT_EQ(target[0], 1);
    EXPECT_TRUE(original.isEmpty()); // NOLINT(bugprone-use-after-move)
}

TEST(DynamicArrayTest, ReverseOnEmptyArrayDoesNotCrash)
{
    DynamicArray<int> array;
    array.reverse();
    EXPECT_TRUE(array.isEmpty());
}

TEST(DynamicArrayTest, ReverseOnSingleElementIsUnchanged)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.reverse();
    ASSERT_EQ(array.size(), 1u);
    EXPECT_EQ(array[0], 1);
}

TEST(DynamicArrayTest, ReverseEvenSizedArray)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);
    array.pushBack(3);
    array.pushBack(4);

    array.reverse();

    ASSERT_EQ(array.size(), 4u);
    EXPECT_EQ(array[0], 4);
    EXPECT_EQ(array[1], 3);
    EXPECT_EQ(array[2], 2);
    EXPECT_EQ(array[3], 1);
}

TEST(DynamicArrayTest, ReverseOddSizedArray)
{
    DynamicArray<int> array;
    array.pushBack(1);
    array.pushBack(2);
    array.pushBack(3);
    array.pushBack(4);
    array.pushBack(5);

    array.reverse();

    ASSERT_EQ(array.size(), 5u);
    EXPECT_EQ(array[0], 5);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 3);
    EXPECT_EQ(array[3], 2);
    EXPECT_EQ(array[4], 1);
}

} // namespace
} // namespace cybercba::structures
