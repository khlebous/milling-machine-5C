#include "pch.h"
#include "MeshRenderer.h"

namespace fe
{
	CLASS_DEFINITION(Component, MeshRenderer)

	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material, VertexShader* vs, PixelShader* ps)
	{
		renderableMesh.emplace_back(mesh, material, vs, ps);
	}

	MeshRenderer::MeshRenderer(Model* model, VertexShader* vs, PixelShader* ps)
	{
		for (size_t i = 0; i < model->GetMeshesSize(); i++)
		{
			renderableMesh.emplace_back(model->GetMesh(i), model->GetMaterial(i), vs, ps);
		}
	}
}
