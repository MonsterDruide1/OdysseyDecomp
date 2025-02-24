#pragma once

#include <math/seadVector.h>

namespace al {
class AreaShape;
class PlacementInfo;

class CameraTargetAreaLimitter {
public:
    CameraTargetAreaLimitter();

    static CameraTargetAreaLimitter* tryCreate(const PlacementInfo& placementInfo);

    bool applyAreaLimit(sead::Vector3f* out, const sead::Vector3f& vec);

private:
    const AreaShape* mAreaShape;
};

static_assert(sizeof(CameraTargetAreaLimitter) == 0x8);
}  // namespace al
