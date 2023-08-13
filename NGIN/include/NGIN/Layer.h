#pragma once

#include "EventBus.h" 
namespace NGIN
{
	class Layer
	{
	public:

		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
	};
}