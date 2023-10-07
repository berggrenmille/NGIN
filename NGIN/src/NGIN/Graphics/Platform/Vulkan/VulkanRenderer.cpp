#include "Precompiled/PCH.h"
#include "NGIN/Graphics/Platform/Vulkan/VulkanRenderer.hpp"
#include "NGIN/Graphics/Window.hpp"
#include "SDL2/SDL_vulkan.h"

#include "NGIN/Logging.hpp"


namespace NGIN::Graphics::Vulkan
{
    VulkanRenderer::VulkanRenderer(Ref<Window> window)
            : window(window)
    {
        // Init other members as necessary
    }

    VulkanRenderer::~VulkanRenderer()
    {
        // Cleanup Vulkan resources
        vkDevice.destroy();
        vkInstance.destroy();
    }

    bool VulkanRenderer::Init()
    {
        if (!SetupInstance())
            return false;
        if (!SetupSurface())
            return false;
        if (!SetupDevice())
            return false;
        if (!SetupSwapchain())
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

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
    {

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
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
        const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif


        // Get the required extensions from SDL and add them to the createInfo
        uint32_t extensionCount = 0;
        SDL_Vulkan_GetInstanceExtensions(window->GetSDLWindow(), &extensionCount, nullptr);
        std::vector<const char*> extensions(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window->GetSDLWindow(), &extensionCount, extensions.data());
#if defined(NGIN_DEBUG)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
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
        for (const auto& device: devices)
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
        for (const auto& queueFamily: queueFamilies)
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


    /// @brief Finds queue families supported by the device.
/// @param device The Vulkan physical device.
/// @param surface The Vulkan surface.
/// @return A structure holding queue family indices.
    struct QueueFamilyIndices
    {
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool IsComplete()
        {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        QueueFamilyIndices indices;

        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

        int i = 0;
        for (const auto& queueFamily: queueFamilies)
        {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                indices.graphicsFamily = i;
            }

            vk::Bool32 presentSupport = device.getSurfaceSupportKHR(i, surface);

            if (presentSupport)
            {
                indices.presentFamily = i;
            }

            if (indices.IsComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    bool VulkanRenderer::SetupSwapchain()
    {
        // 1. Query the surface capabilities
        vk::SurfaceCapabilitiesKHR surfaceCapabilities = vkPhysicalDevice.getSurfaceCapabilitiesKHR(vkSurface);
        std::vector<vk::SurfaceFormatKHR> availableFormats = vkPhysicalDevice.getSurfaceFormatsKHR(vkSurface);
        std::vector<vk::PresentModeKHR> availablePresentModes = vkPhysicalDevice.getSurfacePresentModesKHR(vkSurface);

        // 2. Choose swapchain settings
        vk::SurfaceFormatKHR chosenFormat;
        if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
        {
            chosenFormat.format = vk::Format::eB8G8R8A8Unorm;
            chosenFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        } else
        {
            chosenFormat = availableFormats[0];
        }

        vk::PresentModeKHR chosenPresentMode = vk::PresentModeKHR::eFifo;
        for (const auto& presentMode: availablePresentModes)
        {
            if (presentMode == vk::PresentModeKHR::eMailbox)
            {
                chosenPresentMode = presentMode;
                break;
            }
        }

        vk::Extent2D chosenExtent = surfaceCapabilities.currentExtent;
        if (chosenExtent.width == UINT32_MAX)
        {
            // Handle special case where extent is undefined
            // You may need to get window dimensions here to set the extent
            chosenExtent.width = 800;  // Example width
            chosenExtent.height = 600; // Example height
        }

        // 3. Create the swapchain
        vk::SwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.surface = vkSurface;
        swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
        swapchainCreateInfo.imageFormat = chosenFormat.format;
        swapchainCreateInfo.imageColorSpace = chosenFormat.colorSpace;
        swapchainCreateInfo.imageExtent = chosenExtent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

        QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice, vkSurface);

        uint32_t queueFamilyIndices[] = {static_cast<uint32_t>(indices.graphicsFamily),
                                         static_cast<uint32_t>(indices.presentFamily)};

        // If the graphics family and present family are the same, we could use the "Exclusive" mode
        if (indices.graphicsFamily == indices.presentFamily)
        {
            swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
            swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
            swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
        } else // If they are different, we should use the "Concurrent" mode
        {
            swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapchainCreateInfo.presentMode = chosenPresentMode;
        swapchainCreateInfo.clipped = VK_TRUE;

        vk::Result result = vkDevice.createSwapchainKHR(&swapchainCreateInfo, nullptr, &vkSwapchain);
        if (result != vk::Result::eSuccess)
        {
            NGIN_ERROR("Failed to create Vulkan swapchain.");
            return false;
        }

        // 4. Retrieve the swapchain images
        swapchainImages = vkDevice.getSwapchainImagesKHR(vkSwapchain);

        // 5. Create image views
        vk::ImageViewCreateInfo viewCreateInfo;
        viewCreateInfo.format = chosenFormat.format;
        viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;

        swapchainImageViews.resize(swapchainImages.size());

        for (size_t i = 0; i < swapchainImages.size(); ++i)
        {
            viewCreateInfo.image = swapchainImages[i];
            result = vkDevice.createImageView(&viewCreateInfo, nullptr, &swapchainImageViews[i]);
            if (result != vk::Result::eSuccess)
            {
                NGIN_ERROR("Failed to create image view for swapchain image {}", i);
                return false;
            }
        }

        vkSwapchainImageFormat = chosenFormat.format;

        return true;
    }


    int VulkanRenderer::RateDeviceSuitability(const vk::PhysicalDevice& device) const
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
        for (const auto& family: queueFamilies)
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
        // Create a temporary VkSurfaceKHR handle
        VkSurfaceKHR tempSurface;

        // Attempt to create a Vulkan surface using SDL
        if (!SDL_Vulkan_CreateSurface(window->GetSDLWindow(), static_cast<VkInstance>(vkInstance), &tempSurface))
        {
            NGIN_ERROR("Failed to create Vulkan surface. SDL: {}", SDL_GetError());
            return false;
        }

        // Convert the VkSurfaceKHR handle to a vk::SurfaceKHR handle
        vkSurface = vk::SurfaceKHR(tempSurface);
        return true;
    }
}
