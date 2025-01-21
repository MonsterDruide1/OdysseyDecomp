#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
class HitSensor;
}  // namespace al

class IUsePlayerHack;
class IUsePlayerCollision;

class HackerStateWingFly : public al::ActorStateBase {
public:
    HackerStateWingFly(al::LiveActor* actor, IUsePlayerHack** hack, IUsePlayerCollision* collision);

    void appear() override;
    void goFlyRise();
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool canUpperPunch(al::HitSensor* self, al::HitSensor* other) const;
    void updateFlyLimit();
    bool judgeStart();
    void updateMove();
    bool isOnGround() const;
    bool tryUpperPunchToCollision();
    void updateFlyAction();

    void exeFlyRiseToHighest();
    void exeFlyRiseToTop();
    void exeFlyTop();
    void exeFall();
    void exeFallFly();
    void exeTrample();
    void exeUpperPunch();

private:
    IUsePlayerHack** mHacker;
    IUsePlayerCollision* mCollision;
    const char* mActionFly = "Fly";
    const char* mActionFall = "Fall";
    const char* mActionTrample = nullptr;
    const char* mActionUpperPunch = nullptr;
    f32 mDefaultAccel = 1.0f;
    f32 mSwingAccel = 1.5f;
    f32 mDefaultVelocityY = 15.0f;
    f32 mSwingVelocityY = 23.0f;
    f32 mDefaultFramerate = 1.6f;
    f32 mSwingFramerate = 2.6f;
    f32 mYLerpValue = 230.0f;
    f32 mGravityVelocity = 0.6f;
    f32 mTurnAngle = 20.0f;
    f32 mLerpTime = 0.1f;
    s32 mFallTimeDelay = 0;
    f32 mAccel = 0.0f;
    f32 mVelocityY = 0.0f;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    bool _90 = false;
    f32 mActionFrame = 0;
};
