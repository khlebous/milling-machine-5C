#pragma once
#include "Utils/ErrorLogger.h"
#include "Texture.h"

namespace fe
{
	class VertexShader
	{
	public:
		void Initialize(ID3D11Device* device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, D3D11_PRIMITIVE_TOPOLOGY topology);
		ID3D11VertexShader* GetShader();
		ID3D10Blob* GetBuffer();
		ID3D11InputLayout* GetInputLayout();
		D3D11_PRIMITIVE_TOPOLOGY topology;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	};

	class PixelShader
	{
	public:
		void Initialize(ID3D11Device* device, std::wstring shaderpath);
		ID3D11PixelShader* GetShader();
		ID3D10Blob* GetBuffer();

		Texture* AlbedoTexture;
		Texture* MetalnessTexture;
		Texture* RoughnessTexture;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	};

	class GeometryShader
	{
	public:
		void Initialize(ID3D11Device* device, std::wstring shaderpath);
		ID3D11GeometryShader* GetShader();
		ID3D10Blob* GetBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	};
}
