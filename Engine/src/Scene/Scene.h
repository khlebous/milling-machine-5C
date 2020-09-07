#pragma once
#include "Graphics/Camera3D.h"
#include "Graphics/GameObjects/GameObject.h"

namespace fe
{
	class Scene
	{
	public:
		Scene()
		{
			gameObjects = std::vector<GameObject*>();
		}
		~Scene();

		Camera3D camera;

		void OnStart();
		void OnUpdate();

		std::vector<GameObject*> GetActiveGameObjects();
		std::vector<GameObject*> gameObjects;

	private:
		void TryAddActiveGO(GameObject* go, std::vector<GameObject*>& activeGOs);
	};
}
