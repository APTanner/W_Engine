#pragma once 
#include "VertexArray.h"
#include "Transform.h"
#include "Shader.h"

namespace W_Engine
{
	class Renderer
	{
	public:
		static void Render(const VertexArray& vertexArray, const Transform& transform, Shader& shader);
		static void EnableDepthTest(bool enable);
	private:
	};
}