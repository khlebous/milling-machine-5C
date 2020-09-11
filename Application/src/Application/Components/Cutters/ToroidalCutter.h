#pragma once

#include "Cutter.h"
#include <string>

class ToroidalCutter : public Cutter
{
	CLASS_DECLARATION(ToroidalCutter)

public:
	ToroidalCutter(float majorRadius, float minorRadius, int horizontalLvls, int roundLvls, float height) : Cutter(majorRadius, minorRadius, horizontalLvls, roundLvls, height) { }

private:
	void InitBottomPart(int horizontalLvls, int roundLvls) final;
};
