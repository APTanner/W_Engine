#pragma once

#include <vector>
#include <memory>

#include <W_Engine/Window.h>

namespace W_Engine
{
	class WindowManager
	{
	public:
		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;

		static inline WindowManager& GetInstance() 
		{
			static WindowManager instance;
			return instance;
		}

		Window* InstantiateWindow(const Window::Data& windowData = Window::Data{});

		void SwapWindowBuffers() const;
		void PollEvents() const;
	private:
		//glfw handles its own resources so no need for unique_ptr
		//just want a way to keep track of all windows created
		//for now we will just have one window
		//std::vector<std::unique_ptr<Window>> m_windows = std::vector<std::unique_ptr<Window>>(1);
		std::unique_ptr<Window> m_window;
		bool m_initialized = false;
	private:
		WindowManager();
		~WindowManager();
	};
}

