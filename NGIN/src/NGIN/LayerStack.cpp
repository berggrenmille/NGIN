#include <PCH.h>
#include "NGIN/LayerStack.h"
using namespace NGIN;

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

void LayerStack::PushOverlay(Layer* overlay)
{
	layers.emplace_back(overlay);

}

void LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(layers.begin(), layers.end(), overlay);
	if (it != layers.end())
	{
		layers.erase(it);
		delete overlay;
	}
}

void LayerStack::OnUpdate()
{
	for (Layer* layer : layers)
	{
		layer->OnUpdate();
	}
}
