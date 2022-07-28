project "bullet3"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    files 
    {
        "src/*.cpp",
        "src/*.h",
    }

    includedirs
    {
        "src",
        "."
    }

    include "src/Bullet3Collision"
    include "src/Bullet3Common"
    include "src/Bullet3Dynamics"
    include "src/LinearMath"