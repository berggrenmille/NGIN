#pragma once

#include <NGIN/Defines.hpp>

#include <NGIN/Time.hpp>
#include <NGIN/Logging/FileSink.hpp>
#include <NGIN/Logging.hpp>
#include <NGIN/Core/Engine.hpp>
#include <NGIN/Core/TestModule.hpp>
#include <NGIN/Meta/TypeID.hpp>
#include <NGIN/Meta/TypeName.hpp>
#include <NGIN/Graphics/Context.hpp>
#include <NGIN/Core/Modules/WindowModule.hpp>
#include <SDL2/SDL.h>

#include <thread>

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
    int Init(int argc, char *argv[])
    {

        // Initialize Logging
        std::cout
                << "\033]0;"
                << "NGIN"
                << "\007"
                << std::endl;
        Logging::Init();

        Config::Init();

        NGIN_WARNING("Initializing MTETSTTST {}", Config::GetRawValue("TEST"));

        // Initialize SDL and log the initialization process
        NGIN_WARNING("Initializing SDL... ");
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            // NGIN_ERROR("SDL failed to initialize: {}", SDL_GetError());

            return 1;
        }
        NGIN_INFO("SDL initialized");


        NGIN::Core::Engine engine = NGIN::Core::Engine();
        engine.AddModule<NGIN::Core::WindowModule>();


        engine.Tick();


        // Initialize App
        NGIN_WARNING("Initializing App...");
        std::cout << "Initializing App..." << std::endl;
        NGIN::App *app = new T();
        app->Init();
        // Free memory after app initialization is done
        delete app;
        return 0;
    }
}
