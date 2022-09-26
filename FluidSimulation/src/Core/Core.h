#pragma once
// Core defines for development

#ifdef FS_PLATFORM_WINDOWS
	#ifdef FS_BUILD_DLL
		#define FLUID_API __declspec(dllexport)
	#else
		#define FLUID_API __declspec(dllimport)
	#endif
#endif

#ifdef FS_ENABLE_ASSERTS
	#define FS_ASSERT(x, ...) {if (!(x)) {FS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define FS_CORE_ASSERT(x, ...) {if (!(x)) {FS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define FS_ASSERT(x, ...) 
	#define FS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

