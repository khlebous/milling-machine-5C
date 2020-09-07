#pragma once
#include "Graphics/Device.h"
#include "Layers/Layer.h"
#include "GameGui.h"
#include "HierarchyGui.h"

namespace fe
{
	class Gui
	{
	public:
		~Gui();

		void Initialize(HWND hwnd, Device& device);
		void AddWindow(std::unique_ptr<IGuiWindow> w);
		void SetLayers(std::vector<Layer*>* layers) { this->layers = layers; }
		void Render();

		std::vector<std::unique_ptr<IGuiWindow>> windows;
		
	private:
		bool* open = NULL;

		void DockSpace(bool* open);
		std::vector<Layer*>* layers;
	};
}
