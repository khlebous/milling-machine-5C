#pragma once
#include "Scene/Scene.h"
#include "Graphics/Device.h"

namespace fe
{
	class Renderer
	{
	public:
		void Initialize(Scene* scene, Device* device);
		void Render();

	private:
		Scene* scene;
		Device* device;

		ConstantBuffer<CB_VS_mvpMatrices> cb_vs_mvpMatrices;
		ConstantBuffer<CB_PS_material> cb_ps_material;
		CB_VS_mvpMatrices cb_vs_mvpMatricesData;
		CB_PS_material cb_ps_matrialData;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

		void RenderGameObject(GameObject* gameObject);
	};
}
