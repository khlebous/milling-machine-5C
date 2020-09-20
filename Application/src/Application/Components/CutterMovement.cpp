#include "CutterMovement.h"
#include "Cutters/SphericalCutter.h"
#include "Cutters/ToroidalCutter.h"

CLASS_DEFINITION(fe::Component, CutterMovement)

CutterMovement::CutterMovement(Surface* surface)
{
	this->surface = surface;
}

void CutterMovement::OnUpdate()
{
	//SphericalCutter& cutter = GetOwner().GetComponent<SphericalCutter>();
	ToroidalCutter& cutter = GetOwner().GetComponent<ToroidalCutter>();

	if (v > 1)
		return;

	gp_Pnt d0, d1;
	gp_Vec d1u, d1v;

	surface->BEZ->D0(u, v, d0);
	surface->BEZ->D1(u, v, d1, d1u, d1v);

	sm::Vector3 d0_sm(d0.X(), d0.Y(), d0.Z());
	sm::Vector3 d1_sm(d1.X(), d1.Y(), d1.Z());
	sm::Vector3 d1u_sm(d1u.X(), d1u.Y(), d1u.Z());
	sm::Vector3 d1v_sm(d1v.X(), d1v.Y(), d1v.Z());
	
	cutter.SetPosition(d0_sm, d1u_sm, d1v_sm);
	cutter.SetRotation( d1u_sm, d1v_sm);

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

	sm::Matrix rotMatrix = GetOwner().GetComponent<fe::Transform>().GetModelMatrix();
	sm::Vector3 cutterBottomPos = sm::Vector3::Transform(sm::Vector3(0, cutter.cutRadius, 0), rotMatrix);
	sm::Vector3 cutterUpPos = sm::Vector3::Transform(sm::Vector3(0, cutter.height, 0), rotMatrix);

	for (int i = 0; i < instancedMesh->mInstancedData.size(); i++)
	{
		DirectX::XMFLOAT4 tmp = instancedMesh->mInstancedData[i].ipos;
		sm::Vector3 voxelPos = sm::Vector3(tmp.x, tmp.y, tmp.z);

		if(!cutter.IsNear(cutterBottomPos, cutterUpPos, voxelPos))
			mInstancedDatas.emplace_back(instancedMesh->mInstancedData[i]);
	}
	instancedMesh->SetData(mInstancedDatas);
}
