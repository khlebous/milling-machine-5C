#pragma once
#include "WindowContainer.h"
#include "Time/Timer.h"
#include "Graphics/Resources/ResourceCollection.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"
#include "Layers/Layer.h"
#include "Gui/Gui.h"

namespace fe
{
	class Engine : public WindowContainer
	{
	public:
		~Engine();

		virtual bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class,
			int width, int height);
		void Start();
		bool ProcessMessages();
		void Update();
		void RenderFrame();

		void AddLayer(Layer* layer) { layers.push_back(layer); }

	protected:
		Timer timer;
		ResourceCollection resources;
		Lights lights;
		Scene scene;
		Renderer renderer;

		Gui gui;
		GameGui* gameGui;
		
		void UpdateTime();

	private:
		std::vector<Layer*> layers;

		void LoadShaders();
		void WindowResized() override;
	};
}
