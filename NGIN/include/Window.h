#pragma once

#include <string>
namespace NGIN
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void create(const std::string& title, int width, int height) = 0;
		virtual void destroy() = 0;

		virtual bool isOpen() const = 0;

		virtual void update() = 0;

		virtual void clear() = 0;
		virtual void display() = 0;

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
	};
}