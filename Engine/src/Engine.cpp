#include "pch.h"
#include "Engine.h"
#include "Time/Time.h"

namespace fe
{
	Engine::~Engine()
	{
		for (int i = layers.size() - 1; i >=0; i--)
			delete layers[i];
	}

	bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
	{
		timer.Start();

		if (!this->renderWindow.Initialize(this, hInstance, window_title, window_class, width, height))
			return false;

		if (!graphics.Initialize(this->renderWindow.GetHWND(), this->windowWidth, this->windowHeight))
			return false;

		renderer.Initialize(&scene, &graphics.device);
		resources.Initialize(&graphics.device);

		std::string gameTextureName = "GameTexture";
		auto desc = Texture::GetRenderTexDesc(10, 10);
		Texture* gameTexture = resources.Textures.AddFromDesc(desc, gameTextureName);

		LoadShaders();

		scene.camera.SetPosition(0.0f, 10.0f, -30.0f);
		scene.camera.SetProjectionValues(45.0f, windowWidth, windowHeight, 0.1f, 1000.0f);

		return true;
	}

	void Engine::Start()
	{
		std::unique_ptr<InspectorGui> ig = std::make_unique<InspectorGui>();
		std::unique_ptr<HierarchyGui> hg = std::make_unique<HierarchyGui>();
		hg->Initialize(&scene, ig.get());

		std::unique_ptr<GameGui> gg = std::make_unique<GameGui>();
		gameGui = gg.get();

		gui.Initialize(this->renderWindow.GetHWND(), graphics.device);
		gui.SetLayers(&layers);

		gui.AddWindow(std::move(ig));
		gui.AddWindow(std::move(hg));
		gui.AddWindow(std::move(gg));

		scene.OnStart();
	}

	bool Engine::ProcessMessages()
	{
		return this->renderWindow.ProcessMessages();
	}

	void Engine::Update()
	{
		UpdateTime();

		if (gameGui->gameWindowSizeChanged)
		{
			gameGui->gameWindowSizeChanged = false;

			Texture* gameTexture = resources.Textures.Get("GameTexture");
			auto desc = Texture::GetRenderTexDesc( gameGui->gameTextureSize.x, gameGui->gameTextureSize.y);
			gameTexture->Initialize(graphics.device, desc);
			graphics.InitializeGameTexture(gameTexture);
			gameGui->SetGameTexture(gameTexture);

			scene.camera.SetWidthHeight(gameGui->gameTextureSize.x, gameGui->gameTextureSize.y);
			WindowResized();
		}

		if (gameGui->gameFocused)
		{
			for (auto& layer : layers)
			{
				layer->ProcssKeyboardInput();
				layer->ProcssMouseInput();
			}
		}

		lights.Update(scene);
		graphics.UpdateLights(lights);
		scene.OnUpdate();
		input.Update();
	}

	void Engine::UpdateTime()
	{
		Time::deltaTime = timer.GetMilisecondsElapsed();
		Time::timeSinceLevelLoad += Time::deltaTime;

		timer.Restart();
	}

	void Engine::LoadShaders()
	{
		resources.Shaders.Add(&graphics.cutterVertexShader, "CutterShader");
		resources.Shaders.Add(&graphics.cutterPixelShader, "CutterShader");
		resources.Shaders.Add(&graphics.lampVertexShader, "LampShader");
		resources.Shaders.Add(&graphics.lampPixelShader, "LampShader");
		resources.Shaders.Add(&graphics.surfaceVertexShader, "SurfaceShader");
		resources.Shaders.Add(&graphics.surfacePixelShader, "SurfaceShader");
		resources.Shaders.Add(&graphics.materialVS, "MaterialShader");
		resources.Shaders.Add(&graphics.materialPS, "MaterialShader");
	}

	void Engine::WindowResized()
	{
		float width = static_cast<float>(this->windowWidth);
		float height = static_cast<float>(this->windowHeight);

		graphics.OnResize(width, height);
	}

	void Engine::RenderFrame()
	{
		this->graphics.OnPreRender();
		this->renderer.Render();
		this->graphics.RestoreContext();
		this->gui.Render();
		this->graphics.OnPostRender();
	}
}
