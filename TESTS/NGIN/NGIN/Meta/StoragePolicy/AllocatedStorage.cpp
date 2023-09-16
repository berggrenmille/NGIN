#include <gtest/gtest.h>
#include <NGIN/Meta/StoragePolicy/AllocatedStorage.hpp> // Replace with the actual path
#include <cstring>                                      // for std::memcpy
#include <cstddef>
using namespace NGIN;

namespace
{
    class SampleAllocator
    {
    public:
        size_t allocateCount = 0;
        size_t deallocateCount = 0;

        void *Allocate(size_t size, size_t alignment)
        {
            ++allocateCount;
            return std::aligned_alloc(alignment, size);
        }

        void Deallocate(void *ptr)
        {
            ++deallocateCount;
            std::free(ptr);
        }
    };

    bool destroyed = false;

    struct LargeType
    {
        char data[200];

        LargeType()
        {
            std::fill_n(data, 200, 'a');
            destroyed = false;
        }
        ~LargeType()
        {
            destroyed = true;
        }

        LargeType(LargeType &&other) noexcept
        {
            std::memcpy(data, other.data, 200);
            other.data[0] = 0;
        }
        LargeType &operator=(LargeType &&other)
        {
            std::memcpy(data, other.data, 200);
            other.data[0] = 0;
            return *this;
        }
    };

    struct SmallType
    {
        int x;
        SmallType() : x(42) { destroyed = false; }
        ~SmallType() { destroyed = true; }

        SmallType(SmallType &&other) noexcept : x(other.x) { other.x = 0; }
        SmallType &operator=(SmallType &&other) = default;
    };
}

class AllocatedStorageTest : public ::testing::Test
{
protected:
    SampleAllocator allocator;
};

TEST_F(AllocatedStorageTest, HandlesSmallTypeByValue)
{
    Meta::StoragePolicy::AllocatedStorage policy{SmallType(), allocator};

    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(AllocatedStorageTest, HandlesLargeTypeByValue)
{
    Meta::StoragePolicy::AllocatedStorage policy{LargeType(), allocator};

    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(AllocatedStorageTest, DestructorForSmallType)
{
    {
        Meta::StoragePolicy::AllocatedStorage policy{SmallType(), allocator};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    EXPECT_EQ(destroyed, true);
}

TEST_F(AllocatedStorageTest, DestructorForLargeType)
{
    {
        Meta::StoragePolicy::AllocatedStorage policy{LargeType(), allocator};
        LargeType *ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    EXPECT_EQ(destroyed, true);
}

TEST_F(AllocatedStorageTest, MoveConstructorForSmallType)
{
    {
        Meta::StoragePolicy::AllocatedStorage policy{SmallType(), allocator};
        Meta::StoragePolicy::AllocatedStorage policy2(std::move(policy));

        SmallType *ptr = static_cast<SmallType *>(policy2.get());
        EXPECT_EQ(ptr->x, 42);
    }
    EXPECT_EQ(destroyed, true);
}

TEST_F(AllocatedStorageTest, MoveConstructorForLargeType)
{
    {
        Meta::StoragePolicy::AllocatedStorage policy{LargeType(), allocator};
        Meta::StoragePolicy::AllocatedStorage policy2(std::move(policy));

        LargeType *ptr = static_cast<LargeType *>(policy2.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    EXPECT_EQ(destroyed, true);
}

TEST_F(AllocatedStorageTest, AllocateIsCalledForSmallType)
{
    Meta::StoragePolicy::AllocatedStorage policy{SmallType(), allocator};
    EXPECT_EQ(allocator.allocateCount, 1);
}

TEST_F(AllocatedStorageTest, AllocateIsCalledForLargeType)
{
    Meta::StoragePolicy::AllocatedStorage policy{LargeType(), allocator};
    EXPECT_EQ(allocator.allocateCount, 1);
}

TEST_F(AllocatedStorageTest, DeallocateIsCalledForSmallType)
{
    {
        Meta::StoragePolicy::AllocatedStorage policy{SmallType(), allocator};
    }
    EXPECT_EQ(allocator.deallocateCount, 1);
}

TEST_F(AllocatedStorageTest, DeallocateIsCalledForLargeType)
{
    {
        Meta::StoragePolicy::AllocatedStorage policy{LargeType(), allocator};
    }
    EXPECT_EQ(allocator.deallocateCount, 1);
}

TEST_F(AllocatedStorageTest, DeallocateNotCalledAfterMoveForSmallType)
{
    Meta::StoragePolicy::AllocatedStorage policy{SmallType(), allocator};
    Meta::StoragePolicy::AllocatedStorage policy2(std::move(policy));
    EXPECT_EQ(allocator.deallocateCount, 0);
}

TEST_F(AllocatedStorageTest, DeallocateNotCalledAfterMoveForLargeType)
{
    Meta::StoragePolicy::AllocatedStorage policy{LargeType(), allocator};
    Meta::StoragePolicy::AllocatedStorage policy2(std::move(policy));
    EXPECT_EQ(allocator.deallocateCount, 0);
}
