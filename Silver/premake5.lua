project "Silver"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

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
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.Vulkan}",
        "%{IncludeDirs.EnTT}"
	}

	defines {
        "GLFW_INCLUDE_NONE",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
        "GLM_FORCE_RADIANS",
        "GLM_ENABLE_EXPERIMENTAL"
    }

	filter "system:windows"
        systemversion "latest"

        links {
            "%{Libraries.Vulkan}"
        }

    filter "configurations:Debug"
        defines "AG_DEBUG"
        runtime "Debug"
        symbols "On"
        conformancemode "On"

        links {
			"%{Libraries.ShaderC_Debug}",
			"%{Libraries.SPIRV_Cross_Debug}",
			"%{Libraries.SPIRV_Cross_GLSL_Debug}"
		}

    filter "configurations:Release"
        defines "AG_RELEASE"
        runtime "Release"
        optimize "On"
        conformancemode "On"

        links {
			"%{Libraries.ShaderC_Release}",
			"%{Libraries.SPIRV_Cross_Release}",
			"%{Libraries.SPIRV_Cross_GLSL_Release}"
		}

    filter "configurations:Distribution"
        defines "AG_DIST"
        runtime "Release"
        optimize "Full"
        conformancemode "On"

        links {
			"%{Libraries.ShaderC_Release}",
			"%{Libraries.SPIRV_Cross_Release}",
			"%{Libraries.SPIRV_Cross_GLSL_Release}"
		}