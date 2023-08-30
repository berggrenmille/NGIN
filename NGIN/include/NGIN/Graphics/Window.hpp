#pragma once
#include <NGIN/Core.h>
#include <NGIN/Graphics/GraphicsBackend.hpp>

#include <SDL2/SDL.h>
#include <string>

namespace NGIN::Graphics
{

	class Window
	{
	public:
		NGIN_API Window() = default;
		NGIN_API ~Window() = default;

		NGIN_API bool Init(GraphicsBackend backend, const std::string &title, int width, int height);

		NGIN_API void Shutdown();

		NGIN_API void PollEvents();

		NGIN_API bool IsOpen() const;

		NGIN_API int GetWidth() const;
		NGIN_API int GetHeight() const;

		NGIN_API SDL_Window *GetSDLWindow() const;

	private:
		SDL_Window *window = nullptr;
		bool isInitialized = false;
	};
}