#include <PCH.h>
#include <Memory/FreeListAllocator.h>
#include <cassert>
#include <core/Logger.h>
namespace NGIN
{
	FreeListAllocator::FreeListAllocator(size_t size)
	{
		this->size = size;
		this->usedMemory = 0;
		this->freeBlocks = (FreeBlock*)::operator new(size);
		// Might be able to handle this error more gracefully, like throwing an exception or falling back to another allocator
		// Very unlikely, soooo... TODO
		NGIN_ASSERT(freeBlocks != nullptr, "FreeListAllocator failed to allocate memory at contruction");

		freeBlocks->size = size;
		freeBlocks->next = nullptr;
	}

	FreeListAllocator::~FreeListAllocator()
	{
		::operator delete(freeBlocks);
	}

	void* FreeListAllocator::Allocate(size_t size, size_t alignment, const std::source_location& location)
	{

		Address currentAddress;
		FreeBlock* previousBlock = nullptr;
		FreeBlock* currentBlock = freeBlocks;

		while (currentBlock != nullptr)
		{
			// Calculate adjustment needed to keep object correctly aligned
			currentAddress = Address(currentBlock);
			size_t adjustment = GetAlignmentAdjustment(alignment, currentBlock);

			size_t totalSize = size + adjustment;

			// If allocation doesn't fit in this FreeBlock, try the next
			if (currentBlock->size < totalSize)
			{
				previousBlock = currentBlock;
				currentBlock = currentBlock->next;
				continue;
			}

			// If allocations in the remaining memory will be impossible
			if (currentBlock->size - totalSize <= sizeof(AllocationHeader))
			{
				// Increase allocation size instead of creating a new FreeBlock
				totalSize = currentBlock->size;

				if (previousBlock != nullptr)
				{
					previousBlock->next = currentBlock->next;
				}
				else
				{
					freeBlocks = currentBlock->next;
				}
			}
			else
			{
				// Else create a new FreeBlock containing remaining memory
				FreeBlock* newBlock = (FreeBlock*)(currentAddress + totalSize);
				newBlock->size = currentBlock->size - totalSize;
				newBlock->next = currentBlock->next;

				if (previousBlock != nullptr)
				{
					previousBlock->next = newBlock;
				}
				else
				{
					freeBlocks = newBlock;
				}
			}

			// Setup the header
			AllocationHeader* header = (AllocationHeader*)(currentAddress + adjustment - sizeof(AllocationHeader));
			header->size = totalSize;
			header->adjustment = adjustment;

			usedMemory += totalSize;
			return (void*)(currentAddress + adjustment);
		}

		return nullptr;  // No free blocks found
	}

	void FreeListAllocator::Deallocate(void* ptr)
	{
		// Retrieve the AllocationHeader from the ptr
		AllocationHeader* header = (AllocationHeader*)((Address)ptr - sizeof(AllocationHeader));


		// Add the block to the list
		FreeBlock* block = (FreeBlock*)(Address(ptr) - header->adjustment);
		block->size = header->size;

		// Initialize pointers to blocks
		FreeBlock* currentBlock = freeBlocks;
		FreeBlock* previousBlock = nullptr;

		// Find where to insert the deallocated block
		while (currentBlock != nullptr && currentBlock < block)
		{
			previousBlock = currentBlock;
			currentBlock = currentBlock->next;
		}

		// Merge with next block if possible
		if (block + block->size == currentBlock)
		{
			block->size += currentBlock->size;
			block->next = currentBlock->next;
		}
		else
		{
			block->next = currentBlock;
		}

		// Merge with previous block if possible
		if (previousBlock != nullptr && previousBlock + previousBlock->size == block)
		{
			previousBlock->size += block->size;
			previousBlock->next = block->next;
		}
		else if (previousBlock != nullptr)
		{
			previousBlock->next = block;
		}
		else
		{
			freeBlocks = block;
		}

		usedMemory -= header->size;
	}

	void FreeListAllocator::DeallocateAll()
	{
		freeBlocks->size = size;
		freeBlocks->next = nullptr;
		usedMemory = 0;
	}

} // namespace NGIN
