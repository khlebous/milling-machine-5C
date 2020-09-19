#pragma once

#include <string>
#include "Cutter.h"

namespace sm = DirectX::SimpleMath;

class SphericalCutter : public Cutter
{
	CLASS_DECLARATION(SphericalCutter)

public:
	SphericalCutter(float radius, int horizontalLvls, int roundLvls, float height);

	void SetPosition(const sm::Vector3& d0) final;
	void SetRotation(const sm::Vector3& d1, const sm::Vector3& d1u, const sm::Vector3& d1v) final;
	bool IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos) final;

private:
	void InitBottomPart(int horizontalLvls, int roundLvls) override final;
	bool IsNearBottomPart(const sm::Vector3& cutterPos, const sm::Vector3& voxelPos);
	bool IsNearUpperPart(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos);
};
