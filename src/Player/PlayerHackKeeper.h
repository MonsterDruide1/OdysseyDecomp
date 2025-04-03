#pragma once

#include <math/seadMatrix.h>

namespace al {
class LiveActor;
class HitSensor;
struct ActorInitInfo;
}  // namespace al

class PlayerRecoverySafetyPoint;
class HackCap;
class PlayerInput;
class PlayerDamageKeeper;
class IPlayerModelChanger;
class IUsePlayerHeightCheck;
class HackEndParam;
class PlayerCollider;
class CapTargetInfo;
class IUsePlayerHack;

class PlayerHackKeeper {
public:
    PlayerHackKeeper(al::LiveActor* player, HackCap* cap, PlayerRecoverySafetyPoint* safetyPoint,
                     const PlayerInput* input, const sead::Matrix34f* mtx,
                     PlayerDamageKeeper* damageKeeper, const IPlayerModelChanger* modelChanger,
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
    void requestDamage();
    void receiveRequestDamage();
    void sendSyncDamageVisibility();
    void pushWorldEndBorder(const sead::Vector3f&);
    const char* getCurrentHackName() const;
    PlayerCollider* getPlayerCollision();
    f32 getHackGuideHeight();
    bool isHackGuideEnable() const;
    f32 getHackStayGravityMargine();
    void* getCollisionPartsFilter();
    bool isHackGroupTalkScare() const;
    bool isHackNoCollisionMsg() const;
    bool isHackNoSeparateCameraInput() const;
    bool isHackUsePlayerCollision() const;
    bool isHackCancelCeilingCheck() const;
    bool isHackInvalidLifeRecovery() const;
    void requestForceHackStageStart(al::HitSensor*, const CapTargetInfo*, al::LiveActor*);
    void executeForceHackStageStart(al::HitSensor*, IUsePlayerHack*);
    void startDemo();
    void endDemo();

    al::LiveActor* getCurrentHackActor() const { return mCurrentHackActor; }

    al::HitSensor* getUnkHitSensor() const { return mUnkHitSensor; }

private:
    char padding[0x68];
    al::LiveActor* mCurrentHackActor;
    al::HitSensor* mUnkHitSensor;
    // at 0x98 PlayerHackStartTexKeeper
};
