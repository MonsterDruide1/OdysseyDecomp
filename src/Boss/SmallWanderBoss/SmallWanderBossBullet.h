#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class SmallWanderBossBullet : public al::LiveActor {
public:
    SmallWanderBossBullet(const char*);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void appearAttach(const sead::Matrix34f* baseMtx, const sead::Vector3f* localTrans);
    void appearAttachParabolic(const sead::Matrix34f* baseMtx, const sead::Vector3f* localTrans);
    bool tryStartFlyParabolic(const sead::Vector3f& direction, f32 speed);
    void startLaunch();
    void exeAppearAttach();
    void exeFly();
    void resetPositionByAnim();
    void exeFlyDown();
    void exeSignExplosion();
    void exeExplosion();
};
