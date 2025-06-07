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
    HackerStateWingFly(al::LiveActor* actor, IUsePlayerHack** hacker,
                       IUsePlayerCollision* collision);

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

    struct HackerStateWingFlyParam {
        const char* actionFly = "Fly";
        const char* actionFall = "Fall";
        const char* actionTrample = nullptr;
        const char* actionUpperPunch = nullptr;
        f32 defaultAccel = 1.0f;
        f32 swingAccel = 1.5f;
        f32 defaultVelocityY = 15.0f;
        f32 swingVelocityY = 23.0f;
        f32 defaultFramerate = 1.6f;
        f32 swingFramerate = 2.6f;
        f32 yOvershootMax = 230.0f;
        f32 gravity = 0.6f;
        f32 turnAngle = 20.0f;
        f32 flyStartLerpTime = 0.1f;
    };

    static_assert(sizeof(HackerStateWingFlyParam) == 0x48, "HackerStateWingFlyParam Size");

private:
    IUsePlayerHack** mHacker;
    IUsePlayerCollision* mCollision;
    HackerStateWingFlyParam mParam{};
    s32 mFallTimeDelay = 0;
    f32 mAccel = 0.0f;
    f32 mVelocityY = 0.0f;
    sead::Vector3f mFlyLimit = sead::Vector3f::zero;
    bool mIsJudgeFall = false;
    f32 mFallFrame = 0;
};
