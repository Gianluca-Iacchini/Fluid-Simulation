#include "fspch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

/*
	spdlog wrapper function for Log management from Hazel
	https://github.com/TheCherno/Hazel
*/


namespace FluidSimulation
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("FLUIDSIM");
		s_CoreLogger->set_level(spdlog::level::trace);
		
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	Log::Log() {}
	Log::~Log() {}

}