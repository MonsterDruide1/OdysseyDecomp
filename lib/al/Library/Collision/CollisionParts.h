#pragma once

#include <math/seadVector.h>

namespace al {

class CollisionParts {
public:
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
};

}  // namespace al
