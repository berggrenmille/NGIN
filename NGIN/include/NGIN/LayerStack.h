#pragma once

#include <vector>
#include "Layer.h"
namespace NGIN
{
	using LayerT = NGIN::Layer<>;
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(LayerT *layer);
		void PopLayer(LayerT *layer);

		void PushOverlay(LayerT *overlay);
		void PopOverlay(LayerT *overlay);

		void OnUpdate();

		std::vector<LayerT *>::iterator begin() {}

	private:
		std::vector<LayerT *> layers;
	};
}