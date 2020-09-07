#pragma once
#include "Vertex.h"
#include "Buffers\VertexBuffer.h"
#include "Buffers\IndexBuffer.h"
#include "Buffers\ConstantBuffer.h"
#include "IMesh.h"

namespace fe
{
	struct InstancedData
	{
		DirectX::XMFLOAT4 ipos;
		//DirectX::XMFLOAT4X4 world;
	};

	class InstancedMesh : public IMesh
	{

	public:
		InstancedMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
			void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices);
		InstancedMesh(const InstancedMesh& mesh);

		void Init(void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices) final { }

		int VerticesCount() final { return vertexbuffer.VertexCount(); }
		int IndicesCount() final { return indexbuffer.IndexCount(); }
		int TrianglesCount() final { return indexbuffer.IndexCount() / 3; }
		void Draw() final;
		void SetData(std::vector<InstancedData>& data);
		int instancesCount;
		std::vector<InstancedData> mInstancedData;
		ID3D11Device* device;

	private:
		VertexBuffer vertexbuffer;
		VertexBuffer insanceVertexbuffer;
		IndexBuffer indexbuffer;
		ID3D11DeviceContext* deviceContext;
	};
}
