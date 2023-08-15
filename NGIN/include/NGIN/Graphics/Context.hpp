#pragma once

enum class GraphicsBackend
{
	Vulkan,
	OpenGL,
	DirectX12,
	//... other backends
};



class GraphicsContext
{
public:
	GraphicsContext(GraphicsBackend backend);

	// ... similarly for other components
};
