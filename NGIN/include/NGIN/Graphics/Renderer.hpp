#pragma once
namespace NGIN::Graphics
{

	/**
	 * @brief Represents a common rendering interface that abstracts the specifics of any graphics API.
	 *
	 */
	class NGIN_API Renderer
	{
	public:
		/**
		 * @brief Virtual destructor to ensure proper cleanup for derived classes.
		 */
		virtual ~Renderer() = default;

		/**
		 * @brief Initializes the renderer and any necessary resources.
		 *
		 * @return true if initialization succeeded, false otherwise.
		 */
		virtual bool Initialize() = 0;

		/**
		 * @brief Begins a new frame for rendering.
		 */
		virtual void BeginFrame() = 0;

		/**
		 * @brief Ends the current frame and presents the result to the display.
		 */
		virtual void EndFrame() = 0;

		/**
		 * @brief Sets the viewport dimensions for rendering.
		 *
		 * @param width Width of the viewport.
		 * @param height Height of the viewport.
		 */
		virtual void SetViewport(int width, int height) = 0;

		/**
		 * @brief Clears the rendering target (e.g., screen, offscreen buffer) with the specified color.
		 *
		 * @param r Red component of the clear color.
		 * @param g Green component of the clear color.
		 * @param b Blue component of the clear color.
		 * @param a Alpha component of the clear color.
		 */
		virtual void Clear(float r, float g, float b, float a) = 0;



	};

}
