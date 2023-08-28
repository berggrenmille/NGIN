#pragma once

enum class GraphicsBackend
{
	Vulkan,
	OpenGL,
	DX12,
	//... other backends
};

class GraphicsContext
{
public:
	GraphicsContext(GraphicsBackend backend);

	// ... similarly for other components
};
