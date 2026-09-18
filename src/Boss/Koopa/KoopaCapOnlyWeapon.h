#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class KoopaCapScaleAnimator;
class KoopaCapStateSpinThrow;
class KoopaItemHolder;
class PlayerPushReceiver;

class KoopaCapOnlyWeapon : public al::LiveActor {
public:
    KoopaCapOnlyWeapon(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void updateCollider() override;

    void disappear();
    void startWaitHover(s32 delay);
    void startSpinThrowChase(const sead::Vector3f* targetTrans, f32 degree);
    bool isEndWaitHoverStart() const;
    bool isWaitHover() const;

    void exeWaitHoverDelay();
    void exeWaitHoverStart();
    void exeWaitHover();
    void exeSpinThrow();

private:
    KoopaCapStateSpinThrow* mStateSpinThrow = nullptr;
    KoopaCapScaleAnimator* mScaleAnimator = nullptr;
    KoopaItemHolder* mItemHolder = nullptr;
    PlayerPushReceiver* mPlayerPushReceiver = nullptr;
    f32 mCapSideRotator = 0.0f;
    s32 mWaitHoverDelay = 0;
};

static_assert(sizeof(KoopaCapOnlyWeapon) == 0x130);
