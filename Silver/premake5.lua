project "Silver"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir (BinOutput .. "/%{prj.name}")
	objdir (IntOutput .. "/%{prj.name}")

	pchheader "AgPCH.h"
	pchsource "Source/AgPCH.cpp"

	files {
		"Source/**.cpp",
		"Source/**.h"
	}

	includedirs {
		"Source/",
        "Source/Silver/",
        "%{IncludeDirs.GLFW}"
        -- Vulkan
	}

	defines {
        "GLFW_INCLUDE_NONE"
    }

	filter "system:windows"
        systemversion "latest"

        -- links
        -- {
        --     "%{Libraries.Vulkan}"
        -- }

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