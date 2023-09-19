#pragma once
#include <NGIN/Defines.hpp>

#include <NGIN/Time/Time.hpp>
#include <NGIN/Logging/FileSink.hpp>
#include <NGIN/Logging.hpp>

#include <NGIN/Util/Delegate.hpp>
#include <chrono>

#include <SDL2/SDL.h>

void freeFunction()
{
	volatile int a = 1;
}

int simpleFunction(int a, int b)
{
	return a + b;
}

struct Test
{
	int simpleFunction(int a, int b)
	{
		return a + b;
	}
};

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
	template <NGIN::is_app T>
	int Init(int argc, char *argv[])
	{

		// Initialize Logging
		std::cout
			<< "\033]0;"
			<< "NGIN"
			<< "\007";
		Logging::Init();
		// Init Config
		Config::Init();

		const int NUM_ITERATIONS = 100000000; // or whatever large number you want

		// Set up the std::function and Delegate objects
		std::function<int(int, int)> stdFunc = simpleFunction;
		NGIN::Util::Delegate del(simpleFunction);
		Test test;
		std::function<int(int, int)> stdFunc2 = std::bind(&Test::simpleFunction, &test, std::placeholders::_1, std::placeholders::_2);
		NGIN::Util::Delegate del2(&Test::simpleFunction, &test);

		std::function<void()> stdFunc3 = freeFunction;
		NGIN::Util::Delegate del3(freeFunction);

		// Benchmark std::function
		auto start1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_ITERATIONS; ++i)
		{
			stdFunc(1, 2);
		}
		auto end1 = std::chrono::high_resolution_clock::now();
		auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();

		// Benchmark Delegate
		auto start2 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_ITERATIONS; ++i)
		{
			del(1, 2);
		}
		auto end2 = std::chrono::high_resolution_clock::now();
		auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();

		// Benchmark Delegate to member function

		auto start3 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_ITERATIONS; ++i)
		{
			del2(1, 2);
		}
		auto end3 = std::chrono::high_resolution_clock::now();
		auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3).count();

		// Benchmark std::function to member function
		auto start4 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_ITERATIONS; ++i)
		{
			stdFunc2(1, 2);
		}
		auto end4 = std::chrono::high_resolution_clock::now();
		auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(end4 - start4).count();

		// Benchmark std::function to free function
		auto start5 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_ITERATIONS; ++i)
		{
			stdFunc3();
		}
		auto end5 = std::chrono::high_resolution_clock::now();
		auto duration5 = std::chrono::duration_cast<std::chrono::milliseconds>(end5 - start5).count();

		// Benchmark Delegate to free function

		auto start6 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_ITERATIONS; ++i)
		{
			del3();
		}
		auto end6 = std::chrono::high_resolution_clock::now();
		auto duration6 = std::chrono::duration_cast<std::chrono::milliseconds>(end6 - start6).count();

		// Print results
		std::cout << "std::function took: " << duration1 << "ms" << std::endl;
		std::cout << "std::function to member function took: " << duration4 << "ms" << std::endl;
		std::cout << "std::function to free function took: " << duration5 << "ms" << std::endl;
		std::cout << "Delegate took: " << duration2 << "ms" << std::endl;
		std::cout << "Delegate to member function took: " << duration3 << "ms" << std::endl;
		std::cout << "Delegate to free function took: " << duration6 << "ms" << std::endl;

		NGIN_WARNING("Initializing MTETSTTST {}", Config::GetRawValue("TEST"));

		// Initialize SDL and log the initialization process
		NGIN_WARNING("Initializing SDL... ");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		{
			// NGIN_ERROR("SDL failed to initialize: {}", SDL_GetError());

			return 1;
		}
		NGIN_INFO("SDL initialized");

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
