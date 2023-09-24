#pragma once
#include <NGIN/Defines.hpp>
#include "GraphicsBackend.hpp"
namespace NGIN::Graphics
{
	class Renderer;
	class Window;

	class NGIN_API Context
	{
	public:
		Context(GraphicsBackend backend, const std::string& title, int width, int height);
		~Context();

		Renderer* GetRenderer() const { return renderer; }

	private:
		Renderer* renderer = nullptr;
		Window* window = nullptr;
	};
}
