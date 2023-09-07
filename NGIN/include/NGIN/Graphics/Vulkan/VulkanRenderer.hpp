#pragma once

#include <NGIN/Core.h>
#include <NGIN/Graphics/Renderer.hpp>
#include <vulkan/vulkan.hpp>

namespace NGIN::Graphics
{
	// Forward declaration of the Window class.
	class Window;

	/**
	 * @class VulkanRenderer
	 * @brief A Renderer implementation using the Vulkan Graphics API.
	 *
	 * This class provides functionalities for rendering using Vulkan.
	 * It manages Vulkan resources such as instances, devices, and surfaces, and provides
	 * core rendering functionalities like viewport setting, frame clearing, and more.
	 */
	class VulkanRenderer : public Renderer
	{
	public:
		NGIN_API VulkanRenderer(Window &window);
		NGIN_API ~VulkanRenderer();

		NGIN_API virtual bool Initialize() override;
		NGIN_API virtual void BeginFrame() override;
		NGIN_API virtual void EndFrame() override;
		NGIN_API virtual void SetViewport(int width, int height) override;
		NGIN_API virtual void Clear(float r, float g, float b, float a) override;

	private:
		bool SetupInstance();
		bool SetupSurface();
		bool SetupDevice();

		int RateDeviceSuitability(const vk::PhysicalDevice &device) const;

		Window &window;
		vk::Instance vkInstance;
		vk::SurfaceKHR vkSurface;
		vk::PhysicalDevice vkPhysicalDevice;
		vk::Device vkDevice;

		// ... Other Vulkan-specific resources (e.g., command buffers, pipeline states, etc.)
	};

}
