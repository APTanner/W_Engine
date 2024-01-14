#pragma once 

#include <glm/vec4.hpp>

#include <W_Engine/Camera.h>

#include "VertexArray.h"
#include "Transform.h"
#include "Shader.h"

namespace W_Engine
{
    
	class Renderer
	{
        friend class Application;
	public:
        void SetCamera(Camera& camera);
		void Render(const VertexArray& vertexArray, const Transform& transform, Shader& shader);
		void EnableDepthTest(bool enable);
        void SetBackgroundColor(glm::vec4 color);
    private:
        Camera* m_activeCamera = nullptr;
	private:
        Renderer() {}
        ~Renderer() {}
	};
}