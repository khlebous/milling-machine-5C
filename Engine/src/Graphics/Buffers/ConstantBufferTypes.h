#pragma once
#include "pch.h"

namespace fe
{
	// VERTEX SHADER

	/// <summary>
	/// Constant Buffer used in Vertex Shader under index 0
	/// </summary>
	struct CB_VS_mvpMatrices
	{
		DirectX::SimpleMath::Matrix modelMatrix;
		DirectX::SimpleMath::Matrix viewMatrix;
		DirectX::SimpleMath::Matrix invViewMatrix;
		DirectX::SimpleMath::Matrix projMatrix;
	};

	// PIXEL SHADER

	/// <summary>
	/// Constant Buffer used in Pixel Shader under index 0
	/// </summary>
	struct CB_PS_lightsCount
	{
		int pointLightCount;
	};

	/// <summary>
	/// Constant Buffer used in Pixel Shader under index 1
	/// </summary>
	struct CB_PS_pointLight
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 ambient;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector4 specular;
	};

	/// <summary>
	/// Constant Buffer used in Pixel Shader under index 2
	/// </summary>
	struct CB_PS_material
	{
		DirectX::SimpleMath::Vector4 ambient;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector4 specular;
		float m;

		float _not_used0;
		float _not_used1;
		float _not_used2;

	};
}
