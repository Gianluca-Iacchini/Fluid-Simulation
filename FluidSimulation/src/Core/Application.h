#pragma once

#include "Window.h"
#include "Core/LayerStack.h"
#include "Core/Events/Event.h"
#include "Core/Events/ApplicationEvent.h"

namespace FluidSimulation {

	class Application
	{
	public:
		std::unique_ptr<Window> m_Window;
		Application();
		~Application();

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent e);

		bool m_Running = true;
		LayerStack m_LayerStack;
	
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();


}


