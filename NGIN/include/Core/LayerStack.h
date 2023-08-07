#pragma once

#include <vector>
#include "Layer.h"
namespace NGIN
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		void OnUpdate();

		std::vector<Layer*>::iterator begin() {}

	private:
		std::vector<Layer*> layers;
	};
}