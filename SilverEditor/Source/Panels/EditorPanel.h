#pragma once
#include "Silver/Core/Reference.h"

#include <string>

class EditorPanel : public Silver::RefTarget
{
public:
	virtual ~EditorPanel() = default;
private:
	uint32_t m_PanelID;

	std::string m_Title = "Silver Editor Panel";

	uint32_t m_Width;
	uint32_t m_Height;

	bool m_IsFocused = false;
	bool m_IsHovered = false;
};