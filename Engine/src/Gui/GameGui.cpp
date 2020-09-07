#include "pch.h"
#include "GameGui.h"

namespace fe
{
	GameGui::GameGui() 
		: 
		IGuiWindow("Viewer", true), 
		gameTexture(nullptr), 
		gameTextureId((ImTextureID)-1)
	{
		this->gameFocused = false;
		this->gameWindowSizeChanged = true;
		this->gameTextureSize = ImVec2(1, 1);
	}

	void GameGui::SetGameTexture(Texture* _gameTexture)
	{
		this->gameTexture = _gameTexture;
		this->gameTextureId = (ImTextureID)_gameTexture->GetTextureResourceView();
	}

	void GameGui::Render()
	{
		if (open)
		{
			ImGui::Begin("Viewer", &open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			ImVec2 window_pos = ImGui::GetWindowPos();
			this->gameFocused = ImGui::IsWindowFocused() ? true : false;

			ImVec2 newGameWindowSize = ImGui::GetContentRegionAvail();
			newGameWindowSize.x = std::max(newGameWindowSize.x, minTextureSize.x);
			newGameWindowSize.y = std::max(newGameWindowSize.y, minTextureSize.y);
			if (gameTextureSize.x != newGameWindowSize.x || gameTextureSize.y != newGameWindowSize.y)
			{
				gameTextureSize = newGameWindowSize;
				this->gameWindowSizeChanged = true;
			}

			ImGui::Image(gameTextureId, gameTextureSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			ImGui::End();
		}
	}
}
