#pragma once
#include <SDL2/SDL_vulkan.h>
#include "../Renderer.hpp"
#include <vulkan/vulkan.hpp>

namespace NGIN::Graphics
{
	class VulkanWindow;

	class VulkanRenderer : public Renderer
	{
	public:
		NGIN_API VulkanRenderer(VulkanWindow* window);
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

		VulkanWindow* vulkanWindow;
		vk::Instance		vkInstance;
		vk::SurfaceKHR		vkSurface;
		vk::PhysicalDevice  vkPhysicalDevice;
		vk::Device			vkDevice;

		// ... Other Vulkan-specific resources (e.g., command buffers, pipeline states, etc.)
	};

}
