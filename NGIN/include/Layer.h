#pragma once

#include "EventBus.h" 
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