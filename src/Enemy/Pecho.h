#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class AnimScaleController;
class HitSensor;
class SensorMsg;
}  // namespace al

class Pecho : public al::LiveActor {
public:
    Pecho(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void control() override;

    void startLiquidFast();
    void startLiquid();
    void startLiquidCap();
    void updateVelocity();
    bool tryStartFind();
    void updateVelocityEscapeWallAndFall(f32 force, f32 velocity);

    bool isEnablePush() const;
    bool isEnableAttack() const;
    bool isEnableCap() const;
    bool isEnableSendPechoSpot() const;

    void exeAppear();
    void exeAttackSuccess();
    void exeWait();
    void exeFind();
    void exeMove();
    void exeLiquidSign();
    void exeLiquidStart();
    void exeLiquid();
    void exeLiquidEnd();
    void exeReset();

private:
    al::AnimScaleController* mAnimScaleController = nullptr;
    sead::Quatf mStartingQuat = sead::Quatf::unit;
    sead::Vector3f mStartingTrans = sead::Vector3f::zero;
    sead::Quatf mNextBodyOrientation;
    sead::Vector3f mWaitTrans;
    sead::Quatf mBodyOrientation = sead::Quatf::unit;
    sead::Vector3f mMoment = sead::Vector3f::zero;
    sead::Vector3f mVelocity = sead::Vector3f::zero;

    const char* mLiquidStartAction = "LiquidStart";
    bool mIsStartLiquidFast = false;
    bool mIsWaitTiltClockwise = true;
    bool mIsBubbleReaction = false;
};
