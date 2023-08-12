#pragma once
#include <Core/Core.h>
#include <Core/Memory/FreeListAllocator.hpp>
namespace NGIN
{

	/**
	 * @brief Template function to initialize an NGIN application.
	 *
	 * @tparam T A subclass of App to be initialized.
	 * @param argc The argument count from the command line.
	 * @param argv The argument values from the command line.
	 * @return int 0 if initialization was successful, 1 otherwise.
	 */
	template<NGIN::is_app T>
	int Init(int argc, char* argv[])
	{
		std::cout << "\033]0;" << "NGIN" << "\007";
		// Initialize logger
		Logger::Init(argc, argv);
		NGIN_INFO("Logger initialized");

		// Init Config
		Config::Init();


		FreeListAllocator alloc(1024);

		void* block1 = alloc.Allocate(512);
		alloc.Allocate(256);


		NGIN_INFO("Allocated block1: {0}", alloc.getUsedMemory());

		NGIN_ASSERT(false, "test");

		// Initialize SDL and log the initialization process
		NGIN_WARNING("Initializing SDL...");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		{
			NGIN_ERROR("SDL failed to initialize: {}", SDL_GetError());
			return 1;
		}
		NGIN_INFO("SDL initialized");

		//alloc1.Deallocate(ptrs[19]);
		//NGIN_INFO("{} : {}", 0, alloc1.getUsedMemory());
		//for (int i = 0; i < 100; i++)
		//{
		//	alloc1.Delete<int>((int*)ptrs[i]);
		//	NGIN_INFO("{} : {}", i, alloc1.getUsedMemory());
		//}

		// Initialize App
		NGIN_WARNING("Initializing App...");
		NGIN::App* app = new T();
		app->Init();
		// Free memory after app initialization is done
		delete app;
		return 0;
	}
}