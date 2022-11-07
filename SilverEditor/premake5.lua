project "SilverEditor"
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
        "%{wks.location}/SilverEditor/Source/",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.ImGui}",
        "%{IncludeDirs.Vulkan}"
	}

    links {
        "Silver",
        "GLFW",
        "ImGui"
    }

	defines {
        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
        "GLM_FORCE_RADIANS",
        "GLM_ENABLE_EXPERIMENTAL"
    }

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