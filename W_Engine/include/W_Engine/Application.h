#pragma once

#include <W_Engine/Window.h>
#include <W_Engine/Event.h>

namespace W_Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		inline static Application& Get() { return *m_instance; }
		EventQueue& GetEventQueue() { return m_eventQueue; }
		EventDispatcher& GetEventDispatcher() { return m_eventDispatcher; }

		void Run();
	private:
		static Application* m_instance;
		bool m_running = false;
		Window* m_window;

		EventDispatcher m_eventDispatcher;
		EventQueue m_eventQueue;
	private:
		void RegisterEvents();
	};
}

