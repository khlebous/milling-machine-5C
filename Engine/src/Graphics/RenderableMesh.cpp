#include "pch.h"
#include "RenderableMesh.h"

namespace fe
{
	RenderableMesh::RenderableMesh(IMesh* mesh, Material* material, VertexShader* vs, PixelShader* ps)
	{
		this->mesh = mesh;
		this->material = material;
		this->vertexShader = vs;
		this->pixelShader = ps;
	}
}
