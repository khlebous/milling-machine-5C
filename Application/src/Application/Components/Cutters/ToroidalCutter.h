#pragma once

#include "Cutter.h"
#include <string>

namespace sm = DirectX::SimpleMath;

class ToroidalCutter : public Cutter
{
	CLASS_DECLARATION(ToroidalCutter)

public:
	ToroidalCutter(float majorRadius, float minorRadius, int horizontalLvls, int roundLvls, float height);

	void SetPosition(const sm::Vector3& d0) final;
	void SetRotation(const sm::Vector3& d1, const sm::Vector3& d1u, const sm::Vector3& d1v);

protected:
	void InitBottomPart(int horizontalLvls, int roundLvls) final;

private:
	float majorRadius;
	float minorRadius;

	sm::Vector3 GetTorusPoint(float majorRadius, float minorRadius, float majorRadiusAngle, float minorRadiusAngle);
};
