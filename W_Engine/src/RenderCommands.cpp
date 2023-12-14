#include "RenderCommands.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <W_Engine/Camera.h>

namespace W_Engine
{

	void RenderCommands::SetClearColor(glm::vec4 color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void RenderCommands::ClearColorBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderCommands::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	
	void RenderCommands::DrawElements(const VertexArray& vertexArray, const Transform& transform, Shader& shader)
	{
		Camera& activeCamera = Camera::GetActive();

		shader.Bind();
		shader.SetProjectionMatrix(activeCamera.GetProjection());
		shader.SetViewMatrix(activeCamera.GetView());
		shader.SetLocalToWorldMatrix(transform.LocaltoWorld());

		vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, vertexArray.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}
	void RenderCommands::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}
	void RenderCommands::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}
}