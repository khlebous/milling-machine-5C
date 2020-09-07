#pragma once

#include "Vertex.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/ConstantBuffer.h"
#include "Shaders.h"
#include "Device.h"
#include "Lights/Lights.h"

namespace fe
{
	class Graphics
	{
	public:
		bool Initialize(HWND hwnd, float width, float height);
		void OnResize(float width, float height);
		void UpdateLights(Lights& lights);
		void InitializeGameTexture(Texture* game_texture);
		void RestoreContext();

		void OnPreRender();
		void OnPostRender();

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_engine;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		Device device;
		VertexShader cutterVertexShader, lampVertexShader, surfaceVertexShader, materialVS;
		PixelShader cutterPixelShader, lampPixelShader, surfacePixelShader, materialPS;

	private:
		bool InitializeDirectX(HWND hwnd, float width, float height);
		bool InitializeShaders();
		bool InitializeScene();

		void UpdateViewport(float width, float height);
		void UpdateRenderTargetView();
		void UpdateDepthStencilView(float width, float height);

		ConstantBuffer<CB_PS_pointLight> cb_ps_pointLight;
		ConstantBuffer<CB_PS_lightsCount> cb_ps_lightsCount;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, renderTargetView_engine;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer, depthStencilBuffer_engine;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

		CD3D11_VIEWPORT vieport_engine;
		CD3D11_VIEWPORT viewport;
		Texture* game_texture;

		//float clearColor[4] = { 0.0f, 0.4f, 0.1f, 1.0f };
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}
