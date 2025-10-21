#include "Project/Gravity/GravityPoint.h"

#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {

GravityPoint::GravityPoint() = default;

void GravityPoint::initWithPlacementInfo(const PlacementInfo& info) {
    tryGetTrans(&mTrans, info);
    sead::Vector3f scale = sead::Vector3f::zero;
    tryGetScale(&scale, info);
    mRadius = scale.x * 50.0f;
}

bool GravityPoint::tryCalcGravity(sead::Vector3f* gravity, const sead::Vector3f& position) const {
    if ((mTrans - position).length() <= mRadius) {
        *gravity = (mTrans - position);
        tryNormalizeOrDirZ(gravity);
        return true;
    }
    return false;
}

}  // namespace al
