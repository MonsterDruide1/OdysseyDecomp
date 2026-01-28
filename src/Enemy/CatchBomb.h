#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class CollisionPartsFilterSpecialPurpose;
class HitSensor;
class JointSpringControllerHolder;
class SensorMsg;
}  // namespace al
class CapTargetInfo;
class DisregardReceiver;
class LoopAnimState;

class CatchBomb : public al::LiveActor {
public:
    CatchBomb(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool isEnableExplosion() const;
    bool tryShiftExplode(bool unused);
    bool isEnableEnemyAttack() const;
    void tryShiftAttack();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool tryReceiveMsgPushLocal(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self);
    bool isCapReflectNerve() const;
    bool isUncapNerve() const;
    void exeWait();
    void calcGravity();
    void calcBrake();
    bool tryShiftFall();
    bool tryShiftDamageCollisionExplode();
    void exeFind();
    bool tryShiftWait();
    void exeMove();
    void exeTurn();
    void exeFall();
    void exeBack();
    void exeAttack();
    void exeCatch();
    void requestCapHitReaction(const char* reactionName, bool unused);
    void exePull();
    bool checkCollisionExplode(const sead::Vector3f& dir);
    void exeMarch();
    void exeExplode();
    void exeHide();
    void exeAppear();
    void exeThrow();
    void initThrow(const al::ActorInitInfo& info);
    void appearThrow(const sead::Vector3f& pos, const sead::Vector3f& vel);
    void control() override;

private:
    LoopAnimState* mCatchState = nullptr;
    al::LiveActor* mCap = nullptr;
    sead::Vector3f _118 = sead::Vector3f::zero;
    bool mIsRevival = false;
    sead::Vector3f mSpawnPos = {0.0f, 0.0f, 0.0f};
    s32 mExplodeTimer = 0;
    CapTargetInfo* mCapTargetInfo = nullptr;
    al::CollisionPartsFilterSpecialPurpose* mCollisionPartsFilter = nullptr;
    u32 mPlayerFarCounter = 0;
    bool mIsFirstWait = true;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    sead::Vector3f mTurnTarget;
    DisregardReceiver* mDisregardReceiver = nullptr;
    al::LiveActor* mCatchBombGeneratePoint = nullptr;
};

static_assert(sizeof(CatchBomb) == 0x178);
