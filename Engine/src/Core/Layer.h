#pragma once
#include "TimeStep.h"

#include "Event/Event.h"

namespace Engine
{
	class Layer
	{
	public:
		virtual ~Layer() {};

		virtual void OnInit() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& e) = 0;
	};

}