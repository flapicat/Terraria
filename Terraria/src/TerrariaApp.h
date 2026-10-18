#pragma once
#include "Engine.h"

#include "World/World.h"

namespace Terraria
{
	class TerrariaLayer : public Engine::Layer
	{
	public:
		TerrariaLayer();
		~TerrariaLayer();

		virtual void OnInit() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate(Engine::TimeStep ts) override;
		virtual void OnRender() override;
		virtual void OnEvent(Engine::Event& e) override;
	private:
		void ProcessInput(Engine::TimeStep ts);
		void CameraMovement(Engine::TimeStep ts);
		void CameraZoom(float scrollOffset);
	private:
		std::unique_ptr<World> m_World;
		std::shared_ptr<Engine::Camera> m_Camera;
	};
}
