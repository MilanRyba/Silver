VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDirs = {}
IncludeDirs["GLFW"] = "%{wks.location}/ThirdParty/GLFW/include"
IncludeDirs["spdlog"] = "%{wks.location}/ThirdParty/spdlog/include"
IncludeDirs["glm"] = "%{wks.location}/ThirdParty/glm"
IncludeDirs["Vulkan"] = "%{VULKAN_SDK}/Include"
-- IncludeDirs["shaderc"] = "%{wks.location}/Hazel/vendor/shaderc/include"
-- IncludeDirs["SPIRV_Cross"] = "%{wks.location}/Hazel/vendor/SPIRV-Cross"
IncludeDirs["shaderc"] = "%{VULKAN_SDK}/Include"
IncludeDirs["SPIRV_Cross"] = "%{VULKAN_SDK}/Include"
IncludeDirs["ImGui"] = "%{wks.location}/ThirdParty/ImGui"
IncludeDirs["EnTT"] = "%{wks.location}/ThirdParty/EnTT/include"

LibraryDirs = {}
LibraryDirs["Vulkan"] = "%{VULKAN_SDK}/Lib"

Libraries = {}
Libraries["Vulkan"] = "%{LibraryDirs.Vulkan}/vulkan-1.lib"
Libraries["VulkanUtils"] = "%{LibraryDirs.Vulkan}/VkLayer_utils.lib"

Libraries["ShaderC_Debug"] = "%{LibraryDirs.Vulkan}/shaderc_sharedd.lib"
Libraries["SPIRV_Cross_Debug"] = "%{LibraryDirs.Vulkan}/spirv-cross-cored.lib"
Libraries["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDirs.Vulkan}/spirv-cross-glsld.lib"
Libraries["SPIRV_Tools_Debug"] = "%{LibraryDirs.Vulkan}/SPIRV-Toolsd.lib"

Libraries["ShaderC_Release"] = "%{LibraryDirs.Vulkan}/shaderc_shared.lib"
Libraries["SPIRV_Cross_Release"] = "%{LibraryDirs.Vulkan}/spirv-cross-core.lib"
Libraries["SPIRV_Cross_GLSL_Release"] = "%{LibraryDirs.Vulkan}/spirv-cross-glsl.lib"