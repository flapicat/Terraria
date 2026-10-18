#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct ViewBounds
	{
		float Left;
		float Right;
		float Top;
		float Bottom;
	};

	class Camera
	{
	public:
		Camera(float aspectRatio);
		~Camera() = default;

		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio;  UpdateCamera(); }
		float GetAspectRatio() const { return m_AspectRatio; }

		void SetNear(float Near) { m_Near = Near; UpdateCamera(); }
		float GetNear() const { return m_Near; }

		void SetFar(float Far) { m_Far = Far; UpdateCamera(); }
		float GetFar() const { return m_Far; }

		void SetPosition(const glm::vec3& position) { m_CameraPos = position;  UpdateCamera();}
		glm::vec3 GetPosition() const { return m_CameraPos; }

		// Half the visible height in world units; larger values show more of the world
		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; UpdateCamera(); }
		float GetZoomLevel() const { return m_ZoomLevel; }

		ViewBounds GetViewBounds() const;

		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void UpdateCamera();
	private:
		float m_AspectRatio;
		float m_Near, m_Far;
		float m_ZoomLevel;
		glm::vec3 m_CameraPos;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

}