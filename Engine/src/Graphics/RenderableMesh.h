#pragma once
#include "Graphics/Model.h"
#include "Graphics/Shaders.h"
#include "Graphics/IMesh.h"

namespace fe
{
	class RenderableMesh
	{
	public:
		RenderableMesh(IMesh* mesh, Material* material, VertexShader* vs, PixelShader* ps);

		IMesh* mesh;
		Material* material;
		VertexShader* vertexShader;
		PixelShader* pixelShader;
	};
}
