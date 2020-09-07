#pragma once

#include "Cutter.h"
#include <string>

class ToroidalCutter : public Cutter
{
	CLASS_DECLARATION(ToroidalCutter)

public:
	ToroidalCutter(float majorRadius, float minorRadius) : Cutter(majorRadius, minorRadius) { }

private:
	void InitBottomPart(int horizontalLvls, int roundLvls) final;
};
