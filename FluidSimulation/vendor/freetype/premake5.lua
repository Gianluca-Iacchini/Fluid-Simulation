project "FreeType"
    kind "SharedLib"
    language "C"
    staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "**.h",
        "**.c",
        "**.hpp",
        "**.cpp",
    }

    includedirs
    {
        "..",
    }

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"