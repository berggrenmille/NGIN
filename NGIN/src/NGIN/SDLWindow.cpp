#include <PCH.h>
#include "NGIN/SDLWindow.h"
using namespace NGIN;
SDLWindow::SDLWindow() : window(nullptr), renderer(nullptr) {}

SDLWindow::~SDLWindow()
{
	destroy();
}

void SDLWindow::create(const std::string& title, int width, int height)
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void SDLWindow::destroy()
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	if (window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

bool SDLWindow::isOpen() const
{
	return window != nullptr;
}

void SDLWindow::update()
{
	// Process SDL events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			destroy();
		}
	}
}

void SDLWindow::clear()
{
	SDL_RenderClear(renderer);
}

void SDLWindow::display()
{
	SDL_RenderPresent(renderer);
}

int SDLWindow::getWidth() const
{
	int w;
	SDL_GetWindowSize(window, &w, nullptr);
	return w;
}

int SDLWindow::getHeight() const
{
	int h;
	SDL_GetWindowSize(window, nullptr, &h);
	return h;
}
