project "imgui"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    systemversion "latest"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("build/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.cpp",
        "imgui.h",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "backends/imgui_impl_glfw.cpp",
        "backends/imgui_impl_opengl3.cpp"
    }

    includedirs
    {
        ".",
        "../glfw/include"
    }

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
    
