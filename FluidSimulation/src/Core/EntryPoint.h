#pragma once
// Application entry point

#ifdef FS_PLATFORM_WINDOWS

extern FluidSimulation::Application* FluidSimulation::CreateApplication();

int main(int argc, char** argv)
{
	FluidSimulation::Log::Init();
	FS_CORE_WARN("Initialized Log!");
	FS_INFO("Initialized Client!");

	auto app = FluidSimulation::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif