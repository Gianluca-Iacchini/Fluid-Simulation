project "Assimp"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "assimp/*.cpp",
        "assimp/*.h",
        "assimp/*.hpp",
        "assimp/*.inl"
    }

    includedirs
    {
        "assimp",
        "assimp/Compiler"
    }