#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
}

class WhipTargetInfo {
public:
    WhipTargetInfo();

    void initWhipTarget(al::HitSensor* sensor, const sead::Vector3f* trans);
    void clear();
    bool isValid() const;
    void calcTargetUpDir(sead::Vector3f* upDir);
    const sead::Vector3f& getTrans() const;
};
