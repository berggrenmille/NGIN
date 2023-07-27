#pragma once

#include <SDL.h>
#include "Window.h"
namespace NGIN
{
	class SDLWindow : public Window
	{
	public:
		SDLWindow();
		virtual ~SDLWindow();

		virtual void create(const std::string& title, int width, int height) override;
		virtual void destroy() override;

		virtual bool isOpen() const override;

		virtual void update() override;

		virtual void clear() override;
		virtual void display() override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
	};
}