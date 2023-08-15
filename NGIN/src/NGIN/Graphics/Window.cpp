#include <NGIN/Graphics/Window.hpp>

namespace NGIN::Graphics
{

	void Window::PollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isInitialized = false;
				break;
				// You can handle other events here like keyboard, mouse, etc.
			default:
				break;
			}
		}
	}

	bool Window::IsOpen() const
	{
		return isInitialized;
	}

	int Window::GetWidth() const
	{
		if (window)
		{
			int width;
			SDL_GetWindowSize(window, &width, nullptr);
			return width;
		}
		return 0;
	}

	int Window::GetHeight() const
	{
		if (window)
		{
			int height;
			SDL_GetWindowSize(window, nullptr, &height);
			return height;
		}
		return 0;
	}

} // namespace NGIN
