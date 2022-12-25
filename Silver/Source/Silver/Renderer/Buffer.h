#pragma once
#include "Silver/Core/Reference.h"

#include <vulkan/vulkan.h>
#include "Vma/vk_mem_alloc.h"

#include <vector>
#include <string>

namespace Silver {

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Uint, Uint2, Uint3, Uint4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType inType)
	{
		using enum ShaderDataType;
		switch (inType)
		{
			case Float:		return 4;
			case Float2:	return 8;
			case Float3:	return 12;
			case Float4:	return 16;
			case Int:		return 4;
			case Int2:		return 8;
			case Int3:		return 12;
			case Int4:		return 16;
			case Uint:		return 4;
			case Uint2:		return 8;
			case Uint3:		return 12;
			case Uint4:		return 16;
		}

		AG_ASSERT("Specified incorrect shader data type!");
		return 0;
	}

	struct VertexAttribute
	{
		// Had the constructors for VertexAttribute and VertexBufferLayout deleted but we need them, because they are called when PipelineInfo is created
		VertexAttribute() = default;

		VertexAttribute(ShaderDataType inType, std::string_view inName)
			: Type(inType), Name(inName), Size(ShaderDataTypeSize(inType))  {}

		ShaderDataType Type = ShaderDataType::None;
		std::string Name = "";
		uint32_t Size = 0;
		uint32_t Offset = 0;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;
		VertexBufferLayout(const std::initializer_list<VertexAttribute>& inAttributes) : m_Attributes(inAttributes) { CalculateOffsetAndStride(); }

		inline std::vector<VertexAttribute> GetAttributes() const		{ return m_Attributes; }
		inline uint32_t	GetStride() const								{ return m_Stride; }

		std::vector<VertexAttribute>::iterator begin() { return m_Attributes.begin(); }
		std::vector<VertexAttribute>::iterator end() { return m_Attributes.end(); }
		std::vector<VertexAttribute>::const_iterator begin() const { return m_Attributes.begin(); }
		std::vector<VertexAttribute>::const_iterator end() const { return m_Attributes.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for (auto& attribute : m_Attributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}

	private:
		std::vector<VertexAttribute> m_Attributes;
		uint32_t m_Stride = 0;

	};

	// TEMP
	struct Vertex
	{
		glm::vec2 Position;
		glm::vec3 Color;
	};

	struct VertexBufferInfo
	{
		size_t Size = 0;
		void* Data = nullptr;
	};

	class VertexBuffer : public RefTarget
	{
	public:
		VertexBuffer(VertexBufferInfo inInfo);
		~VertexBuffer();

		void Bind(VkCommandBuffer inCommandBuffer);

	private:
		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VmaAllocation m_Allocation;
	};

	struct IndexBufferInfo
	{
		size_t Size = 0;
		void* Data = nullptr;
	};

	class IndexBuffer : public RefTarget
	{
	public:
		IndexBuffer(IndexBufferInfo inInfo);
		~IndexBuffer();

		void Bind(VkCommandBuffer inCommandBuffer);

	private:
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
	};

}