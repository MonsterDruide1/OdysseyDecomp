
#include "Project/Gravity/GravityHolder.h"
#include "Library/Math/MathUtil.h"
#include "Project/Gravity/GravityPoint.h"

namespace al {

GravityHolder::GravityHolder() = default;

void GravityHolder::init() {
    mPoints = new GravityPoint*[mSize];
    for (s32 i = 0; i < mSize; i++)
        mPoints[i] = nullptr;
}

void GravityHolder::createGravity(const PlacementInfo& info) {
    GravityPoint* point = new GravityPoint();
    point->initWithPlacementInfo(info);
    mPoints[mCount] = point;
    mCount++;
}

bool GravityHolder::tryCalcGravity(sead::Vector3f* gravity, const sead::Vector3f& position) const {
    sead::Vector3f total = sead::Vector3f::zero;
    bool hasGravity = false;

    for (s32 i = 0; i < mCount; i++) {
        sead::Vector3f temp = sead::Vector3f::zero;
        if (mPoints[i]->tryCalcGravity(&temp, position)) {
            total += temp;
            hasGravity = true;
        }
    }

    if (isNearZero(total))
        return false;

    tryNormalizeOrDirZ(&total);
    *gravity = total;
    return hasGravity;
}

bool GravityHolder::tryCalcBlendGravity(sead::Vector3f* gravity,
                                        const sead::Vector3f& position) const {
    if (mCount == 0)
        return false;

    sead::Vector3f total = sead::Vector3f::zero;

    for (s32 i = 0; i < mCount; i++) {
        sead::Vector3f distance = mPoints[i]->getTrans() - position;
        if (isNearZero(distance))
            continue;

        f32 blendFactor = sead::Mathf::clampMin(distance.length() - mPoints[i]->getRadius(), 1.0f);
        tryNormalizeOrZero(&distance);
        distance *= 1.0f / sead::Mathf::square(blendFactor / 1000.0f);
        total += distance;
    }

    if (isNearZero(total))
        return false;

    tryNormalizeOrDirZ(&total);
    *gravity = total;
    return true;
}

}  // namespace al
