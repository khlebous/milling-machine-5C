#pragma once
#include "ImGui/imgui.h"
#include "Graphics/Texture.h"
#include "IGuiWindow.h"

namespace fe
{
	class GameGui : public IGuiWindow
	{
	public:
		GameGui();

		void SetGameTexture(Texture* game_texture);
		void Render() override;

		ImVec2 gameTextureSize;
		bool gameWindowSizeChanged;
		bool gameFocused;

	private:
		Texture* gameTexture;
		ImTextureID gameTextureId;
		
		const ImVec2 minTextureSize = ImVec2(100, 100);
	};
}
