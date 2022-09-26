#pragma once

/*
	spdlog wrapper function for Log management from Hazel
	https://github.com/TheCherno/Hazel
*/


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace FluidSimulation {
	class Log
	{
	public:
		Log();
		~Log();
	
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger; 
	};

}

// Core log Macros
#define FS_CORE_FATAL(...) ::FluidSimulation::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define FS_CORE_ERROR(...) //::FluidSimulation::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FS_CORE_WARN(...) ::FluidSimulation::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FS_CORE_INFO(...) ::FluidSimulation::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FS_CORE_TRACE(...) ::FluidSimulation::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log Macros
#define FS_FATAL(...) ::FluidSimulation::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define FS_ERROR(...) //::FluidSimulation::Log::GetClientLogger()->error(__VA_ARGS__)
#define FS_WARN(...) ::FluidSimulation::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FS_INFO(...) ::FluidSimulation::Log::GetClientLogger()->info(__VA_ARGS__)
#define FS_TRACE(...) ::FluidSimulation::Log::GetClientLogger()->trace(__VA_ARGS__)
