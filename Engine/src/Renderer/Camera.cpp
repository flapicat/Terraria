#include "pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	Camera::Camera(float aspectRatio)
		:m_AspectRatio(aspectRatio)
	{
		m_CameraPos = glm::vec3(0.0f);
		m_Near = -1.0f;
		m_Far = 1.0f;
		m_ZoomLevel = 1.0f;

		UpdateCamera();
	}

	ViewBounds Camera::GetViewBounds() const
	{
		ViewBounds viewBound;
		viewBound.Left = m_CameraPos.x - m_AspectRatio * m_ZoomLevel;
		viewBound.Right = m_CameraPos.x + m_AspectRatio * m_ZoomLevel;
		viewBound.Top = m_CameraPos.y + m_ZoomLevel;
		viewBound.Bottom = m_CameraPos.y - m_ZoomLevel;
		return viewBound;
	}

	void Camera::UpdateCamera()
	{
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_Near, m_Far);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPos);
		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}