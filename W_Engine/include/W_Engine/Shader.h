#pragma once
#include <string>
#include <glm/glm.hpp>

namespace W_Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetLocalToWorldMatrix(const glm::mat4& localToClip);
		void SetViewMatrix(const glm::mat4& view);
		void SetProjectionMatrix(const glm::mat4& projection);
		void SetMat4(const std::string& name, const glm::mat4& matrix);
        void SetInt(const std::string& name, const int value);
	private:
		unsigned int m_shader = 0;
	};
}