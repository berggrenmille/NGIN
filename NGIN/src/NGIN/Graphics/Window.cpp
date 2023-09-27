#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{
	bool Window::Init(GraphicsAPI backend, WindowSettings& settings)
	{
		if (isInitialized)
			return true;
		UInt32 flags = 0; // Add base flags here
		switch (backend)
		{
		case GraphicsAPI::VULKAN:
			flags |= SDL_WINDOW_VULKAN;
			break;
		case GraphicsAPI::OPEN_GL:
			flags |= SDL_WINDOW_OPENGL;
			break;
		}
		window = SDL_CreateWindow(settings.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.width, settings.height, flags);

		if (window == nullptr)
		{
			NGIN_ERROR("Failed to create window: {}", SDL_GetError());
			return false;
		}

		isInitialized = true;
		return true;
	}
	void Window::Shutdown()
	{
		if (!isInitialized)
		{
			NGIN_WARNING("Cannot shutdown window that is not initialized. Stopping shutdown.");
			return;
		}

		if (window)
			SDL_DestroyWindow(window);
		window = nullptr;
		isInitialized = false;
	}

	void Window::PollEvents()
	{

	}

	bool Window::IsOpen() const
	{
		return isInitialized;
	}

	int Window::GetWidth() const
	{
		if (window) [[likely]]
			{
				int width;
				SDL_GetWindowSize(window, &width, nullptr);
				return width;
			}
			return 0;
	}

	int Window::GetHeight() const
	{
		if (window) [[likely]]
			{
				int height;
				SDL_GetWindowSize(window, nullptr, &height);
				return height;
			}
			return 0;
	}

	SDL_Window* Window::GetSDLWindow() const
	{
		return window;
	}
} // namespace NGIN
