#include "AgPCH.h"
#include "Shader.h"
#include "RendererContext.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Utils {

	static shaderc_shader_kind GLShaderStageToShaderC(VkShaderStageFlagBits InStage)
	{
		switch (InStage)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:   return shaderc_glsl_vertex_shader;
		case VK_SHADER_STAGE_FRAGMENT_BIT: return shaderc_glsl_fragment_shader;
		}
		AG_ASSERT(false);
		return (shaderc_shader_kind)0;
	}

	static const char* ShaderStageToString(VkShaderStageFlagBits InStage)
	{
		switch (InStage)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:   return "VK_SHADER_STAGE_VERTEX_BIT";
		case VK_SHADER_STAGE_FRAGMENT_BIT: return "VK_SHADER_STAGE_FRAGMENT_BIT";
		}
		AG_ASSERT(false);
		return nullptr;
	}

	static const char* GetCacheDirectory()
	{
		return "Assets/Cache/Shaders";
	}

	static void CreateCacheDirectoryIfNeeded()
	{
		std::string path = GetCacheDirectory();
		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);
	}

	static const char* ShaderStageCachedVulkanFileExtension(VkShaderStageFlagBits InStage)
	{
		switch (InStage)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:    return ".cached_vulkan.vert";
		case VK_SHADER_STAGE_FRAGMENT_BIT:  return ".cached_vulkan.frag";
		}
		AG_ASSERT(false);
		return "";
	}

}

namespace Silver {

	// TODOs(Milan): 
	//	Shaders will take in only a single file containing every shader source code
	//	Should detect if the shaders have changed so that we compile them (otherwise it will used the cached binaries)
	//	Expand shader reflection

	Shader::Shader(std::string_view InVertex, std::string_view InFragment)
	{
		Utils::CreateCacheDirectoryIfNeeded();

		m_VertexFilepath = InVertex;
		m_FragmentFilepath = InFragment;

		std::unordered_map<VkShaderStageFlagBits, std::string> sources;
		sources[VK_SHADER_STAGE_VERTEX_BIT] = ReadFileAsString(InVertex);
		sources[VK_SHADER_STAGE_FRAGMENT_BIT] = ReadFileAsString(InFragment);

		CompileOrGetVulkanBinaries(sources);

		CreateShaderModule(VK_SHADER_STAGE_VERTEX_BIT);
		CreateShaderModule(VK_SHADER_STAGE_FRAGMENT_BIT);
	}

	Shader::~Shader()
	{
		vkDestroyShaderModule(RendererContext::Get().GetDevice(), m_Shaders[VK_SHADER_STAGE_VERTEX_BIT], nullptr);
		vkDestroyShaderModule(RendererContext::Get().GetDevice(), m_Shaders[VK_SHADER_STAGE_FRAGMENT_BIT], nullptr);
	}

	std::vector<char> Shader::ReadFileAsBinary(std::string_view InFilepath)
	{
		std::ifstream stream(InFilepath.data(), std::ios::ate | std::ios::binary);

		if (!stream.is_open())
			AG_ASSERT("Failed to open file!");

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		// File is empty
		if (size == 0)
			return {};

		std::vector<char> buffer(size);
		stream.read(buffer.data(), size);

		stream.close();

		AG_CORE_TRACE("Buffer size: {0}", size);
		AG_CORE_TRACE("Successfully read file: {0}", InFilepath);
		return buffer;
	}

	std::string Shader::ReadFileAsString(std::string_view InFilepath)
	{
		std::string result;
		std::ifstream in(InFilepath.data(), std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				AG_CORE_ERROR("Could not read from file '{0}'", InFilepath);
			}
		}
		else
			AG_CORE_ERROR("Could not open file '{0}'", InFilepath);

		return result;
	}

	void Shader::CompileOrGetVulkanBinaries(const std::unordered_map<VkShaderStageFlagBits, std::string>& InShaderSources)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : InShaderSources)
		{
			std::filesystem::path shaderFilePath;
			if (stage == VK_SHADER_STAGE_VERTEX_BIT)
				shaderFilePath = m_VertexFilepath;
			else
				shaderFilePath = m_FragmentFilepath;

			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + Utils::ShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			// If the SPIR-V binary is there, we will just read it
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				// We need to compile the shader code
				std::string filename;
				if (stage == VK_SHADER_STAGE_VERTEX_BIT)
					filename = m_VertexFilepath;
				else
					filename = m_FragmentFilepath;
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), filename.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					AG_CORE_ERROR(module.GetErrorMessage());
					AG_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				// Write the SPIR-V binary into the cached directory
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void Shader::CreateShaderModule(VkShaderStageFlagBits InStage)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = m_VulkanSPIRV[InStage].size() * sizeof(uint32_t);
		// createInfo.pCode = m_VulkanSPIRV[VK_SHADER_STAGE_VERTEX_BIT].data();
		createInfo.pCode = m_VulkanSPIRV[InStage].data();
		VkDevice device = RendererContext::Get().GetDevice();
		VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &m_Shaders[InStage]);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create vertex shader module!");
	}

	void Shader::Reflect(VkShaderStageFlagBits InStage, const std::vector<uint32_t>& InData)
	{
		spirv_cross::Compiler compiler(InData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		std::string filename;
		if (InStage == VK_SHADER_STAGE_VERTEX_BIT)
			filename = m_VertexFilepath;
		else
			filename = m_FragmentFilepath;

		AG_CORE_TRACE("Shader::Reflect - {0} {1}", Utils::ShaderStageToString(InStage), filename);
		AG_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		AG_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		AG_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = (uint32_t)compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = (int)bufferType.member_types.size();

			AG_CORE_TRACE("  {0}", resource.name);
			AG_CORE_TRACE("    Size = {0}", bufferSize);
			AG_CORE_TRACE("    Binding = {0}", binding);
			AG_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

}

