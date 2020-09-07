#include "pch.h"
#include "Lights.h"

namespace fe
{
	void Lights::Update(Scene& scene)
	{
		int idx = 0;
		for (int i = 0; i < scene.gameObjects.size() && idx < maxPointLightCount; i++)
		{
			GameObject* go = scene.gameObjects[i];
			ProcessGameObject(go, idx);

			for (size_t child_idx = 0; child_idx < go->children.size(); child_idx++)
				ProcessGameObject(go->children[child_idx], idx);
		}

		lightsCountData.pointLightCount = idx;
	}

	void Lights::ProcessGameObject(GameObject* go, int& idx)
	{
		Light& light = go->GetComponent<Light>();
		if (&light == nullptr)
			return;

		pointLightData[idx].ambient = light.GetAmbient();
		pointLightData[idx].diffuse = light.GetDiffuse();
		pointLightData[idx].specular = light.GetSpecular();

		Transform& transform = go->GetComponent<Transform>();
		pointLightData[idx].position = sm::Vector4(transform.GetPosition());

		idx++;
	}
}
