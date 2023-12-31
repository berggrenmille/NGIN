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
#include <NGIN/Core/Modules/GraphicsModule.hpp>


#include <thread>

// Declare client application's main function
extern int NGINMain(int argc, char* argv[]);


int main(int argc, char* argv[])
{

    // Initialize Global subsystems
    NGIN::Logging::Init();
    NGIN::Config::Init();
//  NGIN::VFS::Init();

    // Call client application's main function
    return NGINMain(argc, argv);
}

// Undefined main to avoid conflicts with SDL
#undef main
#define main NGINMain

