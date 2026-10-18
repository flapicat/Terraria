#include "pch.h"
#include "LayerStack.h"

namespace Engine
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (auto& l : m_Layers){
			l->OnDestroy();
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it == m_Layers.end())
		{
			LOG_WARN("PopLayer was given a layer that is not in the layer stack, nothing was removed");
			return;
		}
		m_Layers.erase(it);
	}
}