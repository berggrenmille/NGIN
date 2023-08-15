#include <PCH.h>
#include <NGIN/Memory/FreeListAllocator.hpp>
#include <cassert>
#include <NGIN/Logging.hpp>
namespace NGIN
{
	FreeListAllocator::FreeListAllocator(size_t size)
		: Allocator(), freeBlocks(nullptr), start(nullptr)
	{
		NGIN_ASSERT(size > sizeof(FreeBlock), "Size should be greater than the size of a FreeBlock: {} bytes", sizeof(FreeBlock));

		this->size = size;
		this->usedMemory = 0;
		start = ::operator new(size);
		freeBlocks = static_cast<FreeBlock*>(start);
		// Might be able to handle this error more gracefully, like throwing an exception or falling back to another allocator
		// Very unlikely, soooo... TODO
		NGIN_ASSERT(freeBlocks != nullptr, "FreeListAllocator failed to allocate memory at contruction");


		freeBlocks->size = size;
		freeBlocks->previous = nullptr;
		freeBlocks->next = nullptr;
	}

	FreeListAllocator::~FreeListAllocator()
	{
		::operator delete(start);
	}

	void* FreeListAllocator::Allocate(size_t size, size_t alignment, const std::source_location& location)
	{
		NGIN_ASSERT(size > 0, "Cannot allocate zero bytes.");

		// Find a suitable free block for the allocation.
		auto [foundBlock, headerAdjustment] = FindFreeBlock(size, alignment);
		// If no suitable block is found, log the error and return nullptr.
		if (foundBlock == nullptr) [[unlikely]]
		{
			NGIN_LOG_SRC(location, Logging::Level::Error, "Allocator reached it's capacity\n NOTE: Some engine provided allocators supports configurable size in Config.json");
			return nullptr;
		}

			// Remove the found block from the free list.
		RemoveBlockFromFreeList(foundBlock);
		// Store a copy of the old block's metadata.
		FreeBlock oldBlock = *foundBlock;

		// Calculate the locations for the AllocationHeader and the payload.
		Address headerPtr = Address(foundBlock) + headerAdjustment;
		Address payloadPtr = headerPtr + sizeof(AllocationHeader);

		// Determine the available space for the payload.
		size_t payloadSpace = oldBlock.size - sizeof(AllocationHeader) - headerAdjustment;
		auto header = new (reinterpret_cast<void*>(headerPtr)) AllocationHeader(payloadSpace, headerAdjustment);

		Address payloadEnd = payloadPtr + size;
		Address blockEnd = payloadPtr + payloadSpace;

		// Check if a new free block can be created from the remaining space.
		Address newBlockAdjustment = GetAlignmentAdjustment(alignof(FreeBlock), reinterpret_cast<void*>(payloadEnd));
		size_t padding = blockEnd - (payloadEnd + newBlockAdjustment);

		if (padding >= minBlockSize)
		{
			header->size -= padding;

			void* newBlockPtr = reinterpret_cast<void*>(payloadEnd + newBlockAdjustment);
			FreeBlock* newBlock = new (newBlockPtr) FreeBlock(padding);

			AddBlockToFreeList(newBlock);
		}

		usedMemory += header->size;
		return reinterpret_cast<void*>(payloadPtr);
	}


	void FreeListAllocator::Deallocate(void* ptr)
	{
		NGIN_ASSERT(ptr != nullptr, "Cannot deallocate a nullptr.");

		AllocationHeader* header = (AllocationHeader*)((uintptr_t)ptr - sizeof(AllocationHeader));
		size_t blockSize = header->adjustment + header->size + sizeof(AllocationHeader);

		void* block = (void*)((uintptr_t)ptr - header->adjustment);

		usedMemory -= header->size;

		FreeBlock* newBlock = new (block) FreeBlock(blockSize);

		AddBlockToFreeList(newBlock);
	}

	void FreeListAllocator::DeallocateAll()
	{
		freeBlocks = reinterpret_cast<FreeBlock*>(new char[size]);
		freeBlocks->size = size;
		freeBlocks->next = nullptr;

		usedMemory = 0;
	}


	void FreeListAllocator::AddBlockToFreeList(FreeBlock* block)
	{

		if (freeBlocks == nullptr)
		{
			freeBlocks = block;
			return;
		}

		if ((Address)block < (Address)freeBlocks)
		{
			block->next = freeBlocks;
			freeBlocks->previous = block;
			freeBlocks = block;
			CoalesceBlock(block);
			return;
		}

		auto currentBlock = freeBlocks;

		while (currentBlock->next != nullptr)
		{
			if ((Address)block > (Address)currentBlock &&
				(Address)block < (Address)(currentBlock->next))
			{

				block->next = currentBlock->next;
				currentBlock->next->previous = block;
				block->previous = currentBlock;
				currentBlock->next = block;
				CoalesceBlock(block);
				return;
			}
			currentBlock = currentBlock->next;
		}

		currentBlock->next = block;
		block->previous = currentBlock;
		CoalesceBlock(block);


	}

	void FreeListAllocator::RemoveBlockFromFreeList(FreeBlock* block)
	{
		NGIN_ASSERT(block != nullptr, "Cannot remove null block");

		if (block == freeBlocks)
		{
			freeBlocks = block->next;
			return;
		}

		if (block->previous != nullptr)
			block->next->previous = block->previous;


		if (block->next != nullptr)
			block->next->previous = block->previous;
	}

	void FreeListAllocator::CoalesceBlock(FreeBlock* block)
	{
		if (block->previous != nullptr)
		{
			Address leftEnd = Address(block->previous) + block->previous->size;
			if (Address(block) == leftEnd)
			{
				block->previous->next = block->next;
				if (block->next != nullptr)
					block->next->previous = block->previous;
				block->previous->size += block->size;
				block = block->previous;
			}
		}

		if (block->next != nullptr)
		{
			Address blockEnd = Address(block) + block->size;

			// If the end of this block is the start of the next block, coalesce
			if (blockEnd == Address(block->next))
			{
				block->size += block->next->size;
				block->next = block->next->next;
				if (block->next != nullptr)
				{
					block->next->previous = block;
				}
			}
		}
	}

	std::pair<FreeListAllocator::FreeBlock*, size_t> FreeListAllocator::FindFreeBlock(size_t size, size_t alignment)
	{
		const auto totSize = size + sizeof(AllocationHeader);

		const size_t reqAlignment = std::max(alignof(AllocationHeader), alignment);

		FreeBlock* currentBlock = freeBlocks;

		size_t headerAdjustment = 0;

		while (currentBlock != nullptr)
		{
			Address first = reinterpret_cast<Address>(currentBlock) + sizeof(AllocationHeader);

			size_t reqAdjustment =
				GetAlignmentAdjustment(reqAlignment, reinterpret_cast<void*>(first));
			size_t totAllocSize = totSize + reqAdjustment;

			if (totAllocSize <= currentBlock->size)
			{
				return { currentBlock, reqAdjustment };
			}

			currentBlock = currentBlock->next;
		}
		return { nullptr, headerAdjustment };

	}

} // namespace NGIN
