#pragma once
#include "Vertex.h"
#include "Buffers\VertexBuffer.h"
#include "Buffers\IndexBuffer.h"
#include "Buffers\ConstantBuffer.h"
#include "IMesh.h"

namespace fe
{
	class Mesh : public IMesh
	{
	public:
		Mesh() {}
		Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
			void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices);
		//Mesh(const Mesh& mesh);
		Mesh(VertexBuffer& vertexbuffer, IndexBuffer& indexbuffer, ID3D11DeviceContext* deviceContext)
		{
			this->device = device;
			this->deviceContext = deviceContext;
			this->indexbuffer = indexbuffer;
			this->vertexbuffer = vertexbuffer;
		}
		Mesh(Mesh* mesh);

		void Init(void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices) final
		{
			HRESULT hr = this->vertexbuffer.Initialize(device, vertices, vertStride, vertSize);
			ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

			hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
			ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
		}

		int VerticesCount() final { return vertexbuffer.VertexCount(); }
		int IndicesCount() final { return indexbuffer.IndexCount(); }
		int TrianglesCount() final{ return indexbuffer.IndexCount() / 3; }
		void Draw() final;

	//private:
		VertexBuffer vertexbuffer;
		IndexBuffer indexbuffer;
		ID3D11DeviceContext* deviceContext;
		ID3D11Device* device;
	};
}