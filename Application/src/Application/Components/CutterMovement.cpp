#include "CutterMovement.h"

CLASS_DEFINITION(fe::Component, CutterMovement)

CutterMovement::CutterMovement(Surface* surface)
{
	this->surface = surface;
}

bool Distance(sm::Vector3& pos1, sm::Vector3& pos2)
{
	//return (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z) < 9;
	float tmp = sqrt(pos2.x * pos2.x + pos2.z + pos2.z) - 2.0f;

	return tmp * tmp + pos2.y * pos2.y < 1.0f;
}

bool Distance(sm::Vector3& v, sm::Vector3& w, sm::Vector3& p)
{
	float l2 = sm::Vector3::DistanceSquared(v, w);
	if (l2 == 0.0)
		return sm::Vector3::Distance(p, v);

	float t = max(0, min(1, (p - v).Dot(w - v) / l2));
	sm::Vector3 projection = v + t * (w - v);

	return sm::Vector3::Distance(p, projection) < 3;
}

void CutterMovement::OnUpdate()
{
	if (v > 1)
		return;

	gp_Pnt x;
	surface->BEZ->D0(u, v, x);
	sm::Vector3 pos(x.X(), x.Y(), x.Z());
	GetOwner().GetComponent<fe::Transform>().SetPosition(pos);

	gp_Pnt p;
	gp_Vec d1u, d1v;
	surface->BEZ->D1(u, v, p, d1u, d1v);
	sm::Vector3 vec_u(d1u.X(), d1u.Y(), d1u.Z());
	sm::Vector3 vec_v(d1v.X(), d1v.Y(), d1v.Z());

	sm::Vector3 rot(vec_u.Cross(vec_v)); rot.Normalize();
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

	auto& cutterTransform = GetOwner().GetComponent<fe::Transform>();
	cutterTransform.SetRotation(cross, angle);

	auto modelMatrix = cutterTransform.GetModelMatrix();
	auto inversedModelMatrix = modelMatrix.Invert();

	u += 0.01;
	if (u > 1)
	{
		u = 0;
		v += 0.01;
	}

	if (!instancedMesh)
		return;

	std::vector<fe::InstancedData> mInstancedDatas;
	mInstancedDatas.reserve(instancedMesh->mInstancedData.size());

	//sm::Matrix rotMatrix =  DirectX::XMMatrixRotationAxis(cross, angle);
	sm::Matrix rotMatrix = GetOwner().GetComponent<fe::Transform>().GetModelMatrix();
	//pos = sm::Vector3::Transform(sm::Vector3(0, 3, 0), rotMatrix);
	//sm::Vector3 pos1 = sm::Vector3::Transform(sm::Vector3(0, 40, 0), rotMatrix);

	sm::Matrix invRotMatrix = rotMatrix.Invert();

	pos = sm::Vector3(0, 0, 0);
	sm::Vector3 pos1 = sm::Vector3(0, 40, 0);

	for (int i = 0; i < instancedMesh->mInstancedData.size(); i++)
	{
		DirectX::XMFLOAT4 tmp = instancedMesh->mInstancedData[i].ipos;
		sm::Vector3 voxelPosition = sm::Vector3(tmp.x, tmp.y, tmp.z);
		voxelPosition = sm::Vector3::Transform(voxelPosition, invRotMatrix);

		if (!Distance(pos, voxelPosition))
		{
			//mInstancedDatas.emplace_back(instancedMesh->mInstancedData[i]);

			if (!Distance(pos, pos1, voxelPosition))
				mInstancedDatas.emplace_back(instancedMesh->mInstancedData[i]);
		}
	}
	instancedMesh->SetData(mInstancedDatas);
}
