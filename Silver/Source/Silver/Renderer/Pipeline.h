#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

#include "Shader.h"
#include "RenderPass.h"

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
		Pipeline(PipelineInfo info);
		~Pipeline();
	private:
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_Layout = VK_NULL_HANDLE;
	};

}
