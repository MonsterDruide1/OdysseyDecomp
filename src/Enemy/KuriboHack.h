#pragma once

#include <container/seadOffsetList.h>

#include "Library/LiveActor/LiveActor.h"

class CapTargetInfo;
class EnemyStateSwoon;
class EnemyStateReset;
class EnemyStateWander;
class ActorStateSandGeyser;
class KuriboStateHack;
class EnemyStateBlowDown;
class EnemyCap;
class CollisionPartsFilterBase;
class PlayerPushReceiver;
class CollisionMultiShape;
class CollisionShapeKeeper;
class PlayerCeilingCheck;
class DisregardReceiver;

namespace al {
class WaterSurfaceFinder;
class JointSpringControllerHolder;

}  // namespace al

class KuriboHack : public al::LiveActor {
public:
    KuriboHack(const char*);
    void init(const al::ActorInitInfo&) override;
    void initAfterPlacement() override;
    void appear() override;
    void makeActorAlive() override;
    void kill() override;
    void makeActorDead() override;
    void movement() override;
    void calcAnim() override;
    void startClipped() override;
    void endClipped() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void updateCollider() override;

    void tryCreateEnemyCap(const al::ActorInitInfo&);
    void setNerveRideOnCommon();
    void resetRideOnPosBottomWithDefaultParam();
    void onSnapShotMode();
    void offSnapShotMode();
    void onDynamics();
    void detach(KuriboHack*);
    bool checkSandSinkPrecisely() const;
    void solveCollisionInHacking(const sead::Vector3f&);
    void pushFrom(KuriboHack*, const sead::Vector3f&);
    void updateCapLockOnMtx();
    void forceStartClipped();
    void appearFall();
    void noRevive();
    void exeWait();
    void setShiftTypeOnGround(s32);
    void offDynamics();
    void syncRideOnPosBottomWithDefaultParam();
    void exeWander();
    bool tryShiftDrown();
    void exeTurn();
    void exeFind();
    void exeChase();
    void exeStop();
    void exeAttack();
    void exePressDown();
    void exeBlowDown();
    void exeDamageCap();
    void exeFall();
    void exeLand();
    bool tryShiftChaseOrWander();
    void exeSink();
    bool updateSink();
    void exeSlide();
    void exeReset();
    void exeSandGeyser();
    void clearSink();
    void exeWaitHack();
    void exeTowerHackEnd();
    void exeHack();
    void invalidateHackDamage();
    void shiftWaitHack();
    void endHack();
    void exeRideOn();
    void endRideOn();
    void exeDrown();
    void exeEatBind();
    void prepareKillByShineGet();
    bool tryReceiveMsgHack(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool tryReceiveMsgWaitHack(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool tryReceiveMsgRideOn(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool tryReceiveMsgEatBind(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool tryReceiveMsgNormal(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void transferGroup(sead::OffsetList<KuriboHack>*);
    void eraseFromHost();
    void notifyJumpSink(f32);
    bool isSinking() const;
    void syncRideOnPosBottom(f32, f32);
    void resetRideOnPosBottom(f32);
    void validateSpecialPush(u32);
    void startRideOnRotation();
    void applyRideOnQuat(const sead::Quatf&);
    bool isInvalidHackDamage() const;
    void validateHipDropProbe(al::HitSensor*);
    s32 indexInHostList() const;
    f32 getRideOnRowSize();
    bool isCapWorn() const;
    bool isEnableHack() const;
    void trySetHipDropActor(const al::SensorMsg*, al::HitSensor*);
    void addCapToHackDemo();
    bool tryReceiveMsgPush(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool tryRideOnHack(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void notifyKillByShineGetToGroup(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);

private:
    CapTargetInfo* mCapTargetInfo;
    EnemyStateSwoon* mStateSwoon;
    EnemyStateReset* mStateReset;
    EnemyStateWander* mStateWander;
    ActorStateSandGeyser* mStateSandGeyser;
    KuriboStateHack* mStateHack;
    EnemyStateBlowDown* mStateBlowDown;
    al::JointSpringControllerHolder* mJointSpringControllerHolder;
    bool mIsKuriboActive;  // really abstract name... only false when kuribo is swooning, chase end,
                           // and right after damage taken
    EnemyCap* mEnemyCap;
    bool mIsEyebrowOff;
    KuriboHack* mKuriboTowerBottom;
    bool mIsGold;
    s32 mChaseEndTimer;  // name unsure
    CollisionPartsFilterBase* mCollisionPartsFilterBase;
    sead::Vector3f mNextKuriboTrans;  // position of the goomba directly above in a tower
    f32 mClippingRadius;
    PlayerPushReceiver* mPlayerPushReceiver;
    CollisionMultiShape* mCollisionMultiShape;
    CollisionShapeKeeper* mCollisionShapeKeeper;
    f32 mSandSinkJumpHeight;
    s32 mKuriboTowerIdx;  // name unsure
    s32 mStartRideTimer;
    s32 _1ac;
    u32 mHackEndTimer;
    s32 mShiftType;
    s32 mCapCancelLockOnTimer;
    f32 mLastHackEndPosY;
    PlayerCeilingCheck* mPlayerCeilingCheck;
    bool mIsHackJustEnded;
    sead::Vector3f mHackEndPos;
    s32 mHipDropTimer;
    const char* mJointName;
    sead::Matrix34f mCapLockOnMtx;
    sead::Matrix34f mInitMtx;
    al::LiveActor* mSensorHost;
    sead::Vector3f mHostDistance;
    al::HitSensor* mHipDropProbe;
    sead::Vector3f mHipDropProbeTrans;
    al::WaterSurfaceFinder* mWaterSurfaceFinder;
    sead::Matrix34f mWaterSurfaceEffectMtx;
    sead::Matrix34f mSandSurfaceEffectMtx;
    bool _2e0;  // set true in ctor, only ever checked if false
    sead::OffsetList<KuriboHack> mKuriboTowerOffsetList;
    sead::ListNode mKuriboTowerList;
    DisregardReceiver* mDisregardReceiver;
    u32 field_318;
};

static_assert(sizeof(KuriboHack) == 0x320);
