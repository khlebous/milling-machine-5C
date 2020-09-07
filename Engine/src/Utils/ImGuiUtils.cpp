#include "pch.h"
#include "ImGuiUtils.h"

namespace fe
{
	void ImGuiUtils::PushDisabledWithAlpha(float alpha)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * alpha);
	}

	void ImGuiUtils::PopDisabled()
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	void ImGuiUtils::HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}