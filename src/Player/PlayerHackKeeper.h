#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

namespace al {
class LiveActor;
class HitSensor;
class CollisionPartsFilterBase;
struct ActorInitInfo;
}  // namespace al

class PlayerRecoverySafetyPoint;
class HackCap;
class PlayerInput;
class PlayerDamageKeeper;
class IPlayerModelChanger;
class IUsePlayerHeightCheck;
struct HackObjInfo;
class HackEndParam;
class PlayerCollider;
class CapTargetInfo;
class PlayerHackStartTexKeeper;
class IUsePlayerHack;

class PlayerHackKeeper {
public:
    PlayerHackKeeper(al::LiveActor* player, HackCap* cap, PlayerRecoverySafetyPoint* safetyPoint,
                     const PlayerInput* input, const sead::Matrix34f* mtx,
                     const PlayerDamageKeeper* damageKeeper,
                     const IPlayerModelChanger* modelChanger,
                     const IUsePlayerHeightCheck* heightCheck);

    void createHackModel(const al::ActorInitInfo&);
    void startHack(al::HitSensor*, al::HitSensor*, al::LiveActor*);
    void setupHack(al::HitSensor*, al::HitSensor*, al::LiveActor*);
    void endHack(const HackEndParam*);
    void endHackStartDemo(al::LiveActor*);
    void startHackStartDemo(al::LiveActor*);
    void startHackStartDemoPuppetable(al::LiveActor*);
    void addHackStartDemo(al::LiveActor*);
    void appearHackDemoModel(const sead::Matrix34f&, f32);
    void updateHackDemoModel(const sead::Matrix34f&, f32);
    void deleteHackDemoModelEffect();
    void killHackDemoModel();
    bool isActiveHackStartDemo() const;
    void recordHack();
    void cancelHackArea();
    void cancelHack();
    void cancelForceRecovery();
    void tryEscapeHack();
    void sendTransferHack();
    void sendMarioDemo();
    void forceKillHack();
    void sendMarioDead();
    void sendMarioInWater();
    void sendMarioDeathArea();
    void sendMsgEnableMapCheckPointWarp();
    void sendMsgSelfCeilingCheckMiss();
    void receiveRequestTransferHack(al::HitSensor*);
    bool requestDamage();
    void receiveRequestDamage();
    void sendSyncDamageVisibility();
    void pushWorldEndBorder(const sead::Vector3f&);
    const char* getCurrentHackName() const;
    PlayerCollider* getPlayerCollision() const;
    f32 getHackGuideHeight() const;
    bool isHackGuideEnable() const;
    f32 getHackStayGravityMargine() const;
    al::CollisionPartsFilterBase* getCollisionPartsFilter() const;
    bool isHackGroupTalkScare() const;
    bool isHackNoCollisionMsg() const;
    bool isHackNoSeparateCameraInput() const;
    bool isHackUsePlayerCollision() const;
    bool isHackCancelCeilingCheck() const;
    bool isHackInvalidLifeRecovery() const;
    void requestForceHackStageStart(al::HitSensor*, const CapTargetInfo*, al::LiveActor*);
    bool executeForceHackStageStart(al::HitSensor*, IUsePlayerHack*);
    void startDemo();
    void endDemo();

    void setPuppetable(bool isPuppetable) { mIsPuppetable = isPuppetable; }

    bool isPuppetable2() const { return mIsPuppetable2; }

    bool isHack() const { return mIsHack; }

    bool isHackDemoStarted() const { return mIsHackDemoStarted; }

    bool isStartedHacking() const { return mIsStartedHacking; }

    al::LiveActor* getHack() const { return mHackActor; }

    al::HitSensor* getHackSensor() const { return mHackHitSensor; }

private:
    al::LiveActor* mParent;
    HackCap* mHackCap;
    PlayerRecoverySafetyPoint* mRecoverySafePoint;
    void* field_18;
    void* field_20;
    PlayerInput* mInput;
    sead::Matrix34f* field_30;
    PlayerDamageKeeper* mDamageKeeper;
    IPlayerModelChanger* mModelChanger;
    IUsePlayerHeightCheck* mHeightCheck;
    al::HitSensor* mParentBodySensor;
    bool mIsPuppetable;
    bool mIsCancellingHack;
    bool mIsHackDemoStarted;
    bool mIsPuppetable2;
    bool mIsStartedHacking;
    bool mIsHack;
    bool mIsTookDamage;
    al::CollisionPartsFilterBase* mCollisionFilter;
    al::LiveActor* mHackActor;
    al::HitSensor* mHackHitSensor;
    HackObjInfo* mHackObjectInfo;
    al::HitSensor* mStageStartActorSensor;
    al::LiveActor* mStageStartActor;
    CapTargetInfo* mStageStartCapTargetInfo;
    PlayerHackStartTexKeeper* mHackStartTexKeeper;
    al::LiveActor* mHackModel;
    sead::PtrArray<sead::Matrix34f> mHackModelSlices;
    sead::Matrix34f* field_b8;
    s32 field_c0;
    s32 field_c4;
    s32 field_c8;
};

static_assert(sizeof(PlayerHackKeeper) == 0xd0);
