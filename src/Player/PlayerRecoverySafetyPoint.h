#pragma once

#include <math/seadVector.h>

class PlayerRecoverySafetyPoint {
public:
    sead::Vector3f* getSafetyPoint() const;
    sead::Vector3f* getSafetyPointGravity() const;
    sead::Vector3f* getSafetyPointArea() const;
};
