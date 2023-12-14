#include <W_Engine/Renderer.h>

#include "RenderCommands.h"

namespace W_Engine
{
	void Renderer::Render(const VertexArray& vertexArray, const Transform& transform, Shader& shader)
	{
		RenderCommands::DrawElements(vertexArray, transform, shader);
	}
	void Renderer::EnableDepthTest(bool enable)
	{
		if (enable)
		{
			RenderCommands::EnableDepthTest();
		}
		RenderCommands::DisableDepthTest();
	}

}
