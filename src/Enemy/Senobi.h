#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class JointSpringControllerHolder;
class SensorMsg;
}  // namespace al

class CapTargetInfo;
class EnemyCap;
class IUsePlayerHack;
class SenobiStateEnemy;
class SenobiStateHack;

class Senobi : public al::LiveActor {
public:
    using LiveActor::LiveActor;

    const char* getStretchJointName();
    s32 getStretchSensorNum();
    const sead::Vector3f& getStretchJointLocalOffset();
    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void makeActorDead() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void movement() override;
    void calcAnim() override;
    void updateCollider() override;
    f32 getStretchRate() const;
    void rebirth(const sead::Vector3f& trans, const sead::Vector3f& front);
    void resetCounter();
    void exeEnemy();
    void exeReset();
    void exeHack();

    void setBossGenerated(bool val) { mIsBossGenerated = val; }

private:
    void* mLeafParamHolder = nullptr;
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    EnemyCap* mEnemyCap = nullptr;
    SenobiStateEnemy* mStateEnemy = nullptr;
    SenobiStateHack* mStateHack = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    f32 mStretchLength = 0.0f;
    u8 _144 = 0;
    bool mIsBossGenerated = false;
    u8 _146[0x2];
};

static_assert(sizeof(Senobi) == 0x148);
