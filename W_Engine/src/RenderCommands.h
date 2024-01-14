#pragma once

#include <glm/vec4.hpp>

#include <W_Engine/VertexArray.h>
#include <W_Engine/Transform.h>
#include <W_Engine/Shader.h>
#include <W_Engine/Camera.h>

namespace W_Engine
{
	class RenderCommands
	{
	public:
		static void SetClearColor(glm::vec4 color);
		static void ClearColorBuffer();
		static void ClearDepthBuffer();

		static void DrawElements(
            const Camera& camera,
            const VertexArray& vertexArray, 
            const Transform& transform, 
            Shader& shader
        );


		static void EnableDepthTest();
		static void DisableDepthTest();

	};
}

