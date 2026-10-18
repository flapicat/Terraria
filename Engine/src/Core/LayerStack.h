#pragma once
#include <vector>

#include "Layer.h"

namespace Engine
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::const_iterator cbegin() { return m_Layers.cbegin(); }
		std::vector<Layer*>::const_iterator cend() { return m_Layers.cend(); }

		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
		std::vector<Layer*>::const_reverse_iterator rcbegin() { return m_Layers.crbegin(); }
		std::vector<Layer*>::const_reverse_iterator rcend() { return m_Layers.crend(); }
	private:
		std::vector<Layer*> m_Layers;
	};

}