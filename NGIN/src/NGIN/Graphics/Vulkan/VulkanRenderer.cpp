#include <NGIN/Graphics/Vulkan/VulkanRenderer.hpp>
#include <NGIN/Graphics/Vulkan/VulkanWindow.hpp>

namespace NGIN::Graphics
{
	VulkanRenderer::VulkanRenderer(VulkanWindow* window)
		: vulkanWindow(window)
	{
		// Initialize other members as necessary
	}

	VulkanRenderer::~VulkanRenderer()
	{
		// Cleanup Vulkan resources
		vkDevice.destroy();
		vkInstance.destroy();
	}

	bool VulkanRenderer::Initialize()
	{
		if (!SetupInstance())
			return false;
		if (!SetupSurface())
			return false;
		if (!SetupDevice())
			return false;

		// Continue with other setup functions, e.g., SetupSurface() and SetupDevice()

		return true;
	}

	void VulkanRenderer::BeginFrame()
	{
		// Begin frame by starting a command buffer

	}
	void VulkanRenderer::EndFrame()
	{
		// Submit command buffers to the GPU
	}

	void VulkanRenderer::SetViewport(int width, int height)
	{
		// Set viewport
	}

	void VulkanRenderer::Clear(float r, float g, float b, float a)
	{
		// Clear the screen
	}


	bool VulkanRenderer::SetupInstance()
	{
		// Define the application info
		vk::ApplicationInfo appInfo;
		appInfo.pApplicationName = "NGIN";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.pEngineName = "NGIN";
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Set up the instance create info
		vk::InstanceCreateInfo createInfo;
		createInfo.pApplicationInfo = &appInfo;

		// Get the required extensions from SDL and add them to the createInfo
		unsigned extensionCount;
		SDL_Vulkan_GetInstanceExtensions(vulkanWindow->GetSDLWindow(), &extensionCount, nullptr);
		std::vector<const char*> extensions(extensionCount);
		SDL_Vulkan_GetInstanceExtensions(vulkanWindow->GetSDLWindow(), &extensionCount, extensions.data());
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Finally, create the Vulkan instance
		vk::Result result = vk::createInstance(&createInfo, nullptr, &vkInstance);
		if (result != vk::Result::eSuccess)
		{
			// Handle error
			return false;
		}

		return true;
	}

	bool VulkanRenderer::SetupDevice()
	{
		// 1. Physical device selection
		std::vector<vk::PhysicalDevice> devices = vkInstance.enumeratePhysicalDevices();

		// Here, we simply pick the first physical device. In a complete implementation, you might want
		// to select based on certain properties or capabilities.
		if (devices.empty())
		{
			// Handle error, maybe log the issue
			return false;
		}
		vkPhysicalDevice = devices[0];

		// 2. Create the logical device
		// For simplicity, we assume the GPU has a graphics queue family at index 0.
		// A full implementation would search for the proper queue family.
		float queuePriority = 1.0f;
		vk::DeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = 0;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		vk::DeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

		// Create the logical device
		vk::Result result = vkPhysicalDevice.createDevice(&deviceCreateInfo, nullptr, &vkDevice);
		if (result != vk::Result::eSuccess)
		{
			// Handle error
			return false;
		}

		return true;
	}

	bool VulkanRenderer::SetupSurface()
	{
		VkSurfaceKHR tempSurface;
		if (!SDL_Vulkan_CreateSurface(vulkanWindow->GetSDLWindow(), static_cast<VkInstance>(vkInstance), &tempSurface))
		{
			// Handle error, maybe log the issue
			return false;
		}
		vkSurface = vk::SurfaceKHR(tempSurface);
		return true;
	}
}

