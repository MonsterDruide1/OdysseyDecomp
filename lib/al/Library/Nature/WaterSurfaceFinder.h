#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;

struct WaterSurfaceProperties {
    f32 distance = 0.0f;
    sead::Vector3f position = {0.0f, 0.0f, 0.0f};
    sead::Vector3f normal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _1c = {0.0f, 0.0f, 0.0f};

    void setDistance(f32 value) { distance = value; }

    void setPosition(const sead::Vector3f& vector) { position.set(vector); }

    void setNormal(const sead::Vector3f& vector) { normal.set(vector); }

    void set1c(const sead::Vector3f& vector) { _1c.set(vector); }
};

class WaterSurfaceFinder {
public:
    WaterSurfaceFinder(const LiveActor* actor);

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

    f32 getDistance() const { return mSurface.distance; };

    const sead::Vector3f& getSurfacePosition() const { return mSurface.position; };

    const sead::Vector3f& getSurfaceNormal() const { return mSurface.normal; };

private:
    const LiveActor* mActor;
    bool mIsFoundSurface = false;
    WaterSurfaceProperties mSurface{};
};

static_assert(sizeof(WaterSurfaceFinder) == 0x38);

}  // namespace al
