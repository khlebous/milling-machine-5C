#pragma once

#include "Cutter.h"
#include <string>

class FlatCutter : public Cutter
{
	CLASS_DECLARATION(FlatCutter)

public:
	FlatCutter(float radius, int horizontalLvls, int roundLvls, float height) : Cutter(radius, 0, horizontalLvls, roundLvls, height) { }
	void SetPosition(const sm::Vector3& d0) final { }

private:
};
