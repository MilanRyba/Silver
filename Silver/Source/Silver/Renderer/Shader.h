#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

#include <unordered_map>

namespace Silver {

	class Shader : public RefTarget
	{
	public:
		Shader(std::string_view InVertex, std::string_view InFragment);
		~Shader();

		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> GetShaderModules() const { return m_Shaders; };
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