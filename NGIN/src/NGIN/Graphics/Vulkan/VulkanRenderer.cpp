#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Vulkan/VulkanRenderer.hpp>
#include <NGIN/Graphics/Vulkan/VulkanWindow.hpp>

#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{
	VulkanRenderer::VulkanRenderer(VulkanWindow *window)
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

		// Enable validation layers if in debug mode
#if defined(NGIN_DEBUG)
		const std::vector<const char *> validationLayers = {
			"VK_LAYER_KHRONOS_validation"};
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#endif

		// Get the required extensions from SDL and add them to the createInfo
		unsigned extensionCount;
		SDL_Vulkan_GetInstanceExtensions(vulkanWindow->GetSDLWindow(), &extensionCount, nullptr);
		std::vector<const char *> extensions(extensionCount);
		SDL_Vulkan_GetInstanceExtensions(vulkanWindow->GetSDLWindow(), &extensionCount, extensions.data());
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Finally, create the Vulkan instance
		vk::Result result = vk::createInstance(&createInfo, nullptr, &vkInstance);
		if (result != vk::Result::eSuccess)
		{
			NGIN_ERROR("Failed to create Vulkan instance.");
			return false;
		}

		return true;
	}

	bool VulkanRenderer::SetupDevice()
	{
		// 1. Physical device selection, return false if no suitable device is found
		std::vector<vk::PhysicalDevice> devices = vkInstance.enumeratePhysicalDevices();
		if (devices.empty())
		{
			NGIN_ERROR("No Vulkan devices found.");
			return false;
		}

		// Rate the devices based on their suitability
		std::multimap<int, vk::PhysicalDevice> candidates;
		for (const auto &device : devices)
		{
			int score = RateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0)
			vkPhysicalDevice = candidates.rbegin()->second;
		else
		{
			NGIN_ERROR("No suitable Vulkan device found.");
			return false;
		}

		// 2. Create the logical device
		// Find the queue family that supports graphics
		auto queueFamilies = vkPhysicalDevice.getQueueFamilyProperties();
		int i = 0;
		int graphicsQueueFamilyIndex = -1;
		for (const auto &queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				graphicsQueueFamilyIndex = i;
				break;
			}
			i++;
		}

		if (graphicsQueueFamilyIndex < 0)
		{
			NGIN_ERROR("No suitable queue family found.");
			return false;
		}

		// Create the queue create info
		float queuePriority = 1.0f;
		vk::DeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		vk::DeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

		// Create the logical device
		vk::Result result = vkPhysicalDevice.createDevice(&deviceCreateInfo, nullptr, &vkDevice);

		if (result != vk::Result::eSuccess)
		{
			NGIN_ERROR("Failed to create Vulkan logical device.");
			return false;
		}

		return true;
	}

	int VulkanRenderer::RateDeviceSuitability(const vk::PhysicalDevice &device) const
	{
		vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
		vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

		int score = 0;

		// Discrete GPUs have a significant performance advantage
		if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			score += 1000;

		// Maximum possible size of textures affects graphics quality.
		score += deviceProperties.limits.maxImageDimension2D;

		// Application can't function without graphics queue family.
		auto queueFamilies = device.getQueueFamilyProperties();
		bool hasGraphicsQueue = false;
		for (const auto &family : queueFamilies)
		{
			if (family.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				hasGraphicsQueue = true;
				break;
			}
		}

		if (!hasGraphicsQueue)
		{
			return 0;
		}

		// Check for specific features
		if (!deviceFeatures.geometryShader)
		{
			return 0;
		}

		// Add more feature checks and rate them here

		return score;
	}

	bool VulkanRenderer::SetupSurface()
	{
		VkSurfaceKHR tempSurface;

		// Attempt to create a Vulkan surface using SDL
		if (!SDL_Vulkan_CreateSurface(vulkanWindow->GetSDLWindow(), static_cast<VkInstance>(vkInstance), &tempSurface))
		{
			NGIN_ERROR("Failed to create Vulkan surface. SDL: {}", SDL_GetError());
			return false;
		}

		// Convert the VkSurfaceKHR handle to a vk::SurfaceKHR handle
		vkSurface = vk::SurfaceKHR(tempSurface);
		return true;
	}
}
