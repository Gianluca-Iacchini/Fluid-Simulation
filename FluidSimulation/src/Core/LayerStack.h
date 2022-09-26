#pragma once
#include "Core//Core.h"
#include "Core/Layer.h"

#include <vector>

/*
	Layer management class for events and rendering sorting from Hazel
	https://github.com/TheCherno/Hazel
*/

namespace FluidSimulation {
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}