#include <W_Engine/Time.h>

#include <GLFW/glfw3.h>

namespace W_Engine
{
	double Time::m_deltaTime = 0.0f;
	double Time::m_lastFrameTime = 0.0f;

	void Time::Update()
	{
		double currentTime = glfwGetTime();
		m_deltaTime = currentTime - m_lastFrameTime;
		m_lastFrameTime = currentTime;
	}
}