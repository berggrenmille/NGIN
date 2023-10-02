#include <gtest/gtest.h>
#include <NGIN/Memory/StackAllocator.hpp>
#include <cstdint>
#include <NGIN/Memory/Allocator.hpp>
#include <utility>

using namespace NGIN::Memory;

class StackAllocatorTest : public ::testing::Test
{
protected:
    StackAllocator allocator;

    StackAllocatorTest() : allocator(1024) {} // Init allocator with 1024 bytes

    // Optional: SetUp() and TearDown() methods to configure and clean up any additional resources.
    // void SetUp() override {}
    // void TearDown() override {}
};

TEST_F(StackAllocatorTest, Constructor)
{
    void* ptr = allocator.Allocate(500);
    ASSERT_NE(ptr, nullptr);
}

TEST_F(StackAllocatorTest, SimpleAllocation)
{
    void* ptr = allocator.Allocate(500);
    ASSERT_NE(ptr, nullptr);

    void* ptr2 = allocator.Allocate(500);
    ASSERT_NE(ptr2, nullptr);

    // Calculate the distance between two memory addresses
    uintptr_t distance = reinterpret_cast<uintptr_t>(ptr2) - reinterpret_cast<uintptr_t>(ptr);

    // Check if memory addresses are spaced properly
    ASSERT_GE(distance, 500);
    // Check if memory addresses are spaced properly, accounting for alignment
    ASSERT_LE(distance, 500 + alignof(std::max_align_t) - 1);
}

TEST_F(StackAllocatorTest, OverAllocation)
{
    void* ptr = allocator.Allocate(500);
    ASSERT_NE(ptr, nullptr);

    void* ptr2 = allocator.Allocate(600); // This should fail because we don't have 600 bytes left
    ASSERT_EQ(ptr2, nullptr);
}

TEST_F(StackAllocatorTest, Deallocation)
{
    void* ptr1 = allocator.Allocate(300);
    ASSERT_NE(ptr1, nullptr);

    void* ptr2 = allocator.Allocate(400);
    ASSERT_NE(ptr2, nullptr);

    allocator.Deallocate(ptr2);
    void* ptr3 = allocator.Allocate(400); // Should be able to allocate 400 bytes after deallocating ptr2
    ASSERT_NE(ptr3, nullptr);
}

TEST_F(StackAllocatorTest, Alignment)
{
    void* ptr1 = allocator.Allocate(1, 256); // Allocate 1 byte, but request 256 byte alignment
    uintptr_t address1 = reinterpret_cast<uintptr_t>(ptr1);
    ASSERT_EQ(address1 % 256, 0); // Check if the allocated address is aligned to 256 bytes

    void* ptr2 = allocator.Allocate(1, 256); // Allocate 1 byte, but request 256 byte alignment
    uintptr_t address2 = reinterpret_cast<uintptr_t>(ptr2);
    ASSERT_EQ(address2 % 256, 0); // Check if the allocated address is aligned to 256 bytes
}

TEST_F(StackAllocatorTest, DeallocateAll)
{
    void* ptr1 = allocator.Allocate(300);
    ASSERT_NE(ptr1, nullptr);

    void* ptr2 = allocator.Allocate(400);
    ASSERT_NE(ptr2, nullptr);

    allocator.DeallocateAll();

    void* ptr3 = allocator.Allocate(800); // Should be able to allocate 800 bytes after deallocating all
    ASSERT_NE(ptr3, nullptr);
}

TEST_F(StackAllocatorTest, Owns)
{
    void* block1 = allocator.Allocate(200);
    EXPECT_TRUE(allocator.Owns(block1));

    void* outsideBlock = malloc(200);
    EXPECT_FALSE(allocator.Owns(outsideBlock));
    free(outsideBlock);

    SUCCEED(); // If we get here, the test passed
}