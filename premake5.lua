workspace "RenderEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "SeaTrials",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

Includes = {}
Includes["Simple_Logger"] = "W_Engine/lib/simple_logger/include"
Includes["GLFW"] = "W_Engine/lib/GLFW/include"
Includes["GLAD"] = "W_Engine/lib/GLAD/include"
Includes["imgui"] = "W_Engine/lib/imgui"
Includes["glm"] = "W_Engine/lib/glm"
Includes["assimp"] = "W_Engine/lib/assimp/include"

include "W_Engine/lib/GLFW"
include "W_Engine/lib/GLAD"
include "W_Engine/lib/imgui"

project "W_Engine"
    location "W_Engine"
    kind "StaticLib"
    language "C++"

    cppdialect "C++17"
    systemversion "latest"
	staticruntime "on"

    defines "IMGUI_IMPL_OPENGL_LOADER_CUSTOM"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("build/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h",
        "%{prj.name}/lib/glm/glm/**.hpp",
        "%{prj.name}/lib/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/include",
        "%{Includes.Simple_Logger}",
        "%{Includes.GLFW}",
        "%{Includes.GLAD}",
        "%{Includes.imgui}",
        "%{Includes.glm}",
        "%{Includes.assimp}"
    }

    links {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "dwmapi.lib",
        "imgui",
    }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "On"

        libdirs
        {
            "W_Engine/lib/Assimp/bin/Debug"
        }

        links 
        {
            "assimp-vc143-mtd.lib"
        }

    filter "configurations:SeaTrials"
        defines "SEA_TRIALS"
        runtime "Release"
        optimize "On"

        libdirs
        {
            "W_Engine/lib/Assimp/bin/Release"
        }

        links 
        {
            "assimp-vc143-mt.lib"
        }

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "On"

        libdirs
        {
            "W_Engine/lib/Assimp/bin/Release"
        }

        links 
        {
            "assimp-vc143-mt.lib"
        }

project "Program"
    location "Program"
    kind "ConsoleApp"

    language "C++"

    cppdialect "C++17"
    systemversion "latest"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("build/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "W_Engine/include",
        "%{Includes.Simple_Logger}",
        "%{Includes.glm}",
        "%{Includes.imgui}"
    }

    links 
    {
        "W_Engine"
    }

    postbuildcommands
    {
        "{COPY} ../resources ../bin/" .. outputdir .. "/%{prj.name}/resources"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
    filter "configurations:SeaTrials"
        runtime "Release"
        optimize "On"
    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    
