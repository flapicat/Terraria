#pragma once
#include <memory>
#include "Window.h"
#include "LayerStack.h"

#include "Event/Events.h"
namespace Engine {
	class App {
	public:
		App();
		~App();
	
		void Run();
		void Close();
		
		void OnEvent(Event& event);

		static App& Get() { return *s_Instance; }
		Window* GetWindow() { return m_Window.get(); };

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
	private:
		void OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		static App* s_Instance;
		bool m_Running = true;
		float m_LastFrame = 0;

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
	};
}