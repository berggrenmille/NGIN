#pragma once
#include <NGIN/Core.h>
#include <SDL2/SDL.h>
#include <string>
namespace NGIN::Graphics
{
	class Window
	{
	public:
		NGIN_API Window() = default;
		NGIN_API virtual ~Window() = default;

		NGIN_API virtual bool Init(const std::string& title, int width, int height) = 0;
		NGIN_API virtual void Shutdown() = 0;
		NGIN_API void PollEvents();

		NGIN_API bool IsOpen() const;

		NGIN_API int GetWidth() const;
		NGIN_API int GetHeight() const;

		SDL_Window* GetSDLWindow() const { return window; };
	protected:
		SDL_Window* window = nullptr;
		bool isInitialized = false;
	};
}