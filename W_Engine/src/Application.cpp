#include <W_Engine/Application.h>

#include "Macros.h"
#include "WindowManager.h"
#include "RenderCommands.h"

#include <W_Engine/Event.h>
#include <W_Engine/Time.h>

namespace W_Engine
{
	Application* Application::m_instance = nullptr;

	Application::Application()
		: m_eventDispatcher(), m_eventQueue(m_eventDispatcher)
	{
		ASSERT(m_instance == nullptr, "Can't have multiple applications.");
		m_instance = this;

		m_window = WindowManager::GetInstance().InstantiateWindow();

		RegisterEvents();
	}

	Application::~Application()
	{
		m_instance = nullptr;
	}

	void Application::Run()
	{
        m_running = true;

		RenderCommands::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
		RenderCommands::EnableDepthTest();

        while (m_running)
        {
			Time::Update();

			WindowManager::GetInstance().PollEvents();
			m_eventQueue.DispatchEvents();

			RenderCommands::ClearColorBuffer();
			RenderCommands::ClearDepthBuffer();

            WindowManager::GetInstance().SwapWindowBuffers();
        }
    }

	void Application::RegisterEvents()
	{
		m_eventDispatcher.RegisterCallback<WindowCloseEvent>(
			[this](const WindowCloseEvent& event) -> bool
			{
				m_running = false;
				return true;
			}
		);
	}
	
}
