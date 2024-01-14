#pragma once

#include <glm/glm.hpp>

#include <W_Engine/Log.h>
#include <W_Engine/Transform.h>

namespace W_Engine 
{
	class Camera
	{
	public:
		Camera(float fieldOfView);
		~Camera();

		static Camera& GetActive()
		{
			if (s_activeCamera == nullptr)
			{
				LOG_ERROR("No active Camera");
			}
			return *s_activeCamera;
		}

		void SetActive();

		Transform& GetTransform() { return m_transform; }
        void UpdateMatricies() { UpdateViewMatrix(); UpdateProjectionMatrix(); }
		inline const glm::mat4& GetView() const { return m_viewMatrix; }
		inline const glm::mat4& GetProjection() const { return m_projectionMatrix; }
	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void SubscribeToOnScreenResize();
	private:
        Transform m_transform{};
        glm::mat4 m_viewMatrix{};
        glm::mat4 m_projectionMatrix{};

		float m_fieldOfView;
	private:
		static Camera* s_activeCamera;
	};
}

