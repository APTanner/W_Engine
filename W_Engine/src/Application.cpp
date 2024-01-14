#include <W_Engine/Application.h>

#include "Macros.h"
#include "WindowManager.h"
#include "RenderCommands.h"

#include <W_Engine/Event.h>
#include <W_Engine/Time.h>
#include <W_Engine/Buffer.h>
#include <W_Engine/VertexArray.h>

namespace W_Engine
{
	Application* Application::m_instance = nullptr;

    VertexArray* vertexArray;
    Shader* shader;

	Application::Application()
		: m_eventDispatcher(), m_eventQueue(m_eventDispatcher), 
        m_renderer(), m_resourceManager(), m_input(),
        m_running(true)
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

    void Application::InitializeAndPoll()
    {
        Time::Update();

        WindowManager::GetInstance().PollEvents();
        m_eventQueue.DispatchEvents();
    }

	void Application::PreRender(Camera& camera)
	{
		RenderCommands::ClearColorBuffer();
		RenderCommands::ClearDepthBuffer();

        m_renderer.SetCamera(camera);
    }

    void Application::PostRender()
    {
        WindowManager::GetInstance().SwapWindowBuffers();
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
