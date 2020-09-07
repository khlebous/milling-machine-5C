#include "pch.h"
#include "Mesh.h"

namespace fe
{
	Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
		void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices)
	{
		this->deviceContext = deviceContext;
		this->device = device;

		HRESULT hr = this->vertexbuffer.Initialize(device, vertices, vertStride, vertSize);
		ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

		hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
		ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
	}
	/*Mesh::Mesh(const Mesh& mesh)
	{
		this->deviceContext = mesh.deviceContext;
		this->indexbuffer = mesh.indexbuffer;
		this->vertexbuffer = mesh.vertexbuffer;
	}*/

	Mesh::Mesh(Mesh* mesh)
	{
		this->deviceContext = mesh->deviceContext;
		this->indexbuffer = mesh->indexbuffer;
		this->vertexbuffer = mesh->vertexbuffer;
	}

	void Mesh::Draw()
	{
		UINT offset = 0;

		this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
		this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
	}
}
