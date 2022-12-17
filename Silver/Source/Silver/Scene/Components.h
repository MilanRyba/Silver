#pragma once
#include <glm/glm.hpp>
#include <string>

namespace Silver {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string_view inTag) : Tag(inTag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& inTransform) : Transform(inTransform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

}