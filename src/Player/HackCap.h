#pragma once

#include "Library/LiveActor/LiveActor.h"

class PlayerInput;
class PlayerAreaChecker;
class PlayerWallActionHistory;
class PlayerCapActionHistory;
class PlayerEyeSensorHitHolder;
class PlayerSeparateCapFlag;
class IUsePlayerCollision;
class IUsePlayerHeightCheck;
class PlayerWetControl;
class PlayerJointControlKeeper;
class HackCapJudgePreInputSeparateThrow;
class HackCapJudgePreInputSeparateJump;
class CapTargetInfo;

class HackCap : public al::LiveActor {
public:
    enum class SwingHandType { /*TODO: missing values*/ };

    HackCap(const al::LiveActor*, const char*, const PlayerInput*, const PlayerAreaChecker*,
            const PlayerWallActionHistory*, const PlayerCapActionHistory*,
            const PlayerEyeSensorHitHolder*, const PlayerSeparateCapFlag*,
            const IUsePlayerCollision*, const IUsePlayerHeightCheck*, const PlayerWetControl*,
            const PlayerJointControlKeeper*, HackCapJudgePreInputSeparateThrow*,
            HackCapJudgePreInputSeparateJump*);

    void init(const al::ActorInitInfo& info) override;
    void hide(bool);
    void movement() override;
    void updateShadowMaskOffset();
    void control() override;
    bool isFlying() const;
    void updateTargetLayout();
    void updateCollider() override;
    void updateFrameOutLayout();
    void attackSpin(al::HitSensor*, al::HitSensor*, f32);
    void prepareLockOn(al::HitSensor*);
    bool sendMsgStartHack(al::HitSensor*);
    bool receiveRequestTransferHack(al::HitSensor*, al::HitSensor*);
    void startThrowSeparatePlayHack(al::HitSensor*, const sead::Vector3f&, const sead::Vector3f&,
                                    f32);
    void startHack();
    void emitHackStartEffect();
    void noticeHackMarioEnter();
    void noticeHackDemoPuppetableEnd();
    void recordHack();
    void addHackStartDemo();
    void addLockOnKeepDemo();
    void syncHackDamageVisibility(bool);
    void endHack();
    void startSpinAttack(const char*);
    void startThrow(bool, const sead::Vector3f&, const sead::Vector3f&, f32, const sead::Vector2f&,
                    const sead::Vector2f&, const sead::Vector3f&, bool, const sead::Vector3f&,
                    SwingHandType, bool, f32, s32);
    void startThrowSeparatePlay(const sead::Vector3f&, const sead::Vector3f&, f32, bool);
    void startThrowSeparatePlayJump(const sead::Vector3f&, const sead::Vector3f&, f32);
    void startCatch(const char*, bool, const sead::Vector3f&);
    bool isNoPutOnHide() const;
    void forcePutOn();
    void forceHack(al::HitSensor*, const CapTargetInfo*);
    void resetLockOnParam();
    void setupStartLockOn();
    bool cancelCapState();
    bool isEnableThrow() const;
    bool isEnableSpinAttack() const;
    bool isSpinAttack() const;
    bool requestReturn(bool*);
    bool tryReturn(bool, bool*);
    void updateCapPose();
    void followTarget();
    void syncPuppetSilhouette();
    void recordCapJump(PlayerWallActionHistory*);
    f32 getFlyingSpeedMax() const;
    f32 getThrowSpeed() const;
    bool requestLockOnHitReaction(const CapTargetInfo*, const char*);
    void startPuppet();
    void endPuppet();
    void hidePuppetCap();
    void showPuppetCap();
    void hidePuppetCapSilhouette();
    void showPuppetCapSilhouette();
    void startPuppetCheckpointWarp();
    void startHackShineGetDemo();
    void endHackThrowAndReturnHack();
    void endHackShineGetDemo();
    void calcHackFollowTrans(sead::Vector3f*, bool) const;
    void makeFollowMtx(sead::Matrix34f*) const;
    void updateCapEyeShowHide(bool, s32);
    void activateInvincibleEffect();
    void syncInvincibleEffect(bool);
    void updateSeparateMode(const PlayerSeparateCapFlag*);
    bool isEnableRescuePlayer() const;
    bool isRescuePlayer() const;
    bool isEnableHackThrow(bool*) const;
    bool isSeparateHipDropLand() const;
    bool isSeparateHide() const;
    bool isSeparateThrowFlying() const;
    void startRescuePlayer();
    void prepareCooperateThrow();
    void requestForceFollowSeparateHide();
    f32 calcSeparateHideSpeedH(const sead::Vector3f&) const;
    void updateModelAlphaForSnapShot();
    s32 getPadRumblePort() const;
    bool isEnableThrowSeparate() const;
    bool isHoldInputKeepLockOn() const;
    bool isRequestableReturn() const;
    bool isLockOnEnableHackTarget() const;
    bool isWaitHackLockOn() const;
    bool isCatched() const;
    bool isHide() const;
    bool isPutOn() const;
    bool isLockOnInterpolate() const;
    bool isEnablePreInput() const;
    bool isForceCapTouchJump() const;
    bool isHackInvalidSeparatePlay() const;
    void exeLockOn();
    void updateThrowJoint();
    void exeHack();
    void exeSpinAttack();
    void exeCatch();
    void exeTrample();
    bool isHoldSpinCapStay() const;
    void exeTrampleLockOn();
    void exeRescue();
    void exeHide();
    void exeThrowStart();
    bool isThrowTypeSpiral() const;
    bool isThrowTypeRolling() const;
    void setupThrowStart();
    f32 getThrowHeight() const;
    bool checkEnableThrowStartSpace(sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                                    const sead::Vector3f&, f32, f32, bool, const sead::Vector3f&);
    void updateWaterArea();
    f32 getThrowRange() const;
    s32 getThrowBrakeTime() const;
    void startThrowCapEyeThrowAction();
    void exeThrow();
    bool tryCollideReflectReaction();
    bool tryCollideWallReaction();
    bool changeThrowParamInWater(s32, bool);
    void addCurveOffset();
    void exeThrowBrake();
    bool tryAppendAttack();
    void exeThrowSpiral();
    bool tryCollideWallReactionSpiral();
    void endThrowSpiral();
    void exeThrowTornado();
    bool tryCollideWallReactionReflect();
    void exeThrowRolling();
    bool tryCollideWallReactionRollingGround();
    void rollingGround();
    void exeThrowRollingBrake();
    void exeThrowStay();
    bool tryChangeSeparateThrow();
    f32 getThrowBackSpeed() const;
    void updateLavaSurfaceMove();
    bool tryCollideWallReactionStay();
    bool isEnableHackThrowAutoCatch() const;
    s32 getThrowStayTime() const;
    s32 getThrowStayTimeMax() const;
    void exeThrowAppend();
    f32 getThrowSpeedAppend() const;
    f32 getThrowRangeAppend() const;
    void exeBlow();
    bool tryCollideWallLockOn();
    void endHackThrowAndReturnHackOrHide();
    void clearThrowType();
    void exeRebound();
    void exeReturn();
    void calcReturnTargetPos(sead::Vector3f*) const;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool stayRollingOrReflect();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endMove();
    bool isEnableCapTouchJumpInput() const;
    void prepareTransferLockOn(al::HitSensor*);
    void collideThrowStartArrow(al::HitSensor*, const sead::Vector3f&, const sead::Vector3f&,
                                const sead::Vector3f&);
    bool trySendAttackCollideAndReaction(bool*);
    bool stayWallHit();
    void endHackThrow();

private:
    unsigned char _108[0x10];
    al::LiveActor* mActorA;
    unsigned char _120[0x08];
    al::LiveActor* mPlayerActor;
    void* _130[0x9c];
};

static_assert(sizeof(HackCap) == 0x610);
