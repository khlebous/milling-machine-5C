#include "pch.h"
#include "Graphics.h"
#include "Constants/PathConstants.h"
#include "Constants/CBConstants.h"

namespace fe
{
	bool Graphics::Initialize(HWND hwnd, float width, float height)
	{
		if (!InitializeDirectX(hwnd, width, height))
			return false;

		if (!InitializeShaders())
			return false;

		if (!InitializeScene())
			return false;

		return true;
	}

	void Graphics::UpdateLights(Lights& lights)
	{
		cb_ps_pointLight.Update(lights.GetPointLightsData());
		cb_ps_lightsCount.Update(&lights.GetLightsCountData());
	}

	void Graphics::OnPreRender()
	{
		device.context()->RSSetViewports(1, &vieport_engine);
		device.context()->OMSetRenderTargets(1, this->renderTargetView_engine.GetAddressOf(), depthStencilView_engine.Get());

		device.context()->ClearRenderTargetView(this->renderTargetView_engine.Get(), clearColor);
		device.context()->ClearDepthStencilView(this->depthStencilView_engine.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		device.context()->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
		device.context()->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
		device.context()->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

		device.context()->PSSetConstantBuffers(CbPsIdxs::PointLightIdx, 1, this->cb_ps_pointLight.GetAddressOf());
		device.context()->PSSetConstantBuffers(CbPsIdxs::LightsCountIdx, 1, this->cb_ps_lightsCount.GetAddressOf());
	}

	void Graphics::RestoreContext()
	{
		device.context()->RSSetViewports(1, &viewport);
		device.context()->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		device.context()->ClearRenderTargetView(this->renderTargetView.Get(), clearColor);
		device.context()->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Graphics::OnPostRender()
	{
		device.swapChain()->Present(1, 0);
	}

	void Graphics::OnResize(float width, float height)
	{
		if (!device.swapChain())
			return;

		device.context()->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		renderTargetView->Release();
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		HRESULT hr = device.swapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		UpdateViewport(width, height);
		UpdateRenderTargetView();
		UpdateDepthStencilView(width, height);
		
		device.context()->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());
	}

	bool Graphics::InitializeDirectX(HWND hwnd, float width, float height)
	{
		try
		{
			device.Initialize(hwnd, width, height);

			UpdateViewport(width, height);
			UpdateRenderTargetView();
			UpdateDepthStencilView(width, height);

			device.context()->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

			//Create depth stencil state
			CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
			depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			HRESULT hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
			ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

			//Create sampler description for sampler state
			CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
			ERROR_IF_FAILED(hr, "Failed to create sampler state.");
		}
		catch (COMException& exception)
		{
			ErrorLogger::Log(exception);
			return false;
		}
		return true;
	}

	bool Graphics::InitializeShaders()
	{
		std::wstring root = StringHelper::StringToWide(PathConstants::SHADERS_PATH);

		cutterVertexShader.Initialize(this->device, root + L"cutter_vs.cso", VertexPN::layout, ARRAYSIZE(VertexPN::layout),
			D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cutterPixelShader.Initialize(this->device, root + L"cutter_ps.cso");

		surfaceVertexShader.Initialize(this->device, root + L"surface_vs.cso", VertexP::layout, ARRAYSIZE(VertexP::layout),
			D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
		surfacePixelShader.Initialize(this->device, root + L"surface_ps.cso");

		lampVertexShader.Initialize(this->device, root + L"lamp_vs.cso", VertexP::layout, ARRAYSIZE(VertexP::layout),
			D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		lampPixelShader.Initialize(this->device, root + L"lamp_ps.cso");

		materialVS.Initialize(this->device, root + L"material_vs.cso", VertexPNW::layout, ARRAYSIZE(VertexPNW::layout),
			D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		materialPS.Initialize(this->device, root + L"material_ps.cso");

		return true;
	}

	bool Graphics::InitializeScene()
	{
		try
		{
			this->cb_ps_pointLight.Initialize(device.get(), device.context(), Lights::maxPointLightCount);
			this->cb_ps_lightsCount.Initialize(device.get(), device.context());
		}
		catch (COMException& exception)
		{
			ErrorLogger::Log(exception);
			return false;
		}
		return true;
	}

	void Graphics::UpdateViewport(float width, float height)
	{
		viewport = CD3D11_VIEWPORT(0.0f, 0.0f, width, height);
		device.context()->RSSetViewports(1, &viewport);
	}

	void Graphics::UpdateRenderTargetView()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = device.swapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create render target view.");
	}

	void Graphics::UpdateDepthStencilView(float width, float height)
	{
		auto depthStencilTextureDesc = Texture::GetDepthStencilTexDesc(width, height);

		HRESULT hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.ReleaseAndGetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.ReleaseAndGetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");
	}

	void Graphics::InitializeGameTexture(Texture* game_texture)
	{
		vieport_engine = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(game_texture->GetWidth()), static_cast<float>(game_texture->GetHeight()));

		this->game_texture = game_texture;
		this->device->CreateRenderTargetView(game_texture->GetTexture(), nullptr, renderTargetView_engine.ReleaseAndGetAddressOf());

		auto depthStencilTextureDesc = Texture::GetDepthStencilTexDesc(game_texture->GetWidth(), game_texture->GetHeight());

		HRESULT hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer_engine.ReleaseAndGetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer_engine.Get(), NULL, this->depthStencilView_engine.ReleaseAndGetAddressOf());
		ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");
	}
}
