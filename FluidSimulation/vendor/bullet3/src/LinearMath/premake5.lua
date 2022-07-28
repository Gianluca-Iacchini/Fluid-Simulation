project "LinearMath"
	kind "StaticLib"
	language "C++"
	staticruntime "on"			

	targetdir ("../../bin/" .. outputdir .. "/bullet3")
    objdir ("../../bin-int/" .. outputdir .. "/bullet3")

	files {
		"**.cpp",
		"**.h"
	}

	includedirs {
		".."
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		runtime "Release"
        optimize "on"