#pragma once

#include <math/seadVector.h>

namespace al {
class PlacementInfo;

class GravityPoint {
public:
    GravityPoint();

    void initWithPlacementInfo(const PlacementInfo& info);
    bool tryCalcGravity(sead::Vector3f* gravity, const sead::Vector3f& position) const;

    const sead::Vector3f& getTrans() const { return mTrans; }
    f32 getRadius() const { return mRadius; }

private:
    sead::Vector3f mTrans = sead::Vector3f::zero;
    f32 mRadius = 0.0f;
};

static_assert(sizeof(GravityPoint) == 0x10);

}  // namespace al
