#pragma once
#include <NGIN/Defines.hpp>
#include "GraphicsBackend.hpp"
#include <NGIN/Graphics/Window.hpp>
namespace NGIN::Graphics
{
	class Renderer;


	class NGIN_API Context
	{
	public:
		Context(GraphicsAPI backend, WindowSettings* windowSettings, const SourceLocation& srcLocation = SourceLocation::current());
		~Context();

		bool Init(GraphicsAPI backend, WindowSettings* windowSettings, const SourceLocation& srcLocation = SourceLocation::current());
		void Shutdown();

		void Tick();



		inline Window* GetWindow() const { return window; }
		inline Renderer* GetRenderer() const { return renderer; }

	private:
		Renderer* renderer = nullptr;
		Window* window = nullptr;
	};
}
