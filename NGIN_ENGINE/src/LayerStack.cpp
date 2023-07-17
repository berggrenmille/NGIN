#include "LayerStack.h"

LayerStack::~LayerStack()
{
	for (Layer* layer : layers)
	{
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(layers.begin(), layers.end(), layer);
	if (it != layers.end())
	{
		layers.erase(it);
		delete layer;
	}
}

void LayerStack::OnUpdate()
{
	for (Layer* layer : layers)
	{
		layer->OnUpdate();
	}
}
