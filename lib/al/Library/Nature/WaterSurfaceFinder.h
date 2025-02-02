#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;

class WaterSurfaceFinder {
public:
    WaterSurfaceFinder(const LiveActor* player);

    void update(const sead::Vector3f& position, const sead::Vector3f& gravity, f32 distance);
    void updateLocal(const sead::Vector3f& position, const sead::Vector3f& gravity, f32 maxDistance,
                     bool isFlat, bool isDisplacement, bool isOverGround);
    void updateForSurfaceShadow(const sead::Vector3f& position, const sead::Vector3f& gravity,
                                f32 distance);
    void updateForDisplacement(const sead::Vector3f& position, const sead::Vector3f& gravity,
                               f32 distance);
    void updateConsiderGround(const sead::Vector3f& position, const sead::Vector3f& gravity,
                              f32 distance);

    bool isNearSurface(f32 distance) const;

    bool isFoundSurface() const { return mIsFoundSurface; };

    f32 distance() const { return mDistance; };

    const sead::Vector3f& surfacePosition() const { return mSurfacePosition; };

private:
    const LiveActor* mActor;
    bool mIsFoundSurface = false;
    f32 mDistance = 0.0f;
    sead::Vector3f mSurfacePosition = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mSurfaceNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _28 = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(WaterSurfaceFinder) == 0x38);

}  // namespace al
