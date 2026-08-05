#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class EnemyCap;
class SenobiStateEnemy;
class SenobiStateHack;

namespace al {
class JointSpringControllerHolder;
}  // namespace al

class Senobi : public al::LiveActor {
public:
    Senobi(const char* actorName) : al::LiveActor(actorName) {}

    const char* getStretchJointName();
    s32 getStretchSensorNum();
    const sead::Vector3f* getStretchJointLocalOffset();

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
    void rebirth(const sead::Vector3f& trans, const sead::Vector3f& rotate);
    void resetCounter();

    void setUseResetState(bool isUseResetState) { mIsUseResetState = isUseResetState; }

    void exeEnemy();
    void exeReset();
    void exeHack();

private:
    void* mParam = nullptr;
    void* mPlayerHack = nullptr;
    void* mPlayerCollision = nullptr;
    EnemyCap* mEnemyCap = nullptr;
    SenobiStateEnemy* mStateEnemy = nullptr;
    SenobiStateHack* mStateHack = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    f32 mStretchLength = 0.0f;
    bool mIsUseResetState = false;
    bool mIsHackStartSwoon = false;
};

static_assert(sizeof(Senobi) == 0x148);
