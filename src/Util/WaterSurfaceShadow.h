#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al

class WaterSurfaceShadow {
public:
    WaterSurfaceShadow(const al::ActorInitInfo& initInfo, const char* archiveName);
    bool isAppearShadow() const;
    void update(const sead::Vector3f& position, const sead::Vector3f& gravity, f32 distance);
    void disappearShadow();
    void setScale(f32 scale);
    void setScale(f32 scaleX, f32 scaleZ);
    void setQuat(const sead::Quatf& quat);

private:
    al::LiveActor* mActor = nullptr;
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    f32 mMaxDistance = 500.0f;
    f32 mEndFrame = 0.25f;
    f32 mStartFrame = 1.0f;
};
