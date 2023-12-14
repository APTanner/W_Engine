#pragma once

namespace W_Engine
{
	class Time
	{
	public:
		static void Update();
		static float DeltaTime() { return static_cast<float>(m_deltaTime); }
	private:
		static double m_lastFrameTime;
		static double m_deltaTime;
	};
}