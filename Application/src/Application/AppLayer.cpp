#include "AppLayer.h"

#include <math.h>
#include <time.h>
#include <numeric>

#include <Graphics/Components/MeshRenderer.h>
#include <Graphics/Lights/Lights.h>
#include <Graphics/InstancedMesh.h>
#include <ImGui/imgui.h>
#include <Time/Time.h>

#include "Components/Surface.h"
#include "Components/CutterMovement.h"
#include "Components/Cutters/SphericalCutter.h"
#include "Components/Cutters/ToroidalCutter.h"

namespace sm = DirectX::SimpleMath;

bool AppLayer::Initialize(fe::ResourceCollection* resources, fe::Scene& scene, fe::Input* input)
{
	fe::GameObject* surfaceObj = CreateSurface(resources, scene);

	fe::GameObject* materialObj = CreateMaterial(resources, scene);

	fe::GameObject* millingObj = CreateCutter(resources, scene);
	millingObj->AddComponent<CutterMovement>(&surfaceObj->GetComponent<Surface>());
	millingObj->GetComponent<CutterMovement>().instancedMesh = static_cast<fe::InstancedMesh*>(materialObj->GetComponent<fe::MeshRenderer>().GetMesh(0).mesh);

	this->scene = &scene;
	this->input = input;

	CreateLights(resources, &scene);

	return true;
}

fe::GameObject* AppLayer::CreateMaterial(fe::ResourceCollection* resources, fe::Scene& scene)
{
	fe::GameObject* materialGO = new fe::GameObject("Material");
	materialGO->AddComponent<fe::Transform>();

	resources->Models.AddFromFile("Cube.obj", "CubeI", true);
	fe::Model* newModel = new fe::Model(resources->Models.Get("CubeI"));
	fe::Material* newMaterial = new fe::Material();
	newModel->SetMaterial(0, newMaterial);

	materialGO->AddComponent<fe::MeshRenderer>(newModel, resources->Shaders.GetVS("MaterialShader"), resources->Shaders.GetPS("MaterialShader"));
	scene.gameObjects.push_back(materialGO);

	return materialGO;
}

fe::GameObject* AppLayer::CreateCutter(fe::ResourceCollection* resources, fe::Scene& scene)
{
	fe::GameObject* millingObj = new fe::GameObject("Cutter");
	scene.gameObjects.push_back(millingObj);

	float minorRadius = 1.0f;
	float radius = 3.0f;
	int horizontalLvls = 10;
	int roundLvls = 10;
	float height = 40;

	if (false)
	{
		millingObj->AddComponent<SphericalCutter>(radius, horizontalLvls, roundLvls, height);

		SphericalCutter& mm = millingObj->GetComponent<SphericalCutter>();
		resources->Meshes.AddFromData(mm.vertices, mm.indices, "bb");
	}
	else
	{
		millingObj->AddComponent<ToroidalCutter>(radius, minorRadius, horizontalLvls, roundLvls, height);

		ToroidalCutter& mm = millingObj->GetComponent<ToroidalCutter>();
		resources->Meshes.AddFromData(mm.vertices, mm.indices, "bb");
	}

	fe::Mesh* bb = resources->Meshes.Get("bb");
	fe::Material* material = new fe::Material();
	millingObj->AddComponent<fe::MeshRenderer>(bb, material, resources->Shaders.GetVS("CutterShader"), resources->Shaders.GetPS("CutterShader"));
	millingObj->AddComponent<fe::Transform>(true);
	fe::Transform& cut_tr = millingObj->GetComponent<fe::Transform>();

	return millingObj;
}

fe::GameObject* AppLayer::CreateSurface(fe::ResourceCollection* resources, fe::Scene& scene)
{
	fe::GameObject* surfaceObj = new fe::GameObject("Surface Preview");
	scene.gameObjects.push_back(surfaceObj);

	surfaceObj->AddComponent<Surface>();

	Surface& surface = surfaceObj->GetComponent<Surface>();
	auto points = surface.GetPoints();
	std::vector<DWORD> indices(points.size());
	std::iota(std::begin(indices), std::end(indices), 0);

	resources->Meshes.AddFromData(points, indices, "SurfaceMesh");
	fe::Mesh* aaa = resources->Meshes.Get("SurfaceMesh");
	fe::Material* material = new fe::Material();
	surfaceObj->AddComponent<fe::MeshRenderer>(aaa, material, resources->Shaders.GetVS("SurfaceShader"), resources->Shaders.GetPS("SurfaceShader"));

	return surfaceObj;
}

