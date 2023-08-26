#include <PCH.h>
#include "NGIN/LayerStack.h"
using namespace NGIN;

LayerStack::~LayerStack()
{
	for (LayerT *layer : layers)
	{
		delete layer;
	}
}

void LayerStack::PushLayer(LayerT *layer)
{
	layers.emplace_back(layer);
}

void LayerStack::PopLayer(LayerT *layer)
{
	auto it = std::find(layers.begin(), layers.end(), layer);
	if (it != layers.end())
	{
		layers.erase(it);
		delete layer;
	}
}

void LayerStack::PushOverlay(LayerT *overlay)
{
	layers.emplace_back(overlay);
}

void LayerStack::PopOverlay(LayerT *overlay)
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
	for (LayerT *layer : layers)
	{
		layer->OnUpdate();
	}
}
