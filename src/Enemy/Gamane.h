#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class CollisionPartsFilterSpecialPurpose;
class EnemyStateBlowDown;
class HitSensor;
class SensorMsg;
}  // namespace al

class CapTargetInfo;
class EnemyStateSwoon;
class EnemyStateHackStart;
class GamaneHackState;
class HackerJudgeNormalFall;
class HackerDepthShadowMapCtrl;
class IUsePlayerHack;

class Gamane : public al::LiveActor {
public:
    Gamane(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* target, al::HitSensor* source) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;
    void control() override;
    void endClipped() override;
    void updateCollider() override;

    void startHack(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target);
    void updateRefract();
    void updateMovement();
    void startRefract(s32 transitionTime);
    void endRefract(s32 transitionTime);

    void exeWait();
    void exeFind();
    void exeRunaway();
    void exeFall();
    void exeLand();
    void exeSwoon();
    void exeHackStart();
    void exeHack();
    void exeTrampled();
    void exePressDown();
    void exeBlowDown();

    CapTargetInfo* getCapTargetInfo() { return mCapTargetInfo; }

private:
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    EnemyStateSwoon* mEnemyStateSwoon = nullptr;
    GamaneHackState* mHackState = nullptr;
    al::EnemyStateBlowDown* mStateBlowDown = nullptr;
    EnemyStateHackStart* mStateHackStart = nullptr;
    HackerJudgeNormalFall* mJudgeNormalFall = nullptr;
    al::CollisionPartsFilterSpecialPurpose* mCollisionPartsFilter = nullptr;

    bool mIsInLove = false;
    s32 mCoinDelay = 30;
    s32 mHackDelay = 0;
    s32 mMaterialIndex = 0;
    s32 mRefractTransitionTime = 0;
    s32 mRefractDelay = 0;
    bool mIsStartRefract = true;
    f32 mShadowMaskIntensity = 0.2f;
    HackerDepthShadowMapCtrl* mDepthShadowMapCtrl = nullptr;
};
