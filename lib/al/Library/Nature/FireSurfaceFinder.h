#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;

struct FireSurfaceProperties {
    f32 distance = 0.0f;
    sead::Vector3f position = {0.0f, 0.0f, 0.0f};
    sead::Vector3f normal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _1c = {0.0f, 0.0f, 0.0f};

    void setDistance(f32 value) { distance = value; }

    void setPosition(const sead::Vector3f& vector) { position.set(vector); }

    void setNormal(const sead::Vector3f& vector) { normal.set(vector); }

    void set1c(const sead::Vector3f& vector) { _1c.set(vector); }
};

class FireSurfaceFinder {
public:
    FireSurfaceFinder(const LiveActor* actor);

    void update(const sead::Vector3f& position, const sead::Vector3f& gravity, f32 distance);

private:
    const LiveActor* mActor;
    bool mIsFoundSurface = false;
    FireSurfaceProperties mSurface{};
};

static_assert(sizeof(FireSurfaceFinder) == 0x38);

}  // namespace al
