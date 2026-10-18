#include <pch.h>
#include "Window.h"

#include <glad/glad.h>

#include "Event/Events.h"

namespace Engine {
	Window::Window(uint32_t width, uint32_t height, std::string_view title)
	{
		Create(width, height, title);
	}

	Window::~Window()
	{
		if (m_Window)
			this->Delete();
	}

	void Window::Create(uint32_t width, uint32_t height, std::string_view title)
	{
		m_Width = width;
		m_Height = height;

		glfwSetErrorCallback([](int code, const char* description) {
			LOG_ERROR("GLFW error 0x{:X}: {}", code, description);
			});

		if (!glfwInit()) {
			LOG_ERROR("Failed to initialize GLFW, the window cannot be created");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		m_Window = glfwCreateWindow(m_Width, m_Height, title.data(), nullptr, nullptr);
		if (!m_Window) {
			LOG_ERROR("Failed to create a {}x{} window with an OpenGL 4.6 context (the GPU or driver may not support 4.6)", m_Width, m_Height);
			return;
		}
		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			LOG_ERROR("Failed to load OpenGL functions with GLAD, nothing can be rendered");
			return;
		}

		LOG_INFO("Window created: {}x{} \"{}\"", m_Width, m_Height, title);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			win->m_EvCallback(event);
			});


		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			MouseScrollEvent event(xoffset, yoffset);
			win->m_EvCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event(xpos, ypos);
			win->m_EvCallback(event);
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width, height);
			win->m_Width = width;
			win->m_Height = height;
			win->m_EvCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					win->m_EvCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					win->m_EvCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					win->m_EvCallback(event);
					break;
				}
				}
			});
	}

	void Window::Delete()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::SetWindowPosition(uint32_t x, uint32_t y)
	{
		glfwSetWindowPos(m_Window, x - m_Width/2, y - m_Height/2);
	}

	void Window::SetWindowPosition(const glm::vec2& pos)
	{
		glfwSetWindowPos(m_Window, pos.x - m_Width / 2, pos.y - m_Height / 2);
	}

	glm::vec2 Window::GetWindowPosition() const
	{
		int32_t x = 0, y = 0;
		glfwGetWindowPos(m_Window, &x, &y);
		return glm::vec2(x, y);
	}

	glm::vec2 Window::GetMonitorSize() const
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		return glm::vec2(mode->width,mode->height);
	}
}