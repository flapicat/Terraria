#pragma once
#include <string_view>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Event/Event.h"

namespace Engine {
	class Window {
	public:
		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();

		void Create(uint32_t width, uint32_t height, std::string_view title);
		void Delete();
		void Update();
		GLFWwindow* GetNativeWindow() const { return m_Window; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		
		void SetWindowPosition(uint32_t x, uint32_t y);
		void SetWindowPosition(const glm::vec2& pos);
		glm::vec2 GetWindowPosition() const;
		glm::vec2 GetMonitorSize() const;

		void SetEventCallback(const std::function<void(Event&)>& callback) { m_EvCallback = callback; }
	private:
		GLFWwindow* m_Window;
		uint32_t m_Width;
		uint32_t m_Height;
		std::function<void(Event&)> m_EvCallback;
	};
}