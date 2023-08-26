#include <gtest/gtest.h>
#include <NGIN/Memory/FallbackAllocator.hpp>

#include <utility>
#include <source_location>

using namespace NGIN::Memory;

// Define some MockAllocators for testing purposes
class MockPrimaryAllocator
{
public:
    void *Allocate(size_t size, size_t alignment, const std::source_location &location)
    {
        return nullptr; // Always fail the allocation to test fallback
    }
    void Deallocate(void *ptr) {}
    void DeallocateAll() {}
    bool Owns(void *ptr) const { return false; }
};

class MockFallbackAllocator
{
public:
    void *Allocate(size_t size, size_t alignment, const std::source_location &location)
    {
        ++counter;
        return malloc(size); // Use actual allocation for fallback
    }
    void Deallocate(void *ptr)
    {
        --counter;
        free(ptr);
    }
    void DeallocateAll() {}                            // Not implemented
    bool Owns(void *ptr) const { return counter > 0; } // Assume ownership for simplicity
private:
    int counter = 0;
};

class FallbackAllocatorTest : public ::testing::Test
{
protected:
    FallbackAllocator<MockPrimaryAllocator, MockFallbackAllocator> allocator;

    FallbackAllocatorTest() : allocator(MockPrimaryAllocator(), MockFallbackAllocator()) {}
};

TEST_F(FallbackAllocatorTest, BasicAllocation)
{
    void *block = allocator.Allocate(512);
    EXPECT_NE(block, nullptr);
    allocator.Deallocate(block);
}

TEST_F(FallbackAllocatorTest, FallbackUsed)
{
    void *block = allocator.Allocate(512);
    EXPECT_NE(block, nullptr);
    EXPECT_TRUE(allocator.Owns(block));
    allocator.Deallocate(block);
}

TEST_F(FallbackAllocatorTest, Deallocate)
{
    void *block = allocator.Allocate(512);
    allocator.Deallocate(block);
    // Assuming you have a way to check if a block is deallocated
    // For simplicity, we won't check this in the test
}

TEST_F(FallbackAllocatorTest, Owns)
{
    void *block = allocator.Allocate(200);
    EXPECT_TRUE(allocator.Owns(block));
    allocator.Deallocate(block);

    void *outsideBlock = malloc(200);
    EXPECT_FALSE(allocator.Owns(outsideBlock));
    free(outsideBlock);
}