void AppLayer::CreateLights(fe::ResourceCollection* resources, fe::Scene* scene)
{
	resources->Models.AddFromFile("Cube.obj", "Cube");
	fe::GameObject* lights = new fe::GameObject("Lights");

	lights->AddChild(CreateLight(0, sm::Vector3(-0, 40.0f, -0), sm::Color(255.f / 255.f, 255.f / 255.f, 255.f / 255.f), resources));
	//lights->AddChild(CreateLight(1, sm::Vector3(-0, 10, -0), sm::Color(255.f / 255.f, 255.f / 255.f, 255.f / 255.f), resources));
	//lights->AddChild(CreateLight(2, sm::Vector3(10, 10, 0), sm::Color(255.f / 255.f, 255.f / 255.f, 255.f / 255.f), resources));

	scene->gameObjects.push_back(lights);
}

fe::GameObject* AppLayer::CreateLight(int idx, sm::Vector3 position, sm::Color intensity, fe::ResourceCollection* resources)
{
	fe::GameObject* go = new fe::GameObject("Light" + std::to_string(idx));

	fe::Transform transform;
	transform.SetPosition(position);
	transform.SetScale(0.2f, 0.2f, 0.2f);
	go->AddComponent<fe::Transform>(transform);

	fe::Light light(fe::LightType::Point);
	light.SetAmbient(intensity);
	light.SetDiffuse(intensity);
	light.SetSpecular(intensity);
	go->AddComponent<fe::Light>(light);

	fe::Model* newModel = new fe::Model(resources->Models.Get("Cube"));
	fe::Material* newMaterial = new fe::Material();
	newModel->SetMaterial(0, newMaterial);
	go->AddComponent<fe::MeshRenderer>(newModel, resources->Shaders.GetVS("LampShader"), resources->Shaders.GetPS("LampShader"));

	return go;
}

void AppLayer::ProcssMouseInput()
{
	while (!input->mouse.EventBufferIsEmpty())
	{
		fe::MouseEvent me = input->mouse.ReadEvent();
		if (input->mouse.IsRightDown())
		{
			if (me.GetType() == fe::MouseEvent::EventType::RAW_MOVE)
			{
				scene->camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}
}

void AppLayer::ProcssKeyboardInput()
{
	float cameraSpeed = input->keyboard.KeyIsPressed(VK_SHIFT) ? 0.06f : 0.006f;
	float t = fe::Time::deltaTime / 1000;

	while (!input->keyboard.KeyBufferIsEmpty())
	{
		fe::KeyboardEvent ke = input->keyboard.ReadKey();
	}

	if (input->keyboard.KeyIsPressed('W'))
	{
		scene->camera.AdjustPosition(scene->camera.GetTransform().GetForward(true) * cameraSpeed * fe::Time::deltaTime);
	}
	if (input->keyboard.KeyIsPressed('S'))
	{
		scene->camera.AdjustPosition(scene->camera.GetTransform().GetBackward(true) * cameraSpeed * fe::Time::deltaTime);
	}
	if (input->keyboard.KeyIsPressed('A'))
	{
		scene->camera.AdjustPosition(scene->camera.GetTransform().GetLeft(true) * cameraSpeed * fe::Time::deltaTime);
	}
	if (input->keyboard.KeyIsPressed('D'))
	{
		scene->camera.AdjustPosition(scene->camera.GetTransform().GetRight(true) * cameraSpeed * fe::Time::deltaTime);

	}
	if (input->keyboard.KeyIsPressed('E'))
	{
		scene->camera.AdjustPosition(0.0f, cameraSpeed * fe::Time::deltaTime, 0.0f);
	}
	if (input->keyboard.KeyIsPressed('Q'))
	{
		scene->camera.AdjustPosition(0.0f, -cameraSpeed * fe::Time::deltaTime, 0.0f);
	}
}
