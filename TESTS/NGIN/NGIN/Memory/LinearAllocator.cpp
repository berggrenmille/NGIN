#include <gtest/gtest.h>
#include <NGIN/Memory/LinearAllocator.hpp>
#include <NGIN/Memory/Allocator.hpp>
#include <NGIN/Meta/StoragePolicy.hpp>
#include <utility>
using namespace NGIN::Memory;

class LinearAllocatorTest : public ::testing::Test
{
protected:
	LinearAllocator allocator;

	LinearAllocatorTest() : allocator(1024) {} // Initializes a block of 1024 bytes for each test
};

TEST_F(LinearAllocatorTest, BasicAllocation)
{
	void *block = allocator.Allocate(512);
	EXPECT_NE(block, nullptr);
}

TEST_F(LinearAllocatorTest, OverAllocation)
{
	void *block1 = allocator.Allocate(512);
	EXPECT_NE(block1, nullptr);

	void *block2 = allocator.Allocate(512);
	EXPECT_NE(block2, nullptr);

	void *block3 = allocator.Allocate(1);
	EXPECT_EQ(block3, nullptr); // Should be out of memory after previous allocations
}

TEST_F(LinearAllocatorTest, Alignment)
{
	void *block = allocator.Allocate(1, 256); // 256 byte alignment
	uintptr_t address = reinterpret_cast<uintptr_t>(block);
	EXPECT_EQ(address % 256, 0); // Check if the address is aligned
}

TEST_F(LinearAllocatorTest, DeallocateAll)
{
	allocator.Allocate(512);
	allocator.DeallocateAll();

	void *block = allocator.Allocate(1024);
	EXPECT_NE(block, nullptr); // We should be able to allocate the entire block after a deallocate all
}

TEST_F(LinearAllocatorTest, TypeErase)
{
	NGIN::Memory::Allocator<NGIN::Meta::StoragePolicy::Dynamic> typeErasedAllocator(std::move(allocator));
	SUCCEED(); // If we get here, the test passed
}

TEST_F(LinearAllocatorTest, Owns)
{
	void *block1 = allocator.Allocate(200);
	EXPECT_TRUE(allocator.Owns(block1));

	void *outsideBlock = malloc(200);
	EXPECT_FALSE(allocator.Owns(outsideBlock));
	free(outsideBlock);

	SUCCEED(); // If we get here, the test passed
}