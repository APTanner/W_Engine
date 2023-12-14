#include <W_Engine/Window.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <W_Engine/Application.h>
#include <W_Engine/Event.h>

namespace W_Engine
{
	Window::Window(const Data& data)
	{
		m_data = data;
		m_window = glfwCreateWindow(
			m_data.Width,
			m_data.Height,
			m_data.Title.c_str(),
			NULL,
			NULL
		);
		glfwSetWindowUserPointer(m_window, &m_data);
	}

	void Window::SetCallbacks()
	{
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				std::unique_ptr<Event> event;
				switch (action)
				{
				case GLFW_PRESS:
					event = std::make_unique<KeyPressedEvent>(key, 0);
					break;
				case GLFW_RELEASE:
					event = std::make_unique<KeyReleasedEvent>(key);
					break;
				case GLFW_REPEAT:
					event = std::make_unique<KeyPressedEvent>(key, 1);
					break;
				}
				Application::Get().GetEventQueue().Push(std::move(event));
			});
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				std::unique_ptr<Event> event;
				switch (action)
				{
				case GLFW_PRESS:
					event = std::make_unique<MouseButtonPressEvent>(button);
					break;
				case GLFW_RELEASE:
					event = std::make_unique<MouseButtonReleaseEvent>(button);
					break;
				}
				Application::Get().GetEventQueue().Push(std::move(event));
			});
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				Application::Get().GetEventQueue().Push(
					std::move(
						std::make_unique<MouseMoveEvent>(
							static_cast<float>(xPos),
							static_cast<float>(yPos)
						)
					)
				);
			});
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				Application::Get().GetEventQueue().Push(
					std::move(
						std::make_unique<MouseScrollEvent>(
							static_cast<float>(xOffset),
							static_cast<float>(yOffset)
						)
					)
				);
			});
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
				data->Height = height;
				data->Width = width;

				Application::Get().GetEventQueue().Push(
					std::move(
						std::make_unique<WindowResizeEvent>(width, height)
					)
				);
			});
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				Application::Get().GetEventQueue().Push(
					std::move(
						std::make_unique<WindowCloseEvent>()
					)
				);
			});
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_data.VSync = enabled;
	}
}
