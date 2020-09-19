#pragma once

#include <Graphics/Components/Component.h>
#include <Graphics/Vertex.h>

#include <ImGui/imgui.h>
#include <vector>

namespace sm = DirectX::SimpleMath;

class Cutter : public fe::Component
{
	CLASS_DECLARATION(Cutter)

public:
	Cutter(float radius, float startHeight, int horizontalLvls, int roundLvls, float height);

	float cutRadius;
	float cutRadius2;
	float height;
	std::vector<fe::VertexPN> vertices;
	std::vector<DWORD> indices;

	virtual void SetPosition(const sm::Vector3& d0) = 0;
	virtual void SetRotation(const sm::Vector3& d1, const sm::Vector3& d1u, const sm::Vector3& d1v) = 0;
	virtual bool IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos) = 0;
	void GetUI() final;

protected:
	void InitUpperPart(int horizontalLvls, float startHeight, float height);
	virtual void InitBottomPart(int horizontalLvls, int roundLvls) = 0;
};
