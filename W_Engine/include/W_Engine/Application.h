#pragma once

#include <W_Engine/Window.h>
#include <W_Engine/Event.h>
#include <W_Engine/Renderer.h>
#include <W_Engine/ResourceManager.h>
#include <W_Engine/Camera.h>

namespace W_Engine
{
	class Application
	{
	public:
		Application();
		~Application();

        inline static bool IsRunning() { return Get().m_running; }
		inline static Application& Get() { return *m_instance; }

        Renderer& GetRenderer() { return m_renderer; }
        ResourceManager& GetResourceManager() { return m_resourceManager; }

		EventQueue& GetEventQueue() { return m_eventQueue; }
		EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }

		void PreRender(Camera& camera);
        void PostRender();
	private:
		static Application* m_instance;
		bool m_running = false;
		Window* m_window;

		EventDispatcher m_eventDispatcher;
		EventQueue m_eventQueue;
        Renderer m_renderer;
        ResourceManager m_resourceManager;
	private:
		void RegisterEvents();
	};
}

