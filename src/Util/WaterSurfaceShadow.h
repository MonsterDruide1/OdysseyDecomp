#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ActorInitInfo;
class LiveActor;
}  // namespace al

class WaterSurfaceFinder;

class WaterSurfaceShadow {
public:
    WaterSurfaceShadow(const al::ActorInitInfo&, const char*);
    bool isAppearShadow() const;
    void update(const sead::Vector3f&, const sead::Vector3f&, f32);
    void disappearShadow();
    void setScale(f32);
    void setScale(f32, f32);
    void setQuat(const sead::Quatf&);

private:
    al::LiveActor* mActor = nullptr;
    WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    sead::Vector3f mScale = {500.0f, 0.25f, 1.0f};
};
