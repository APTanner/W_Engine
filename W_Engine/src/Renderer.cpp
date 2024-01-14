#include <W_Engine/Renderer.h>

#include "RenderCommands.h"

namespace W_Engine
{
    void Renderer::SetCamera(Camera& camera)
    {
        camera.UpdateMatricies();
        m_activeCamera = &camera;
    }

	void Renderer::Render(const VertexArray& vertexArray, const Transform& transform, Shader& shader)
	{
		RenderCommands::DrawElements(*m_activeCamera, vertexArray, transform, shader);
	}

	void Renderer::EnableDepthTest(bool enable)
	{
		if (enable)
		{
			RenderCommands::EnableDepthTest();
		}
        else
        {
            RenderCommands::DisableDepthTest();
        }
	}

    void Renderer::SetBackgroundColor(glm::vec4 color)
    {
        RenderCommands::SetClearColor(color);
    }
}
