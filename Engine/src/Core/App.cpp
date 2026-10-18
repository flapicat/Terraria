#include <pch.h>
#include "App.h"

#include "Input.h"
#include "Time.h"
#include "TimeStep.h"

#include "Renderer/Renderer.h"

namespace Engine {
	App* App::s_Instance = nullptr;


	App::App() {
		s_Instance = this;
		constexpr uint32_t Width = 1920;
		constexpr uint32_t Height = 1080;
		m_Window.reset(new Window(Width, Height, "Terraria"));
		m_Window->SetEventCallback([this](Event& e) {OnEvent(e); });
		Renderer::InitRenderer();
	}

	App::~App() {

	}

	void App::Run() {
		while (m_Running) {
			float time = Engine::GetTime();
			TimeStep timeStep = time - m_LastFrame;
			m_LastFrame = time;

			glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto& layer : m_LayerStack){
				layer->OnUpdate(timeStep);
			}

			for (auto& layer : m_LayerStack){
				layer->OnRender();
			}

			m_Window->Update();
		}
		LOG_INFO("Main loop stopped, shutting down");
	}

	void App::OnEvent(Event& event)
	{
		EventHandler hander(event);
		hander.Handle<WindowCloseEvent>([this](const WindowCloseEvent& ev) {OnWindowCloseEvent(ev); });

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
			(*it)->OnEvent(event);
	}

	void App::Close() {
		m_Running = false;
	}

	void App::PushLayer(Layer* layer){
		m_LayerStack.PushLayer(layer);
		layer->OnInit();
	}

	void App::PopLayer(Layer* layer){
		layer->OnDestroy();
		m_LayerStack.PopLayer(layer);
	}

	void App::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		m_Running = false;
	}
}