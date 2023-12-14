#include "WindowManager.h"

#include <W_Engine/Log.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace W_Engine
{

	WindowManager::WindowManager()
	{

	}

	WindowManager::~WindowManager()
	{
		LOG_DEBUG("Destructor called on window manager");

		glfwTerminate();
	}

	Window* WindowManager::InstantiateWindow(const Window::Data& windowData)
	{
		if (!m_initialized)
		{
			m_initialized = true;
			if (!glfwInit())
			{
				LOG_ERROR("Failed to initialize glfw");
			}
		}
		//window has private constructor and so can't be constructed by make_unique()
		m_window = std::unique_ptr<Window>(new Window(windowData));
		glfwMakeContextCurrent(m_window->GetGLFWwindow());
		m_window->SetVSync(windowData.VSync);
		m_window->SetCallbacks();
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialize glad");
		}
		return m_window.get();
	}

	void WindowManager::SwapWindowBuffers() const
	{
		glfwSwapBuffers(m_window->GetGLFWwindow());
	}

	void WindowManager::PollEvents() const
	{
		glfwPollEvents();
	}
}