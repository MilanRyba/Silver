#pragma once
#include "Silver/Renderer/Shader.h"

#include <vulkan/vulkan.h>

namespace Silver {

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(std::string_view InVertex, std::string_view InFragment);
	private:
		std::vector<char> ReadFileAsBinary(std::string_view InFilepath);
		std::string ReadFileAsString(std::string_view InFilepath);

		void CompileOrGetVulkanBinaries(const std::unordered_map<VkShaderStageFlagBits, std::string>& InShaderSources);
		void CreateShaderModule(VkShaderStageFlagBits InStage);

		void Reflect(VkShaderStageFlagBits InStage, const std::vector<uint32_t>& InData);
	private:
		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_Shaders;
		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> m_VulkanSPIRV;

		std::string m_VertexFilepath;
		std::string m_FragmentFilepath;
		std::string m_Name;
	};

}