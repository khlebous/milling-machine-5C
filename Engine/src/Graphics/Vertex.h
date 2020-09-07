#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	struct VertexP
	{
		VertexP() {}

		VertexP(sm::Vector3 _pos)
			: pos(_pos) {}

		VertexP(float x, float y, float z)
			: pos(x, y, z) {}

		sm::Vector3 pos;

		static D3D11_INPUT_ELEMENT_DESC layout[1];
	};

	struct VertexPN
	{
		VertexPN() {}

		VertexPN(sm::Vector3 _pos, sm::Vector3 _normal)
			: pos(_pos), normal(_normal) {}

		VertexPN(float x, float y, float z, float nx, float ny, float nz)
			: pos(x, y, z), normal(nx, ny, nz) {}

		sm::Vector3 pos;
		sm::Vector3 normal;

		static D3D11_INPUT_ELEMENT_DESC layout[2];
	};

	struct VertexPT
	{
		VertexPT() {}

		VertexPT(sm::Vector3 _pos, sm::Vector2 _tex)
			: pos(_pos), tex(_tex) {}

		VertexPT(float x, float y, float z, float u, float v)
			: pos(x, y, z), tex(u, v) {}

		sm::Vector3 pos;
		sm::Vector2 tex;

		static D3D11_INPUT_ELEMENT_DESC layout[2];
	};

	struct VertexPT3
	{
		VertexPT3() {}

		VertexPT3(sm::Vector3 _pos, sm::Vector3 _tex)
			: pos(_pos), tex(_tex) {}

		VertexPT3(float x, float y, float z, float u, float v, float w)
			: pos(x, y, z), tex(u, v, w) {}

		sm::Vector3 pos;
		sm::Vector3 tex;

		static D3D11_INPUT_ELEMENT_DESC layout[2];
	};

	struct VertexPNT
	{
		VertexPNT() {}

		VertexPNT(sm::Vector3 _pos, sm::Vector3 _normal, sm::Vector2 _tex)
			: pos(_pos), normal(_normal), tex(_tex) { }

		VertexPNT(float x, float y, float z, float nx, float ny, float nz, float u, float v)
			: pos(x, y, z), normal(nx, ny, nz), tex(u, v) { }

		sm::Vector3 pos;
		sm::Vector3 normal;
		sm::Vector2 tex;

		static D3D11_INPUT_ELEMENT_DESC layout[3];
	};

	struct VertexPNW
	{
		VertexPNW() {}

		VertexPNW(sm::Vector3 _pos, sm::Vector3 _normal, sm::Vector4 _world0, sm::Vector4 _world1, sm::Vector4 _world2, sm::Vector4 _world3)
			: pos(_pos), normal(_normal), world0(_world0), world1(_world1), world2(_world2), world3(_world3) { }

		sm::Vector3 pos;
		sm::Vector3 normal;
		sm::Vector4 world0;
		sm::Vector4 world1;
		sm::Vector4 world2;
		sm::Vector4 world3;

		static D3D11_INPUT_ELEMENT_DESC layout[6];
	};

	struct VertexPTC
	{
		VertexPTC() {}

		VertexPTC(sm::Vector3 _pos, sm::Vector2 _tex, sm::Vector4 _color)
			: pos(_pos), tex(_tex), color(_color) {}

		VertexPTC(float x, float y, float z, float u, float v, float r, float g, float b, float a)
			: pos(x, y, z), tex(u, v), color(r, g, b, a) {}

		sm::Vector3 pos;
		sm::Vector2 tex;
		sm::Vector4 color;

		static D3D11_INPUT_ELEMENT_DESC layout[3];
	};
}
