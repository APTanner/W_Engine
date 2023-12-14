#pragma once

#include <string>

struct GLFWwindow;

namespace W_Engine
{
	class WindowManager;

	class Window
	{
	public:
		friend class WindowManager;

		struct Data
		{
			std::string Title;
			int Width;
			int Height;
			bool VSync;

			Data
			(
				const std::string& title = "Window",
				int width = 1080,
				int height = 720,
				bool vSync = true
			)
				: Title(title), Width(width), Height(height), VSync(vSync)
			{
			}
		};
	public:
		inline int GetWidth() const { return m_data.Width; }
		inline int GetHeight() const { return m_data.Height; }
		inline GLFWwindow* GetGLFWwindow() const { return m_window; }

		void SetCallbacks();
		void SetVSync(bool enabled);
	private:
		GLFWwindow* m_window;
		Data m_data;
	private:
		Window(const Data& data);
	};
}

