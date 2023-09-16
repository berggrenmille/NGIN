#include <gtest/gtest.h>
#include <NGIN/Memory/Mallocator.hpp>
#include <NGIN/Memory/Allocator.hpp>
#include <NGIN/Meta/StoragePolicy.hpp>
#include <utility>
#include <cstddef>
#include <source_location>
using namespace NGIN::Memory;

class MallocatorTest : public ::testing::Test
{
protected:
    Mallocator allocator; // Mallocator does not require any initialization
};

TEST_F(MallocatorTest, BasicAllocation)
{
    void *block = allocator.Allocate(512);
    EXPECT_NE(block, nullptr);
}

TEST_F(MallocatorTest, AllocationAlignment)
{
    void *block = allocator.Allocate(1024, 256); // 256 byte alignment
    uintptr_t address = reinterpret_cast<uintptr_t>(block);
    EXPECT_EQ(address % 256, 0); // Check if the address is aligned
}

TEST_F(MallocatorTest, Deallocate)
{
    void *block = allocator.Allocate(512);
    EXPECT_NE(block, nullptr);
    allocator.Deallocate(block);

    // After deallocation, you cannot assert much about the block pointer.
    // The purpose of this test is mainly to ensure that Deallocate does not throw an exception.
    SUCCEED();
}

TEST_F(MallocatorTest, TypeErase)
{
    NGIN::Memory::Allocator<NGIN::Meta::StoragePolicy::DynamicStorage> typeErasedAllocator(std::move(allocator));
    SUCCEED(); // If we get here, the test passed
}
