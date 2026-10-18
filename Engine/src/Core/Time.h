#pragma once

#include <GLFW/glfw3.h>

namespace Engine {
	inline double GetTime() { return glfwGetTime(); }

	class Timer {
	public:
		Timer() {
			m_TimeAtStart = GetTime();
		}

		double GetTimePassedMS() {  return (Engine::GetTime() - m_TimeAtStart) * 1000.0; }
		double GetTimePassed() { return (Engine::GetTime() - m_TimeAtStart); }
	private:
		double m_TimeAtStart = 0;
	};
}