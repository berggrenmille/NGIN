#pragma once

#include "EventBus.h" // Include the event bus header file or any other necessary dependencies
namespace NGIN
{
	class Layer
	{
	public:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
	};
}