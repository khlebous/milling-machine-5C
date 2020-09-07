#include "pch.h"
#include "Mesh.h"
#include "InstancedMesh.h"

namespace fe
{
	
	InstancedMesh::InstancedMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices)
	{
		this->deviceContext = deviceContext;
		this->device = device;

		HRESULT hr = this->vertexbuffer.Initialize(device, vertices, vertStride, vertSize);
		ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

		hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
		ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");

		const int n = 50;
		const int m = 30;
		const int l = 50;
		instancesCount = n * m * l;
		mInstancedData.resize(instancesCount);
		float dx = 1.0;
		//float dy = 3.1;
		//float dz = 3.1;
		for (int k = 0; k < n; k++)
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < l; j++)
				{
					mInstancedData[k * m * l + i * l + j].ipos = DirectX::XMFLOAT4(j * dx, i * dx, k * dx, 1);
						
						/*.world = DirectX::XMFLOAT4X4(
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						j * dx, i * dx, k * dx, 1);*/
					//	i*dx, j*dy, k*dz, 1);
				}
			}
		}

		hr = this->insanceVertexbuffer.InitializeInstanced(device, mInstancedData.data(), sizeof(InstancedData), mInstancedData.size());
		ERROR_IF_FAILED(hr, "Failed to initialize instanced buffer for mesh.");
	}


	InstancedMesh::InstancedMesh(const InstancedMesh& mesh)
	{
		this->deviceContext = mesh.deviceContext;
		this->indexbuffer = mesh.indexbuffer;
		this->vertexbuffer = mesh.vertexbuffer;
	}

	void InstancedMesh::Draw()
	{
		ID3D11Buffer* vbs[2] = { vertexbuffer.Get(), insanceVertexbuffer.Get() };
		const UINT stride[2] = { this->vertexbuffer.Stride(), this->insanceVertexbuffer.Stride() };
		UINT offset[2] = { 0, 0 };

		this->deviceContext->IASetVertexBuffers(0, 2, vbs, stride, offset);
		this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		this->deviceContext->DrawIndexedInstanced(this->indexbuffer.IndexCount(), instancesCount, 0, 0, 0);
	}
	void InstancedMesh::SetData(std::vector<InstancedData>& mInstancedData)
	{
		this->mInstancedData = mInstancedData;
		HRESULT hr = this->insanceVertexbuffer.InitializeInstanced(device, mInstancedData.data(), sizeof(InstancedData), mInstancedData.size());
		ERROR_IF_FAILED(hr, "Failed to initialize instanced buffer for mesh.");
	}
}
