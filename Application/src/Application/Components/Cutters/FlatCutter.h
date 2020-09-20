#pragma once

#include "Cutter.h"
#include <string>

class FlatCutter : public Cutter
{
	CLASS_DECLARATION(FlatCutter)

public:
	FlatCutter(float radius, int horizontalLvls, int roundLvls, float height);

	void SetPosition(const sm::Vector3& d0, const sm::Vector3& d1u, const sm::Vector3& d1v) final { }
	void SetRotation(const sm::Vector3& d1u, const sm::Vector3& d1v) final { }
	bool IsNear(const sm::Vector3& cutterPos, const sm::Vector3& cutterUpPos, const sm::Vector3& voxelPos) final { return false; }

private:
};
