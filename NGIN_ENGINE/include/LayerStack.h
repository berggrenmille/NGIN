#pragma once

#include <vector>
#include "Layer.h"

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

private:
	std::vector<Layer*> layers;
};