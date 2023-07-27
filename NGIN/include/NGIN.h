#pragma once

// Includes required for the NGIN game engine
#include <Core.h>

// Disables warnings from various libraries
#include <WarningDisabler.h>

// Additional NGIN includes
#include <App.h>
#include <Window.h>
#include <Layer.h>
#include <EventBus.h>
#include <Logger.h>

// Includes for the SDL2 library, used for managing low-level tasks
#include <SDL2/SDL.h>

// Standard includes
#include <iostream>

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
		// Initialize logger
		Logger::Log(Verbosity::WARNING, CURR_FILE(), "Initializing Logger...");
		Logger::Init(argc, argv);
		Logger::Log(Verbosity::INFO, CURR_FILE(), "Logger Initialized");

		// Initialize SDL and log the initialization process
		Logger::Log(Verbosity::WARNING, CURR_FILE(), "Initializing SDL...");
		if (SDL_Init(SDL_INIT_EVERYTHING))
		{
			std::cout << SDL_GetError() << std::endl;
			Logger::Log(Verbosity::ERROR, CURR_FILE(), "SDL failed to initialize: %s", SDL_GetError());
			return 1;
		}
		Logger::Log(Verbosity::INFO, CURR_FILE(), "SDL initialized");

		// Initialize App
		Logger::Log(Verbosity::WARNING, CURR_FILE(), "Initializing NGIN...");
		NGIN::App* app = new T();
		app->Init();
		// Free memory after app initialization is done
		delete app;
		return 0;
	}
}
