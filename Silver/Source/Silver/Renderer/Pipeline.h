#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

#include "Shader.h"
#include "RenderPass.h"
#include "Buffer.h"

namespace Silver {

	enum class PrimitiveTopology
	{
		None = 0,
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan
	};

	enum class DepthCompareOperator
	{
		None = 0,
		GreaterOrEqual
	};

	struct PipelineInfo
	{
		Ref<Shader> Shader;
		Ref<RenderPass> RenderPass;
		VertexBufferLayout Layout;
		PrimitiveTopology Topology = PrimitiveTopology::Triangles;
		DepthCompareOperator DepthOperator = DepthCompareOperator::GreaterOrEqual;
		bool BackfaceCulling = true;
		bool DepthTest = true;
		bool DepthWrite = true;
		bool Wireframe = false;
		float LineWidth = 1.0f;

		std::string DebugName;
	};

	class Pipeline : public RefTarget
	{
	public:
		Pipeline(PipelineInfo inInfo);
		~Pipeline();

		VkPipeline GetPipeline() const { return m_Pipeline; }
		PipelineInfo GetInfo() const { return m_Info; }
	private:
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_Layout = VK_NULL_HANDLE;

		PipelineInfo m_Info;
	};

}
