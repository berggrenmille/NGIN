#pragma once

// Includes required for the NGIN game engine
#include <Core.h>

// Additional NGIN includes
#include <App.h>
#include <Window.h>
#include <Layer.h>
#include <EventBus.h>
#include <Logger.h>
#include <Config/Config.h>

// Includes for the SDL2 library
#include <SDL2/SDL.h>

// Standard includes
#include <iostream>

	/**
	 * @brief The main namespace
	 */
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
		NGIN_INFO("{}", Config::Get<int>("test"));





		// Initialize SDL and log the initialization process
		NGIN_WARNING("Initializing SDL...");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		{
			NGIN_ERROR("SDL failed to initialize: {}", SDL_GetError());
			return 1;
		}
		NGIN_INFO("SDL initialized");

		// Initialize App
		NGIN_WARNING("Initializing App...");
		NGIN::App* app = new T();
		app->Init();
		// Free memory after app initialization is done
		delete app;
		return 0;
	}
}
