#include "SphericalCutter.h"

#include <Graphics/GameObjects/GameObject.h>

CLASS_DEFINITION(Cutter, SphericalCutter)

SphericalCutter::SphericalCutter(float radius, int horizontalLvls, int roundLvls, float height) : Cutter(radius, radius, horizontalLvls, roundLvls, height)
{
	InitBottomPart(horizontalLvls, roundLvls);
	
	cutRadius2 = cutRadius * cutRadius;
}

void SphericalCutter::SetPosition(const sm::Vector3& d0)
{
	GetOwner().GetComponent<fe::Transform>().SetPosition(d0);
}

void SphericalCutter::SetRotation(const sm::Vector3& d1, const sm::Vector3& d1u, const sm::Vector3& d1v)
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

	GetOwner().GetComponent<fe::Transform>().SetRotation(cross, angle);
}

bool SphericalCutter::IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos)
{
	if (IsNearBottomPart(cutterPos, voxelPos))
		return true;

	if (IsNearUpperPart(cutterPos, cutterUpPos, voxelPos))
		return true;

	return false;
}

bool SphericalCutter::IsNearBottomPart(const sm::Vector3& cutterPos, const sm::Vector3& voxelPos)
{
	return sm::Vector3::DistanceSquared(cutterPos, voxelPos) < cutRadius2;
}

bool SphericalCutter::IsNearUpperPart(const sm::Vector3& v, const sm::Vector3& w, const sm::Vector3& voxelPos)
{
	float l2 = sm::Vector3::DistanceSquared(v, w);
	if (l2 == 0.0)
		return sm::Vector3::Distance(voxelPos, v);

	float t = max(0, min(1, (voxelPos - v).Dot(w - v) / l2));
	sm::Vector3 projection = v + t * (w - v);

	return sm::Vector3::Distance(voxelPos, projection) < cutRadius;
}

void SphericalCutter::InitBottomPart(int horizontalLvls, int roundLvls)
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
