#include <NGIN/Graphics/Vulkan/VulkanWindow.hpp>


namespace NGIN::Graphics
{
	VulkanWindow::~VulkanWindow()
	{
		Shutdown();
	}

	bool VulkanWindow::Init(const std::string& title, int width, int height)
	{
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);
		if (window == nullptr)
		{
			// TODO: Log error
			return false;
		}
	}

	void VulkanWindow::Shutdown()
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}



}