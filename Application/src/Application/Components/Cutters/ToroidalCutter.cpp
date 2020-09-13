#include "ToroidalCutter.h"

CLASS_DEFINITION(Cutter, ToroidalCutter)

void ToroidalCutter::InitBottomPart(int horizontalLvls, int roundLvls)
{
	float majorRadius = this->majorRadius - this->minorRadius;
	float minorRadius = this->minorRadius;

	for (int i = 0; i < horizontalLvls; i++)
	{
		float majorRadiusAngle = DirectX::XM_2PI * i / horizontalLvls;
		float majorRadiusAngle2 = DirectX::XM_2PI * (i + 1) / horizontalLvls;

		for (int j = 0; j < roundLvls; j++)
		{
			float minorRadiusAngle = DirectX::XM_2PI * j / roundLvls;
			float minorRadiusAngle2 = DirectX::XM_2PI * (j + 1) / roundLvls;

			sm::Vector3 p1 = GetTorusPoint(majorRadius, minorRadius, majorRadiusAngle, minorRadiusAngle);
			sm::Vector3 p1Normal = p1;
			p1Normal.Normalize();

			sm::Vector3 p2 = GetTorusPoint(majorRadius, minorRadius, majorRadiusAngle, minorRadiusAngle2);
			sm::Vector3 p2Normal = p2;
			p2Normal.Normalize();

			sm::Vector3 p3 = GetTorusPoint(majorRadius, minorRadius, majorRadiusAngle2, minorRadiusAngle);
			sm::Vector3 p3Normal = p3;
			p3Normal.Normalize();

			sm::Vector3 p4 = GetTorusPoint(majorRadius, minorRadius, majorRadiusAngle2, minorRadiusAngle2);
			sm::Vector3 p4Normal = p4;
			p4Normal.Normalize();

			int count = vertices.size();
			vertices.emplace_back(p1, p1Normal);
			vertices.emplace_back(p2, p2Normal);
			vertices.emplace_back(p3, p3Normal);
			vertices.emplace_back(p4, p4Normal);

			indices.push_back(count); indices.push_back(count + 2); indices.push_back(count + 1);
			indices.push_back(count + 2); indices.push_back(count + 3); indices.push_back(count + 1);
		}
	}
}

// GetPoint on Torus defined on XZ Plane
sm::Vector3 ToroidalCutter::GetTorusPoint(float majorRadius, float minorRadius, float majorRadiusAngle, float minorRadiusAngle)
{
	float x = (majorRadius + minorRadius * cos(minorRadiusAngle)) * cos(majorRadiusAngle);
	float z = (majorRadius + minorRadius * cos(minorRadiusAngle)) * sin(majorRadiusAngle);
	float y = -(minorRadius * sin(minorRadiusAngle));

	return sm::Vector3(x, y, z);
}
