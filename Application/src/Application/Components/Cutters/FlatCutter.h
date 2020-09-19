#pragma once

#include "Cutter.h"
#include <string>

class FlatCutter : public Cutter
{
	CLASS_DECLARATION(FlatCutter)

public:
	FlatCutter(float radius, int horizontalLvls, int roundLvls, float height) : Cutter(radius, 0, horizontalLvls, roundLvls, height) { }
	void SetPosition(const sm::Vector3& d0) final { }
	void SetRotation(const sm::Vector3& d1, const sm::Vector3& d1u, const sm::Vector3& d1v) final { }

private:
};
