#pragma once

#include <vector>
#include "Layer.hpp"
namespace NGIN
{

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(NGIN::Layer *layer);
		void PopLayer(NGIN::Layer *layer);

		void PushOverlay(NGIN::Layer *overlay);
		void PopOverlay(NGIN::Layer *overlay);

		void OnUpdate();

		std::vector<NGIN::Layer *>::iterator begin() {}

	private:
		std::vector<NGIN::Layer *> layers;
	};
}