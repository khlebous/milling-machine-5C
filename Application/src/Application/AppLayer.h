#pragma once
#include <Graphics/Resources/ResourceCollection.h>
#include <Layers/Layer.h>
#include <Input/Input.h>
#include <Scene/Scene.h>

class AppLayer : public fe::Layer
{
public:
	AppLayer() : fe::Layer("game layer") { }

	bool Initialize(fe::ResourceCollection* resources, fe::Scene& scene, fe::Input* input);

	void ProcssMouseInput() override;
	void ProcssKeyboardInput() override;

private:
	fe::Scene* scene;
	fe::Input* input;

	fe::GameObject* CreateMaterial(fe::ResourceCollection* resources, fe::Scene& scene);
	fe::GameObject* CreateCutter(fe::ResourceCollection* resources, fe::Scene& scene);
	fe::GameObject* CreateSurface(fe::ResourceCollection* resources, fe::Scene& scene);
	void CreateLights(fe::ResourceCollection* resources, fe::Scene* scene);
	fe::GameObject* CreateLight(int idx, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Color intensity,
		fe::ResourceCollection* resources);
};
