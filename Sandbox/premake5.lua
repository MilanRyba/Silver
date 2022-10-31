project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
    staticruntime "off"

	targetdir (BinOutput .. "/%{prj.name}")
	objdir (IntOutput .. "/%{prj.name}")

	files {
		"Source/**.cpp",
		"Source/**.h"
	}

	includedirs {
		"%{wks.location}/Silver/Source/",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.spdlog}"
	}

    links {
        "Silver",
        "GLFW"
    }

	defines {}

	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "AG_DEBUG"
        runtime "Debug"
        symbols "On"
        conformancemode "On"

    filter "configurations:Release"
        defines "AG_RELEASE"
        runtime "Release"
        optimize "On"
        conformancemode "On"

    filter "configurations:Distribution"
        defines "AG_DIST"
        runtime "Release"
        optimize "Full"
        conformancemode "On"