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
    void startRefract(s32 delay);
    void endRefract(s32 delay);

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
    IUsePlayerHack* somePlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    EnemyStateSwoon* mEnemyStateSwoon = nullptr;
    GamaneHackState* mGamaneHackState = nullptr;
    al::EnemyStateBlowDown* mEnemyStateBlowDown = nullptr;
    EnemyStateHackStart* mEnemyStateHackStart = nullptr;
    HackerJudgeNormalFall* mHackerJudgeNormalFall = nullptr;
    al::CollisionPartsFilterSpecialPurpose* mCollisionPartsFilter = nullptr;

    bool mIsInLove = false;
    int mCoinDelay = 30;
    int mHackDelay = 0;
    int mMaterialIndex = 0;
    int mRefractDelayTransisionTime = 0;
    int mRefractDelay = 0;
    bool mIsStartRefract = true;
    float mShadowMaskIntensity = 0.2;
    HackerDepthShadowMapCtrl* mHackerDepthShadowMapCtrl = nullptr;
};

void initializeGamaneGlobals();
