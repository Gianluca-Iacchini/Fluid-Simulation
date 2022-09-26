#pragma once

#include "Core/Core.h"
#include "Core/Events/Event.h"

/*
	Layer management class for events and rendering sorting from Hazel
	https://github.com/TheCherno/Hazel
*/

namespace FluidSimulation {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}