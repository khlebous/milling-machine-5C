#pragma once

#include "Cutter.h"
#include <string>

namespace sm = DirectX::SimpleMath;

class ToroidalCutter : public Cutter
{
	CLASS_DECLARATION(ToroidalCutter)

public:
	ToroidalCutter(float majorRadius, float minorRadius, int horizontalLvls, int roundLvls, float height);

	void SetPosition(const sm::Vector3& d0, const sm::Vector3& d1u, const sm::Vector3& d1v) final;
	void SetRotation(const sm::Vector3& d1u, const sm::Vector3& d1v);
	bool IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos) final;
	void GetUI() final;

protected:
	void InitUpperPart(int horizontalLvls, float startHeight, float height);
	void InitBottomPart(int horizontalLvls, int roundLvls) final;
	bool IsNearBottomPart(const sm::Vector3& cutterPos, const sm::Vector3& voxelPos);
	bool IsNearUpperPart(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos);

private:
	float majorRadius;
	float minorRadius;
	float alpha;

	sm::Vector3 GetTorusPoint(float majorRadius, float minorRadius, float majorRadiusAngle, float minorRadiusAngle);
};
