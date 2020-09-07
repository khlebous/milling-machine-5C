#pragma once
#include "Component.h"
#include "Graphics/RenderableMesh.h"

namespace fe
{
	class MeshRenderer : public Component
	{
		CLASS_DECLARATION(MeshRenderer)

	public:
		MeshRenderer(Mesh* mesh, Material* material, VertexShader* vs, PixelShader* ps);
		MeshRenderer(Model* model, VertexShader* vs, PixelShader* ps);

		size_t GetMeshesSize() { return renderableMesh.size(); }
		RenderableMesh& GetMesh(size_t idx) { return renderableMesh[idx]; }

	private:
		std::vector<RenderableMesh> renderableMesh;
	};
}
