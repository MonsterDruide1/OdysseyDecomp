#include "Library/Nature/WaterSurfaceFinder.h"

#include "Library/Nature/NatureUtil.h"

namespace al {

WaterSurfaceFinder::WaterSurfaceFinder(const LiveActor* actor) : mActor(actor) {}

// NON_MATCHING: inlined updateLocal
void WaterSurfaceFinder::update(const sead::Vector3f& position, const sead::Vector3f& gravity,
                                f32 distance) {
    updateLocal(position, gravity, distance, false, false, false);
}

// NON_MATCHING: storing {0,0,0} if no surface was found (https://decomp.me/scratch/zHEdm)
void WaterSurfaceFinder::updateLocal(const sead::Vector3f& position, const sead::Vector3f& gravity,
                                     f32 distance, bool flat, bool displacement, bool overGround) {
    sead::Vector3f surfacePos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f surfaceNormal = {0.0f, 0.0f, 0.0f};
    mIsFoundSurface = false;

    if (flat)
        mIsFoundSurface = calcFindWaterSurfaceFlat(&surfacePos, &surfaceNormal, mActor, position,
                                                   gravity, distance);
    else if (displacement)
        mIsFoundSurface = calcFindWaterSurfaceDisplacement(&surfacePos, &surfaceNormal, mActor,
                                                           position, gravity, distance);
    else if (overGround)
        mIsFoundSurface = calcFindWaterSurfaceOverGround(&surfacePos, &surfaceNormal, mActor,
                                                         position, gravity, distance);
    else
        mIsFoundSurface =
            calcFindWaterSurface(&surfacePos, &surfaceNormal, mActor, position, gravity, distance);

    if (mIsFoundSurface) {
        // requires this manual dot product calculation to match
        mDistance = gravity.x * (surfacePos.x - position.x) +
                    gravity.y * (surfacePos.y - position.y) +
                    gravity.z * (surfacePos.z - position.z);
        // mDistance = gravity.dot(surfacePos - position);
        mSurfacePosition.set(surfacePos);
        mSurfaceNormal.set(surfaceNormal);
    } else {
        mDistance = 0.0f;
        mSurfacePosition = {0.0f, 0.0f, 0.0f};
        mSurfaceNormal = {0.0f, 0.0f, 0.0f};
    }

    _28 = {0.0f, 0.0f, 0.0f};
}

// NON_MATCHING: inlined updateLocal
void WaterSurfaceFinder::updateForSurfaceShadow(const sead::Vector3f& position,
                                                const sead::Vector3f& gravity, f32 distance) {
    updateLocal(position, gravity, distance, true, false, false);
}

// NON_MATCHING: inlined updateLocal
void WaterSurfaceFinder::updateForDisplacement(const sead::Vector3f& position,
                                               const sead::Vector3f& gravity, f32 distance) {
    updateLocal(position, gravity, distance, false, true, false);
}

// NON_MATCHING: inlined updateLocal
void WaterSurfaceFinder::updateConsiderGround(const sead::Vector3f& position,
                                              const sead::Vector3f& gravity, f32 distance) {
    updateLocal(position, gravity, distance, false, false, true);
}

bool WaterSurfaceFinder::isNearSurface(f32 distance) const {
    return mIsFoundSurface && sead::Mathf::abs(mDistance) < distance;
}

}  // namespace al
