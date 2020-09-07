#pragma once

namespace fe
{
	class ImGuiUtils
	{
	public:
		static void PushDisabledWithAlpha(float alpha = 0.5f);
		static void PopDisabled();
		static void HelpMarker(const char* desc);
	};
}
