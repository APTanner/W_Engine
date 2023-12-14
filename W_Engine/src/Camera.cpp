#include <W_Engine/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

#include <W_Engine/Application.h>

namespace W_Engine
{
    constexpr float aspectRatio = 1.5f;

    Camera* Camera::s_activeCamera = nullptr;

    Camera::Camera(float fieldOfView)
    {
        m_viewMatrix = glm::mat4(1.0f);
        m_projectionMatrix = glm::mat4(1.0f);
        UpdateViewMatrix();
        m_fieldOfView = glm::radians(fieldOfView);
        UpdateProjectionMatrix();

        //subscribe to event
        SubscribeToOnScreenResize();
    }
    Camera::~Camera()
    {
    }

    void Camera::SetActive()
    {
        s_activeCamera = this;
    }

    void Camera::UpdateViewMatrix()
    {
       m_viewMatrix = glm::lookAt(
            m_transform.GetPosition(),
            m_transform.GetForward() + m_transform.GetPosition(),
            m_transform.GetUp());
    }

    void Camera::UpdateProjectionMatrix()
    {
        m_projectionMatrix = glm::perspective(
            m_fieldOfView, 
            aspectRatio,
            0.1f, 100.0f);
    }

    void Camera::SubscribeToOnScreenResize()
    {
        Application::Get().GetEventDispatcher().RegisterCallback<WindowResizeEvent>
        (
            [this](const WindowResizeEvent& e) -> bool
            {
                UpdateProjectionMatrix();
                return false;
            }
        );
    }
}
