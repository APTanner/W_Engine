#pragma once

#include <glm/glm.hpp>

namespace W_Engine
{
	class Transform
	{
	public:
		Transform();
		~Transform();

		// I don't allow scaling so the forward vector doesn't need to be normalized
		inline const glm::mat4x4& LocaltoWorld() const { return m_localtoWorld; }
		inline glm::vec3 GetPosition() const { return glm::vec3(m_localtoWorld[3]); }
		inline glm::vec3 GetForward() const { return -glm::vec3(m_localtoWorld[2]); }
		inline glm::vec3 GetUp() const { return glm::vec3(m_localtoWorld[1]); }
		inline glm::vec3 GetRight() const { return glm::vec3(m_localtoWorld[0]); }

		void SetPosition(const glm::vec3& position);
		void TranslateLocalSpace(const glm::vec3& v);

		void SetRotation(const glm::vec3& rotation);
		void Rotate(const glm::vec3& v);
	private:
		glm::mat4x4 m_localtoWorld = glm::mat4x4(1.0f);
	};
}

