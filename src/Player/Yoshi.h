#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitSensor;
class WaterSurfaceFinder;
}  // namespace al

class CapTargetInfo;
class PlayerAnimator;
class PlayerColliderYoshi;
class PlayerConst;
class PlayerEffect;
class PlayerJumpMessageRequest;
class PlayerModelChangerYoshi;
class PlayerModelHolder;
class PlayerPushReceiver;
class PlayerTrigger;
class YoshiJointControlKeeper;
class YoshiStateEgg;
class YoshiStateHack;
class YoshiStateNpc;

class Yoshi : public al::LiveActor {
public:
    Yoshi(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void movement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void control() override;
    void updateCollider() override;

    void updateCollisionShape();
    void sendCollisionMsg();
    void appearEgg();
    void startFruitShineGetDemo();
    void exeEgg();
    void exeNpc();
    void exeHack();

private:
    PlayerConst* mPlayerConst = nullptr;
    PlayerModelHolder* mModelHolder = nullptr;
    PlayerModelChangerYoshi* mModelChanger = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    PlayerColliderYoshi* mCollider = nullptr;
    PlayerEffect* mEffect = nullptr;
    PlayerTrigger* mTrigger = nullptr;
    PlayerJumpMessageRequest* mJumpMessageRequest = nullptr;
    PlayerPushReceiver* mPushReceiver = nullptr;
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    YoshiJointControlKeeper* mJointControlKeeper = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    YoshiStateEgg* mStateEgg = nullptr;
    YoshiStateNpc* mStateNpc = nullptr;
    YoshiStateHack* mStateHack = nullptr;
    al::HitSensor* mHitSensor = nullptr;
    sead::Matrix34f mLocalMtx = sead::Matrix34f::ident;
};
