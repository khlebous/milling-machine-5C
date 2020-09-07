#include "pch.h"
#include "Cutter.h"

namespace sm = DirectX::SimpleMath;

CLASS_DEFINITION(fe::Component, Cutter)

Cutter::Cutter(float radius, float startHeight)
{
	this->cutRadius = radius;
	int horizontalLvls = 10;
	int roundLvls = 10;
	float height = 40;

	InitUpperPart(horizontalLvls, startHeight, height);
	InitBottomPart(horizontalLvls, roundLvls);
}

void Cutter::InitUpperPart(int horizontalLvls, float startHeight, float height)
{
	for (int i = 0; i < horizontalLvls; i++)
	{
		float angle = DirectX::XM_2PI * i / horizontalLvls;
		float angle2 = DirectX::XM_2PI * (i + 1) / horizontalLvls;
		sm::Vector3 a = sm::Vector3(cos(angle), sin(angle), 0);	a.Normalize();
		sm::Vector3 b = sm::Vector3(cos(angle2), sin(angle2), 0); b.Normalize();

		int count = vertices.size();
		vertices.emplace_back(cutRadius * a.x, startHeight, cutRadius * a.y, a.x, a.y, 0);
		vertices.emplace_back(cutRadius * a.x, height, cutRadius * a.y, a.x, a.y, 0);
		vertices.emplace_back(cutRadius * b.x, height, cutRadius * b.y, b.x, b.y, 0);
		vertices.emplace_back(cutRadius * b.x, startHeight, cutRadius * b.y, b.x, b.y, 0);

		indices.push_back(count); indices.push_back(count + 1); indices.push_back(count + 2);
		indices.push_back(count); indices.push_back(count + 2); indices.push_back(count + 3);
	}
}

void Cutter::GetUI()
{
	if (ImGui::TreeNodeEx("Cutter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("radius", &cutRadius);

		ImGui::TreePop();
	}
}
