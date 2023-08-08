#pragma once

#include <math/seadVector.h>

class PlayerRecoverySafetyPoint {
public:
    sead::Vector3f* getSafetyPoint(void) const;
    sead::Vector3f* getSafetyPointGravity(void) const;
    sead::Vector3f* getSafetyPointArea(void) const;
};