#include "ToroidalCutter.h"
#include <Graphics/GameObjects/GameObject.h>

CLASS_DEFINITION(Cutter, ToroidalCutter)

void ToroidalCutter::SetPosition(const sm::Vector3& d0)
{
	GetOwner().GetComponent<fe::Transform>().SetPosition(d0);
}

void ToroidalCutter::SetRotation(const sm::Vector3& d1, const sm::Vector3& d1u, const sm::Vector3& d1v)
{
	// TODO
	sm::Vector3 rot(d1u.Cross(d1v)); rot.Normalize();
	sm::Vector3 cross = rot.Cross(sm::Vector3::Up); cross.Normalize();
	float angle = acos(rot.Dot(sm::Vector3::Up));

	if (cross.Length() < 0.001)
	{
		if (rot.Dot(sm::Vector3::Up) > 0) // Should be 1
		{
			cross = sm::Vector3::Up;
			angle = 0;
		}
		else // -1
		{
			cross = sm::Vector3(0, 0, 1);
			angle = DirectX::XM_PI;
		}
	}
	else
	{
		sm::Vector3 add_cross = cross.Cross(sm::Vector3::Up);
		if (add_cross.Dot(rot) < 0)
			angle = -angle;
	}

	GetOwner().GetComponent<fe::Transform>().SetRotation(cross, angle);
}

ToroidalCutter::ToroidalCutter(float majorRadius, float minorRadius, int horizontalLvls, int roundLvls, float height) : Cutter(majorRadius, 0.0f, horizontalLvls, roundLvls, height)
{
	this->minorRadius = minorRadius;
	this->majorRadius = majorRadius;
	InitBottomPart(horizontalLvls, roundLvls);
}

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

bool ToroidalCutter::IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos)
{
	// TODO
	if (IsNearBottomPart(cutterPos, voxelPos))
		return true;

	if (IsNearUpperPart(cutterPos, cutterUpPos, voxelPos))
		return true;

	return false;
}

bool ToroidalCutter::IsNearBottomPart(const sm::Vector3& cutterPos, const sm::Vector3& voxelPos)
{
	return sm::Vector3::DistanceSquared(cutterPos, voxelPos) < cutRadius2;
}

bool ToroidalCutter::IsNearUpperPart(const sm::Vector3& v, const sm::Vector3& w, const sm::Vector3& voxelPos)
{
	float l2 = sm::Vector3::DistanceSquared(v, w);
	if (l2 == 0.0)
		return sm::Vector3::Distance(voxelPos, v);

	float t = max(0, min(1, (voxelPos - v).Dot(w - v) / l2));
	sm::Vector3 projection = v + t * (w - v);

	return sm::Vector3::Distance(voxelPos, projection) < cutRadius;
}
