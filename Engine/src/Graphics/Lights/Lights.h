#pragma once
#include "Graphics/Components/Light.h"
#include "Graphics/Buffers/ConstantBufferTypes.h"
#include "Scene/Scene.h"

namespace fe
{
	class Lights
	{
	public:
		static const int maxPointLightCount = 5;

		Lights() { }

		CB_PS_pointLight* GetPointLightsData() { return pointLightData; }
		CB_PS_lightsCount& GetLightsCountData() { return lightsCountData; }

		void Update(Scene& scene);

	private:
		CB_PS_pointLight pointLightData[Lights::maxPointLightCount];
		CB_PS_lightsCount lightsCountData;

		void ProcessGameObject(GameObject* go, int& idx);
	};
}
