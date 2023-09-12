#include <gtest/gtest.h>
#include <NGIN/Memory/Allocator.hpp>
#include <NGIN/Meta/StoragePolicy.hpp>

using namespace NGIN;
namespace
{
    bool allocateCalled = false;
    bool deallocateCalled = false;
    bool deallocateAllCalled = false;
    bool ownsCalled = false;
}
// Define a MockAllocator for testing purposes
class MockAllocator
{
public:
    // Default constructor
    MockAllocator() = default;

    // Move constructor
    MockAllocator(MockAllocator &&other) noexcept
    {
        allocateCalled = false;
        deallocateCalled = false;
        deallocateAllCalled = false;
    }

    // Move assignment operator
    MockAllocator &operator=(MockAllocator &&other) noexcept
    {
        if (this != &other)
        {

            allocateCalled = false;
            deallocateCalled = false;
            deallocateAllCalled = false;
        }
        return *this;
    }

    void *Allocate(size_t size, size_t alignment, const std::source_location &location) const
    {
        allocateCalled = true;
        return nullptr; // Just returning nullptr for simplicity; you can modify if necessary
    }

    void Deallocate(void *ptr) const
    {
        deallocateCalled = true;
    }

    void DeallocateAll() const
    {
        deallocateAllCalled = true;
    }

    bool Owns(void *ptr) const
    {
        ownsCalled = true;
        return false;
    }
};

class AllocatorTest : public ::testing::Test
{
protected:
    MockAllocator mockAllocator;
    Memory::Allocator<NGIN::Meta::StoragePolicy::Dynamic> allocator;

    AllocatorTest() : allocator(std::move(mockAllocator)) {}
};

TEST_F(AllocatorTest, AllocateCalled)
{
    allocator.Allocate(128);
    EXPECT_TRUE(allocateCalled);
}

TEST_F(AllocatorTest, DeallocateCalled)
{
    allocator.Deallocate(nullptr); // Just using nullptr for simplicity
    EXPECT_TRUE(deallocateCalled);
}

TEST_F(AllocatorTest, DeallocateAllCalled)
{
    allocator.DeallocateAll();
    EXPECT_TRUE(deallocateAllCalled);
}

TEST_F(AllocatorTest, NewCalled)
{
    auto obj = allocator.New<int>();
    EXPECT_TRUE(allocateCalled);
    allocator.Delete(obj);
}

TEST_F(AllocatorTest, DeleteCalled)
{
    auto obj = new int(0);
    allocator.Delete(obj);
    delete obj;
    EXPECT_TRUE(deallocateCalled);
}

TEST_F(AllocatorTest, OwnsCalled)
{
    auto obj = new int(0);
    allocator.Owns(obj);
    delete obj;
    EXPECT_TRUE(ownsCalled);
}
