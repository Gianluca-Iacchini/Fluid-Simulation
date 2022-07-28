workspace "FluidSimulation"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Includes --
IncludeDir = {}
IncludeDir["GLFW"] = "FluidSimulation/vendor/GLFW/include"
IncludeDir["Glad"] = "FluidSimulation/vendor/Glad/include"
IncludeDir["glm"] = "FluidSimulation/vendor/glm"
IncludeDir["assimp"] = "FluidSimulation/vendor/assimp"
IncludeDir["stb_image"] = "FluidSimulation/vendor/stb_image"
IncludeDir["bullet3"] = "FluidSimulation/vendor/bullet3/src"
IncludeDir["Bullet3Common"] = "FluidSimulation/vendor/bullet3/src/Bullet3Common"
IncludeDir["Bullet3Collision"] = "FluidSimulation/vendor/bullet3/src/Bullet3Collision"
IncludeDir["BulletCollision"] = "FluidSimulation/vendor/bullet3/src/BulletCollision"
IncludeDir["Bullet3Dynamics"] = "FluidSimulation/vendor/bullet3/src/Bullet3Dynamics"
IncludeDir["BulletDynamics"] = "FluidSimulation/vendor/bullet3/src/BulletDynamics"
IncludeDir["LinearMath"] = "FluidSimulation/vendor/bullet3/src/LinearMath"
IncludeDir["FreeType"] = "FluidSimulation/vendor/freetype"

include "FluidSimulation/vendor/GLFW"
include "FluidSimulation/vendor/Glad"
include "FluidSimulation/vendor/assimp"
include "FluidSimulation/vendor/bullet3/src/Bullet3Collision"
include "FluidSimulation/vendor/bullet3/src/BulletCollision"
include "FluidSimulation/vendor/bullet3/src/Bullet3Dynamics"
include "FluidSimulation/vendor/bullet3/src/BulletDynamics"
include "FluidSimulation/vendor/bullet3/src/Bullet3Common"
include "FluidSimulation/vendor/bullet3/src/LinearMath"
include "FluidSimulation/vendor/freetype"

project "FluidSimulation"
	location "FluidSimulation"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "fspch.h"
	pchsource "FluidSimulation/src/fspch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.frag",
		"%{prj.name}/src/**.vert",
		"%{prj.name}/src/**.geom",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.bullet3}",
		"%{IncludeDir.Bullet3Common}",
		"%{IncludeDir.Bullet3Collision}",
		"%{IncludeDir.BulletCollision}",
		"%{IncludeDir.Bullet3Dynamics}",
		"%{IncludeDir.BulletDynamics}",
		"%{IncludeDir.LinearMath}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.FreeType}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"Assimp",
		"FreeType",
		"opengl32.lib",
		"Bullet3Collision",
		"BulletCollision",
		"Bullet3Dynamics",
		"BulletDynamics",
		"Bullet3Common",
		"LinearMath",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FS_PLATFORM_WINDOWS",
			"FS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines 
		{
			"FS_DEBUG",
			"FS_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FS_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "FS_DIST"
		optimize "on"
		runtime "Release"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleAPp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"FluidSimulation/vendor/spdlog/include",
		"FluidSimulation/vendor/",
		"FluidSimulation/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.bullet3}",
		"%{IncludeDir.Bullet3Common}",
		"%{IncludeDir.Bullet3Collision}",
		"%{IncludeDir.BulletCollision}",
		"%{IncludeDir.Bullet3Dynamics}",
		"%{IncludeDir.BulletDynamics}",
		"%{IncludeDir.LinearMath}",
	}

	links
	{
		"FluidSimulation",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "FS_DEBUG"
		symbols "on"
		runtime "Debug"



	filter "configurations:Release"
		defines "FS_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "FS_DIST"
		optimize "on"
		runtime "Release"
