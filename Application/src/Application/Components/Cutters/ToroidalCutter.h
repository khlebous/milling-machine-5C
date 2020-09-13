#pragma once

#include "Cutter.h"
#include <string>

namespace sm = DirectX::SimpleMath;

class ToroidalCutter : public Cutter
{
    CLASS_DECLARATION(ToroidalCutter)

public:
    ToroidalCutter(float majorRadius, float minorRadius, int horizontalLvls, int roundLvls, float height) : Cutter(majorRadius, 0.0f, horizontalLvls, roundLvls, height)
    {
        this->minorRadius = minorRadius;
        this->majorRadius = majorRadius;
        InitBottomPart(horizontalLvls, roundLvls);
    }

protected:
    void InitBottomPart(int horizontalLvls, int roundLvls) final;

private:
    float majorRadius;
    float minorRadius;

    sm::Vector3 GetTorusPoint(float majorRadius, float minorRadius, float majorRadiusAngle, float minorRadiusAngle);
};
