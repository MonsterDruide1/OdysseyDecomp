#include "Library/Nature/WaterSurfaceFinder.h"

#include "Library/Nature/NatureUtil.h"

namespace al {

WaterSurfaceFinder::WaterSurfaceFinder(const LiveActor* actor) : mActor(actor) {}

void WaterSurfaceFinder::update(const sead::Vector3f& position, const sead::Vector3f& gravity,
                                f32 distance) {
    updateLocal(position, gravity, distance, false, false, false);
}

void WaterSurfaceFinder::updateLocal(const sead::Vector3f& position, const sead::Vector3f& gravity,
                                     f32 maxDistance, bool isFlat, bool isDisplacement,
                                     bool isOverGround) {
    sead::Vector3f surfacePos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f surfaceNormal = {0.0f, 0.0f, 0.0f};
    mIsFoundSurface = false;

    if (isFlat)
        mIsFoundSurface = calcFindWaterSurfaceFlat(&surfacePos, &surfaceNormal, mActor, position,
                                                   gravity, maxDistance);
    else if (isDisplacement)
        mIsFoundSurface = calcFindWaterSurfaceDisplacement(&surfacePos, &surfaceNormal, mActor,
                                                           position, gravity, maxDistance);
    else if (isOverGround)
        mIsFoundSurface = calcFindWaterSurfaceOverGround(&surfacePos, &surfaceNormal, mActor,
                                                         position, gravity, maxDistance);
    else
        mIsFoundSurface = calcFindWaterSurface(&surfacePos, &surfaceNormal, mActor, position,
                                               gravity, maxDistance);

    if (mIsFoundSurface) {
        // requires this manual dot product calculation to match
        mSurface.setDistance(gravity.x * (surfacePos.x - position.x) +
                             gravity.y * (surfacePos.y - position.y) +
                             gravity.z * (surfacePos.z - position.z));
        mSurface.setPosition(surfacePos);
        mSurface.setNormal(surfaceNormal);
    } else {
        mSurface.setDistance(0.0f);
        mSurface.setPosition({0.0f, 0.0f, 0.0f});
        mSurface.setNormal({0.0f, 0.0f, 0.0f});
    }

    mSurface.set1c({0.0f, 0.0f, 0.0f});
}

void WaterSurfaceFinder::updateForSurfaceShadow(const sead::Vector3f& position,
                                                const sead::Vector3f& gravity, f32 distance) {
    updateLocal(position, gravity, distance, true, false, false);
}

void WaterSurfaceFinder::updateForDisplacement(const sead::Vector3f& position,
                                               const sead::Vector3f& gravity, f32 distance) {
    updateLocal(position, gravity, distance, false, true, false);
}

void WaterSurfaceFinder::updateConsiderGround(const sead::Vector3f& position,
                                              const sead::Vector3f& gravity, f32 distance) {
    updateLocal(position, gravity, distance, false, false, true);
}

bool WaterSurfaceFinder::isNearSurface(f32 distance) const {
    return mIsFoundSurface && sead::Mathf::abs(mSurface.distance) < distance;
}

}  // namespace al
