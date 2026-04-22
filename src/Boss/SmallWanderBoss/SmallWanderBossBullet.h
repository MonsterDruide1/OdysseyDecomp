#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class ParabolicPath;
class SensorMsg;
class WaterSurfaceFinder;
}  // namespace al

class WaterSurfaceShadow;

class SmallWanderBossBullet : public al::LiveActor {
public:
    SmallWanderBossBullet(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void appearAttach(const sead::Matrix34f* attachMtx, const sead::Vector3f* attachOffset);
    void appearAttachParabolic(const sead::Matrix34f* attachMtx,
                               const sead::Vector3f* attachOffset);
    bool tryStartFlyParabolic(const sead::Vector3f& front, f32 launchSpeed);
    void startLaunch();
    void exeAppearAttach();
    void exeFly();
    void resetPositionByAnim();
    void exeFlyDown();
    void exeSignExplosion();
    void exeExplosion();

private:
    sead::Vector3f _108;  // TODO: verify exact meaning
    u32 _114;             // TODO: verify exact meaning
    const sead::Matrix34f* mAttachMtx;
    const sead::Vector3f* mAttachOffset;
    al::ParabolicPath* mParabolicPath;
    al::WaterSurfaceFinder* mWaterSurfaceFinder;
    WaterSurfaceShadow* mWaterSurfaceShadow;
    f32 mLaunchSpeed;  // TODO: verify exact usage
    s32 mExplosionStepMax;
};

static_assert(sizeof(SmallWanderBossBullet) == 0x148);
