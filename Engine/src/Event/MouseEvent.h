#pragma once
#include "Event.h"

namespace Engine
{
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			:m_xOffset(xOffset), m_yOffset(yOffset)
		{
			SetCategory(EventCategory::Mouse);
		}

		float GetXoffset() const { return m_xOffset; }
		float GetYoffset() const { return m_yOffset; }
	private:
		float m_xOffset, m_yOffset;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int xPos, int yPos)
			:m_xPos(xPos), m_yPos(yPos)
		{
			SetCategory(EventCategory::Mouse);
		}

		int GetxPos() const { return m_xPos; }
		int GetyPos() const { return m_yPos; }
	private:
		int m_xPos, m_yPos;
	};
}