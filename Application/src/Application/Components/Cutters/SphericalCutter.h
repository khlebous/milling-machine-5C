#pragma once

#include "Cutter.h"
#include <string>

namespace sm = DirectX::SimpleMath;

class SphericalCutter : public Cutter
{
	CLASS_DECLARATION(SphericalCutter)

public:
	SphericalCutter(float radius, int horizontalLvls, int roundLvls, float height) : Cutter(radius, radius, horizontalLvls, roundLvls, height)
	{
		InitBottomPart(horizontalLvls, roundLvls);
	}

private:
	void InitBottomPart(int horizontalLvls, int roundLvls) final
	{
		for (int i = 0; i < horizontalLvls; i++)
		{
			float angle = DirectX::XM_2PI * i / horizontalLvls;
			float angle2 = DirectX::XM_2PI * (i + 1) / horizontalLvls;

			for (int j = 0; j < roundLvls; j++)
			{
				float roundAngle = DirectX::XM_PIDIV2 * j / roundLvls;
				float roundAngle2 = DirectX::XM_PIDIV2 * (j + 1) / roundLvls;

				sm::Vector3 a = sm::Vector3(cos(angle) * sin(roundAngle), sin(angle) * sin(roundAngle), cos(roundAngle)); a.Normalize();
				sm::Vector3 a2 = sm::Vector3(cos(angle) * sin(roundAngle2), sin(angle) * sin(roundAngle2), cos(roundAngle2)); a2.Normalize();
				sm::Vector3 b = sm::Vector3(cos(angle2) * sin(roundAngle), sin(angle2) * sin(roundAngle), cos(roundAngle)); b.Normalize();
				sm::Vector3 b2 = sm::Vector3(cos(angle2) * sin(roundAngle2), sin(angle2) * sin(roundAngle2), cos(roundAngle2)); b2.Normalize();

				int count = vertices.size();
				vertices.emplace_back(cutRadius * a.x, cutRadius * (1 - a.z), cutRadius * a.y, a.x, a.y, a.z);
				vertices.emplace_back(cutRadius * a2.x, cutRadius * (1 - a2.z), cutRadius * a2.y, a2.x, a2.y, a2.z);
				vertices.emplace_back(cutRadius * b2.x, cutRadius * (1 - b2.z), cutRadius * b2.y, b2.x, b2.y, b2.z);
				vertices.emplace_back(cutRadius * b.x, cutRadius * (1 - b.z), cutRadius * b.y, b2.x, b.y, b.z);

				indices.push_back(count); indices.push_back(count + 1); indices.push_back(count + 2);
				indices.push_back(count); indices.push_back(count + 2); indices.push_back(count + 3);
			}
		}
	}
};

