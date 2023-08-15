#pragma once

#include <NGIN/Graphics/Window.hpp>

namespace NGIN::Graphics
{
	class VulkanWindow : public Window
	{
		VulkanWindow() = default;
		~VulkanWindow();
		virtual bool Init(const std::string& title, int width, int height) override;
		virtual void Shutdown() override;


	};
}