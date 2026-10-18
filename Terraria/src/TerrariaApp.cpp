#include "TerrariaApp.h"

#include <algorithm>
#include <cmath>

#define WORLD_GENERATION_TEST 0

namespace Terraria
{
	TerrariaLayer::TerrariaLayer()
	{
	}

	TerrariaLayer::~TerrariaLayer()
	{
	}

	void TerrariaLayer::OnInit()
	{
		auto window = Engine::App::Get().GetWindow();
		glm::vec2 pos = window->GetMonitorSize();
		window->SetWindowPosition({ pos.x / 2, pos.y / 2 });

		m_Camera.reset(new Engine::Camera((float)window->GetWidth() / (float)window->GetHeight()));

		m_World = std::make_unique<World>();
		m_World->Init();

#if WORLD_GENERATION_TEST 
		m_Camera->SetZoomLevel(m_World->GetWidth() * World::TileSpacing / 2.0f);
		m_Camera->SetPosition({ m_World->GetWidth() * World::TileSpacing / 2.0f, -(m_World->GetHeight() * World::TileSpacing / 2.0f), 0.0f });
#else
		m_Camera->SetZoomLevel(3.0f);
		m_Camera->SetPosition({ m_World->GetSpawnPoint(), 0.0f });
#endif // WORLD_GENERATION_TEST

	}

	void TerrariaLayer::OnDestroy()
	{
	}

	void TerrariaLayer::OnUpdate(Engine::TimeStep ts)
	{
		ProcessInput(ts);
		m_World->Update(ts);
	}


	void TerrariaLayer::OnRender()
	{
		Engine::Renderer::BeginRender(*m_Camera);

		m_World->Render(*m_Camera);

		Engine::Renderer::EndRender();
	}

	void TerrariaLayer::OnEvent(Engine::Event& e) { 
		Engine::EventHandler handler(e);
		handler.Handle<Engine::WindowResizeEvent>([this](const Engine::WindowResizeEvent& ev) { m_Camera->SetAspectRatio((float)ev.GetWidth() / (float)ev.GetHeight()); });
		handler.Handle<Engine::MouseScrollEvent>([this](const Engine::MouseScrollEvent& ev) { CameraZoom((float)ev.GetYoffset()); });
		//handler.Handle<Engine::WindowCloseEvent >([](const Engine::WindowCloseEvent&)     { LOG_TRACE("WindowCloseEvent"); }); 
		//handler.Handle<Engine::KeyPressedEvent  >([](const Engine::KeyPressedEvent& ev)   { LOG_TRACE("KeyPressedEvent: key={}, repeat={}", ev.GetKeyCode(), ev.IsRepeat()); }); 
		//handler.Handle<Engine::KeyReleasedEvent >([](const Engine::KeyReleasedEvent& ev)  { LOG_TRACE("KeyReleasedEvent: key={}", ev.GetKeyCode()); }); 
		//handler.Handle<Engine::MouseMovedEvent  >([](const Engine::MouseMovedEvent& ev)   { LOG_TRACE("MouseMovedEvent: x={}, y={}", ev.GetxPos(), ev.GetyPos()); }); 
		//handler.Handle<Engine::MouseScrollEvent >([](const Engine::MouseScrollEvent& ev)  { LOG_TRACE("MouseScrollEvent: x={}, y={}", ev.GetXoffset(), ev.GetYoffset()); });
	}

	void TerrariaLayer::ProcessInput(Engine::TimeStep ts)
	{
		if (Engine::Input::OnKeyPressed(Engine::Key::ESCAPE)) {
			Engine::App::Get().Close();
		}
		CameraMovement(ts);
	}

	void TerrariaLayer::CameraMovement(Engine::TimeStep ts)
	{
		glm::vec3 CamPos = m_Camera->GetPosition();
		static float camSpeed = 1.0f;

		// Scale by zoom so panning feels the same at any zoom level
		float camStep = camSpeed * ts;

		if (Engine::Input::IsKeyPressed(Engine::Key::W)) {
			CamPos += glm::vec3(0.0f, camStep,0.0f);
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::S)) {
			CamPos += glm::vec3(0.0f, -camStep, 0.0f);
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::A)) {
			CamPos += glm::vec3(-camStep, 0.0f, 0.0f);
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::D)) {
			CamPos += glm::vec3(camStep, 0.0f, 0.0f);
		}
		if (Engine::Input::IsKeyPressed(Engine::Key::LEFT_CONTROL)) {
			camSpeed = 20.0f;
		}else{
			camSpeed = 4.0f;
		}

		m_Camera->SetPosition(CamPos);
	}

	void TerrariaLayer::CameraZoom(float scrollOffset)
	{
		static const float zoomStep = 1.1f;
		static const float minZoom = 1.0f;

#if WORLD_GENERATION_TEST
		static const float maxZoom = 1000.0f;
#else
		static const float maxZoom = 6.0f;
#endif // WORLD_GENERATION_TEST
	
		float zoom = m_Camera->GetZoomLevel() * std::pow(zoomStep, -scrollOffset);
		m_Camera->SetZoomLevel(std::clamp(zoom, minZoom, maxZoom));
	}
}
