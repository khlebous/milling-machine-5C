#include "FlatCutter.h"

CLASS_DEFINITION(Cutter, FlatCutter)

FlatCutter::FlatCutter(float radius, int horizontalLvls, int roundLvls, float height) : Cutter(radius, 0, horizontalLvls, roundLvls, height)
{
	InitUpperPart(horizontalLvls, 0, height);
}
