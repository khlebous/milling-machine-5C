#include "ToroidalCutter.h"
#include <Graphics/GameObjects/GameObject.h>

CLASS_DEFINITION(Cutter, ToroidalCutter)

constexpr float ANGLE = DirectX::XM_PIDIV4;

void ToroidalCutter::SetPosition(const sm::Vector3& d0, const sm::Vector3& d1u, const sm::Vector3& d1v)
{
	sm::Vector3 normal(d1u.Cross(d1v)); normal.Normalize();
	sm::Vector3 pos = d0 + normal * minorRadius;

	GetOwner().GetComponent<fe::Transform>().SetPosition(pos);
}

void ToroidalCutter::SetRotation(const sm::Vector3& d1u, const sm::Vector3& d1v)
{
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

	sm::Matrix rotMatrix1 = DirectX::XMMatrixRotationAxis(sm::Vector3::Left, ANGLE);
	sm::Matrix rotMatrix2 = DirectX::XMMatrixRotationAxis(cross, angle);
	GetOwner().GetComponent<fe::Transform>().SetRotation(rotMatrix1 * rotMatrix2);
}

ToroidalCutter::ToroidalCutter(float majorRadius, float minorRadius, int horizontalLvls, int roundLvls, float height) : Cutter(majorRadius, minorRadius, horizontalLvls, roundLvls, height)
{
	this->minorRadius = minorRadius;
	this->majorRadius = majorRadius;

	InitUpperPart(horizontalLvls, 0, height);
	InitBottomPart(horizontalLvls, roundLvls);

	this->cutRadius = 0.0f;
}

void ToroidalCutter::InitUpperPart(int horizontalLvls, float startHeight, float height)
{
	for (int i = 0; i < horizontalLvls; i++)
	{
		float angle = DirectX::XM_2PI * i / horizontalLvls;
		float angle2 = DirectX::XM_2PI * (i + 1) / horizontalLvls;
		sm::Vector3 a = sm::Vector3(cos(angle), sin(angle), 0);	a.Normalize();
		sm::Vector3 b = sm::Vector3(cos(angle2), sin(angle2), 0); b.Normalize();

		int count = vertices.size();
		vertices.emplace_back(cutRadius * a.x, startHeight, cutRadius * a.y + majorRadius - minorRadius, a.x, a.y, 0);
		vertices.emplace_back(cutRadius * a.x, height, cutRadius * a.y + majorRadius - minorRadius, a.x, a.y, 0);
		vertices.emplace_back(cutRadius * b.x, height, cutRadius * b.y + majorRadius - minorRadius, b.x, b.y, 0);
		vertices.emplace_back(cutRadius * b.x, startHeight, cutRadius * b.y + majorRadius - minorRadius, b.x, b.y, 0);

		indices.push_back(count); indices.push_back(count + 1); indices.push_back(count + 2);
		indices.push_back(count); indices.push_back(count + 2); indices.push_back(count + 3);
	}
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
	float z = (majorRadius + minorRadius * cos(minorRadiusAngle)) * sin(majorRadiusAngle) + majorRadius;
	float y = -(minorRadius * sin(minorRadiusAngle));

	return sm::Vector3(x, y, z);
}

bool ToroidalCutter::IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos)
{
	if (IsNearBottomPart(cutterPos, voxelPos))
		return true;

	if (IsNearUpperPart(cutterPos, cutterUpPos, voxelPos))
		return true;

	return false;
}

bool ToroidalCutter::IsNearBottomPart(const sm::Vector3& cutterPos, const sm::Vector3& voxelPos)
{
	// check if voxel pos inside torus
	const float MRadius = majorRadius - minorRadius;

	auto localVoxelPos = voxelPos;
	localVoxelPos.z -= MRadius;
	auto localVoxelPosSquared = localVoxelPos * localVoxelPos;

	float tmp = sqrt(localVoxelPosSquared.x + localVoxelPosSquared.z) - MRadius;
	return (tmp * tmp + localVoxelPosSquared.y) < (minorRadius * minorRadius);
}

bool ToroidalCutter::IsNearUpperPart(const sm::Vector3& v, const sm::Vector3& w, const sm::Vector3& voxelPos)
{
	const float MRadius = majorRadius - minorRadius;
	auto cutterBottom = v;
	auto cutterTop = w;
	cutterBottom.z += MRadius;
	cutterTop.z += MRadius;

	float l2 = sm::Vector3::DistanceSquared(cutterBottom, cutterTop);
	if (l2 == 0.0)
		return sm::Vector3::Distance(voxelPos, cutterBottom);

	float t = max(0, min(1, (voxelPos - cutterBottom).Dot(cutterTop - cutterBottom) / l2));
	sm::Vector3 projection = cutterBottom + t * (cutterTop - cutterBottom);

	return sm::Vector3::Distance(voxelPos, projection) < majorRadius;
}
