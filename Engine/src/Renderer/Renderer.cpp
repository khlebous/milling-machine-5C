#include "pch.h"
#include "Renderer.h"
#include "Graphics/Components/MeshRenderer.h"
#include "Graphics/Components/ShaderConfiguration.h"
#include "Constants/ShaderResourcesConstants.h"
#include "Constants/CBConstants.h"

namespace fe
{
	void Renderer::Initialize(Scene* scene, Device* device)
	{
		this->scene = scene;
		this->device = device;

		this->cb_vs_mvpMatrices.Initialize(device->get(), device->context());
		this->cb_ps_material.Initialize(device->get(), device->context());
	}

	void Renderer::Render()
	{
		device->context()->VSSetConstantBuffers(CbVsIdxs::MvpMatricesIdx, 1, this->cb_vs_mvpMatrices.GetAddressOf());
		device->context()->PSSetConstantBuffers(CbPsIdxs::MaterialIdx, 1, this->cb_ps_material.GetAddressOf());

		cb_vs_mvpMatricesData.viewMatrix = scene->camera.GetViewMatrix();
		cb_vs_mvpMatricesData.invViewMatrix = XMMatrixInverse(nullptr, scene->camera.GetViewMatrix());
		cb_vs_mvpMatricesData.projMatrix = scene->camera.GetProjectionMatrix();

		for (auto& go : this->scene->GetActiveGameObjects())
			RenderGameObject(go);

		//cleaning
		device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Renderer::RenderGameObject(GameObject* gameObject)
	{
		MeshRenderer& mr = gameObject->GetComponent<MeshRenderer>();
		if (&mr)
		{
			Transform& tr = gameObject->GetComponent<Transform>();
			cb_vs_mvpMatricesData.modelMatrix = &tr ? tr.GetModelMatrix() : sm::Matrix::Identity;
			cb_vs_mvpMatrices.Update(&cb_vs_mvpMatricesData);

			for (size_t mesh_idx = 0; mesh_idx < mr.GetMeshesSize(); mesh_idx++)
			{
				RenderableMesh& rm = mr.GetMesh(mesh_idx);
				device->context()->IASetInputLayout(rm.vertexShader->GetInputLayout());

				device->context()->IASetPrimitiveTopology(rm.vertexShader->topology);
				device->context()->VSSetShader(rm.vertexShader->GetShader(), NULL, 0);
				device->context()->PSSetShader(rm.pixelShader->GetShader(), NULL, 0);

				if (rm.pixelShader->AlbedoTexture != nullptr)
					device->context()->PSSetShaderResources(SR_TextureIdxs::Albedo, 1, rm.pixelShader->AlbedoTexture->GetTextureResourceViewAddress());
				if (rm.pixelShader->MetalnessTexture != nullptr)
					device->context()->PSSetShaderResources(SR_TextureIdxs::Roughness, 1, rm.pixelShader->RoughnessTexture->GetTextureResourceViewAddress());
				if (rm.pixelShader->MetalnessTexture != nullptr)
					device->context()->PSSetShaderResources(SR_TextureIdxs::Metalness, 1, rm.pixelShader->MetalnessTexture->GetTextureResourceViewAddress());

				Material* material = rm.material;
				if (material != nullptr)
				{
					cb_ps_matrialData.ambient = material->ambient;
					cb_ps_matrialData.diffuse = material->diffuse;
					cb_ps_matrialData.specular = material->specular;
					cb_ps_matrialData.m = material->specularExponent;
				}
				cb_ps_material.Update(&cb_ps_matrialData);

				rm.mesh->Draw();
			}
		}
	}
}
