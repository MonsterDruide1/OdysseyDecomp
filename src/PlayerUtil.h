#pragma once

#include "Library/Collision/Collider.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Player/IJudge.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerActorHakoniwa.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerEquipmentUser.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerPainPartsKeeper.h"
#include "Util/IUseDimension.h"
#include "container/seadPtrArray.h"
#include "math/seadQuat.h"
#include "prim/seadSafeString.h"

namespace al {
class CameraTicket;
}

namespace PlayerEquipmentFunction {
bool tryGetEquipmentForceDashInfo(int *,float *,PlayerEquipmentUser const*);
bool isEquipmentNoCapThrow(PlayerEquipmentUser const*);
}

namespace rs {

void initPlayerActorInfo(PlayerActorBase *,PlayerInitInfo const&);
const char* getInitPlayerModelName(PlayerInitInfo const&);
const char* getInitCapTypeName(PlayerInitInfo const&);
bool isClosetScenePlayer(PlayerInitInfo const&);
bool isNeedCreateNoseNeedle(PlayerInitInfo const&);
void resetCollision(IUsePlayerCollision *);
bool isOnGroundRunAngle(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*);
bool isOnGround(al::LiveActor const*,IUsePlayerCollision const*);
bool isJustLand(IUsePlayerCollision const*);
void cutVerticalVelocityGroundNormal(al::LiveActor *,IUsePlayerCollision const*);
bool calcSlideDir(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&);
void calcGroundNormalOrGravityDir(sead::Vector3<float> *,al::LiveActor const*,IUsePlayerCollision const*);
void startHitReactionLandJumpIfLanding(al::LiveActor const*,IUsePlayerCollision const*,bool);
void startHitReactionLandIfLanding(al::LiveActor const*,IUsePlayerCollision const*,bool);
void noticePlayerJumpStart(PlayerTrigger *,al::LiveActor const*);
bool isPlayerDamageStopDemo(al::LiveActor const*);
bool isKidsMode(const al::LiveActor*);
bool isTouchJumpCode(al::LiveActor const*,IUsePlayerCollision const*);
bool isActiveDemo(al::LiveActor const*);
bool isPressedCollision(IUsePlayerCollision const*);
void resetCollisionPose(IUsePlayerCollision const*,sead::Quat<float> const&);
void calcSnapVelocitySnapMoveArea(sead::Vector3<float> *,al::LiveActor const*,IUsePlayerCollision const*,sead::Vector3<float> const&,float);
void calcSnapVelocitySnapMoveAreaWithCutDir(sead::Vector3<float> *,al::LiveActor const*,IUsePlayerCollision const*,sead::Vector3<float> const&,float,sead::Vector3<float> const&);
bool isCollidedCeiling(IUsePlayerCollision const*);
void reflectCeiling(al::LiveActor *,float);
bool isOnGroundAndGravity(al::LiveActor const*,IUsePlayerCollision const*);
void calcGroundNormalExceptJustLandOrGravityDir(sead::Vector3<float> *,al::LiveActor const*,IUsePlayerCollision const*);
bool isOnGroundSkateCode(al::LiveActor const*,IUsePlayerCollision const*);
bool findWallEnablePush(al::LiveActor const*,IUsePlayerCollision const*,float,float);
bool isCollidedWallFace(IUsePlayerCollision const*);
const al::CollisionParts* getCollidedGroundCollisionParts(IUsePlayerCollision const*);
const sead::Vector3f& getCollidedCeilingNormal(IUsePlayerCollision const*);
void reflectCeilingUpperPunch(al::LiveActor *,IUsePlayerCollision const*,PlayerInput const*,PlayerConst const*,PlayerTrigger const*,bool);
void setupLongJumpVelocity(al::LiveActor *,IUsePlayerCollision const*,float,float,float,float,float);
bool isModeE3Rom();
bool isModeE3MovieRom();
void calcFrontVelocityAndDirH(sead::Vector3<float> *,sead::Vector3<float> *,al::LiveActor const*,IUsePlayerCollision const*);
bool isCollidedGroundOverAngle(const al::LiveActor*,IUsePlayerCollision const*,float);
bool isCollidedGroundLessAngle(const al::LiveActor*,IUsePlayerCollision const*,float);
bool calcAlongDirFront(sead::Vector3<float> *,al::LiveActor const*,sead::Vector3<float> const&);
void addPlayerThrowCapCount(const al::LiveActor*);
void moveInertiaTurn(sead::Vector3<float> *,sead::Quat<float> *,al::LiveActor *,IUsePlayerCollision const*,sead::Vector3<float> const&,float,float,float,float,float);

}

class CapFunction {
public:
    static void putOnCapPlayer(HackCap *,PlayerAnimator *);

};

#include "Player/PlayerModelHolder.h"

class PlayerJointControlKeeper {
public:
    PlayerJointControlKeeper(al::LiveActor *,PlayerConst const*,bool,bool,bool);
    void resetPartsDynamics();
    void calcGroundPoseUp(sead::Vector3<float> *) const;

public:
    void *gap[24];
    sead::Matrix34f* mtx;
    void *size_f0[5];
};

class PlayerCeilingCheck;
class CollisionShapeKeeper;

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerModelHolder.h"

#include "Player/PlayerAnimator.h"

class PlayerEffect {
public:
    PlayerEffect(al::LiveActor *,PlayerModelHolder const*,sead::Matrix34<float> const*);
    void updateWaterSurfaceMtx(al::WaterSurfaceFinder const*);
    void tryEmitRollingEffect();
    void tryDeleteRollingEffect();
private:
    void* size[0xC0/8];
};

class PlayerAnimControlRun {
public:
    PlayerAnimControlRun(PlayerAnimator *,PlayerConst const*,IJudge const*,PlayerEffect *,bool);
private:
    void* size[0x40/8];
};

#include "Player/PlayerContinuousJump.h"
#include "Player/PlayerContinuousLongJump.h"

class PlayerDamageKeeper {
public:
    PlayerDamageKeeper(al::LiveActor *,PlayerEffect *);

    void update(IPlayerModelChanger *,bool,bool);
public:
    al::LiveActor *mPlayerActor;
    PlayerEffect *mPlayerEffect;
    bool _10;
    int _14;
    void *gap[3];
};

class PlayerDemoActionFlag {
public:
    PlayerDemoActionFlag();
private:
    void* size[0x18/8];
};

class PlayerCounterAfterCapCatch;
#include "Player/PlayerCapActionHistory.h"

class PlayerAreaChecker {
public:
    PlayerAreaChecker(al::LiveActor const*,PlayerModelHolder const*);
private:
    void* size[0x10/8];
};

namespace al {
    class AreaObj;
}
class WorldEndBorderKeeper : public al::NerveExecutor {
public:
    WorldEndBorderKeeper(al::LiveActor const*);
    void update(sead::Vector3<float> const&,sead::Vector3<float> const&,bool);
public:
    const al::LiveActor *mPlayer;
    sead::Vector3f _18;
    sead::Vector3f _24;
    bool mIsSwimOrInAir;
    bool pad[7];
    al::AreaObj *mWorldEndBorderArea;
    sead::Vector3f _40;
    float _4c;
    float _50;
    sead::Vector3f _54 = {0.0f, 0.0f, 0.0f};
};

#include "Player/PlayerWallActionHistory.h"

#include "Player/PlayerCounterAfterUpperPunch.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerCounterQuickTurnJump.h"

#include "Player/PlayerJumpMessageRequest.h"

class PlayerSandSinkAffect {
public:
    PlayerSandSinkAffect(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision *,PlayerEffect *);
    bool isSinkDeathHeight();
    bool isEnableCapThrow() { return true; }
private:
    void* size[0x38/8];
};

class PlayerEyeSensorHitHolder {
public:
    PlayerEyeSensorHitHolder(int);
    void createTargetMarkerBuffer(int);
    void clear();
private:
    void* size[0x48/8];
};

class PlayerPushReceiver {
public:
    PlayerPushReceiver(al::LiveActor* const);
    void clear();
    void calcPushedVelocityWithCollide(sead::Vector3<float> *,sead::Vector3<float> const&,IUsePlayerCollision const*,float);
private:
    void* size[0x38/8];
};

class PlayerHitPush {
public:
    PlayerHitPush(al::LiveActor* const, PlayerConst* const);
    void clearHitFlag();
public:
    al::LiveActor *mPlayer;
    const PlayerConst *mConst;
    bool mIsPush = false;
    bool _11 = false;
    sead::Vector3f mPush = {0.0f, 0.0f, 0.0f};
};

#include "Player/PlayerExternalVelocity.h"

class PlayerWetControl {
public:
    PlayerWetControl(al::LiveActor const*,al::LiveActor*,PlayerAreaChecker const*);
    void recordPuddleRolling();
public:
    void *gap[1];
    PlayerEffect *mEffect;
    void *size[5];
};

class PlayerSeparateCapFlag {
public:
    PlayerSeparateCapFlag();

    bool someCheck() const {
        return (*((int*)this) & 0xFF0000) != 0 || *((u16*)this) == 0;
    }
public:
    bool _0;
    bool _1;
    bool _2;
    sead::Vector3f _4;
};

class HackCapJudgePreInputSeparateThrow : public IJudge {
public:
    HackCapJudgePreInputSeparateThrow(PlayerInput const*,PlayerSeparateCapFlag const*);
    
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x20/8];
};

#include "Player/HackCapJudgePreInputSeparateJump.h"

class PlayerRippleGenerator {
public:
    PlayerRippleGenerator(al::LiveActor const*,al::LiveActor const*,PlayerModelHolder const*);
    void reset();
private:
    void* size[0x58/8];
};

#include "Player/PlayerRecoverySafetyPoint.h"

class IUsePlayerPuppet {};
class PlayerPuppet : public IUsePlayerPuppet {
public:
    PlayerPuppet(al::LiveActor *,HackCap *,PlayerAnimator *,IUsePlayerCollision *,ActorDimensionKeeper *,IPlayerModelChanger *,WorldEndBorderKeeper *,PlayerCounterForceRun *,PlayerDamageKeeper *,PlayerEffect *,PlayerInput const*,PlayerConst const*);
    bool isNoCollide();
private:
    void* size[0xD0/8];
};

class PlayerBindableSensorList;
class PlayerBindKeeper {
public:
    PlayerBindKeeper(al::HitSensor *,IUsePlayerPuppet *);
    void clearBindableSensor();
    bool sendStartMsg();
public:
    al::HitSensor *mBodyHitSensor = nullptr;
    al::HitSensor *_8 = nullptr;
    PlayerBindableSensorList *mBindableSensorList = nullptr;
    IUsePlayerPuppet *mPuppet;
    int _20 = 0;
    bool _24 = false;
};
static_assert(sizeof(PlayerBindKeeper) == 0x28);

class PlayerJointParamHandLegAngle;
class IUsePlayerCeilingCheck;
#include "Player/PlayerCarryKeeper.h"

class PlayerSeCtrl {
public:
    PlayerSeCtrl(al::LiveActor const*,PlayerAnimator const*,HackCap const*,PlayerModelChangerHakoniwa const*,al::LiveActor const*,PlayerExternalVelocity const*);
    void update();
public:
    void *size1[5];
    sead::FixedSafeString<256> _28;
    void *size2[6];
    float _170;
    float _174;
    void *size3[4];
};

#include "Player/PlayerHackKeeper.h"

class PlayerFormSensorCollisionArranger {
public:
    PlayerFormSensorCollisionArranger(al::LiveActor *,PlayerColliderHakoniwa *,IPlayerModelChanger const*,PlayerHackKeeper const*);
private:
    void* size[0x40/8];
};

class PlayerCapManHeroEyesControl {
public:
    PlayerCapManHeroEyesControl(char const*,al::LiveActor *,al::LiveActor *);
private:
    void* size[0x20/8];
};

class PlayerJudgeAirForceCount : public IJudge {
public:
    PlayerJudgeAirForceCount(al::LiveActor const*,PlayerExternalVelocity const*,IUsePlayerCollision const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x28/8];
};

class PlayerCounterAfterCapCatch;
class PlayerJudgeCapCatchPop : public IJudge {
public:
    PlayerJudgeCapCatchPop(al::LiveActor const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCapActionHistory const*,PlayerCounterAfterCapCatch const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x38/8];
};

#include "Player/PlayerJudgeDeadWipeStart.h"

class PlayerJudgeDirectRolling : public IJudge {
public:
    PlayerJudgeDirectRolling(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerCounterForceRun const*);
    
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x48/8];
};


#include "Player/PlayerJudgeEnableStandUp.h"
#include "Player/PlayerJudgeForceSlopeSlide.h"
#include "Player/PlayerJudgeForceRolling.h"

class PlayerJudgeGrabCeil : public al::HioNode, public IJudge {
public:
    PlayerJudgeGrabCeil(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerExternalVelocity const*);
    
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
public:
    const al::LiveActor *mPlayer;
    const PlayerConst *mConst;
    const IUsePlayerCollision *mCollision;
    const IPlayerModelChanger *mModelChanger;
    const PlayerCarryKeeper *mCarryKeeper;
    const PlayerExternalVelocity *mExternalVelocity;
    bool mIsJudge;
    const al::CollisionParts *_40;
    sead::Vector3f _48;
    sead::Vector3f _54;
    sead::Vector3f _60;
};

class PlayerJudgeInWater : public IJudge {
public:
    PlayerJudgeInWater(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*,PlayerAreaChecker const*,al::WaterSurfaceFinder const*,IUsePlayerHeightCheck const*,PlayerCounterForceRun const*,bool,bool,bool);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x48/8];
};

#include "Player/PlayerJudgeInvalidateInputFall.h"

#include "Player/PlayerModelChangerHakoniwa.h"

class IUsePlayerFallDistanceCheck;
class PlayerJudgeLongFall : public IJudge {
public:
    PlayerJudgeLongFall(al::LiveActor const*,PlayerConst const*,IUsePlayerFallDistanceCheck const*,PlayerHackKeeper const*,PlayerBindKeeper const*,IPlayerModelChanger const*);
    
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x38/8];
};

class PlayerJudgePoleClimb : public al::HioNode, public IJudge {
public:
    PlayerJudgePoleClimb(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerExternalVelocity const*,PlayerInput const*,PlayerTrigger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
public:
    const al::LiveActor *mPlayer;
    const PlayerConst *mConst;
    const IUsePlayerCollision *mCollision;
    const IPlayerModelChanger *mModelChanger;
    const PlayerCarryKeeper *mCarryKeeper;
    const PlayerExternalVelocity *mExternalVelocity;
    const PlayerInput *mInput;
    const PlayerTrigger *mTrigger;
    bool mIsJudge = 0;
    const al::CollisionParts *mCollisionParts = 0;
    sead::Vector3f _58 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _64 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _70 = {0.0f, 0.0f, 0.0f};
    float _7C = 0;
    float mPoleCodeAngleOffsetCeiling = 0;
    const char *mMaterialCodeCeiling = 0;
};
static_assert(sizeof(PlayerJudgePoleClimb) == 0x90);

#include "Player/PlayerJudgePreInputCapThrow.h"

class PlayerJudgePreInputHackAction : public IJudge {
public:
    PlayerJudgePreInputHackAction(PlayerConst const*,PlayerInput const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x20/8];
};

class HackCapJudgePreInputHoveringJump : public IJudge {
public:
    HackCapJudgePreInputHoveringJump(al::LiveActor const*,IUsePlayerCollision const*,HackCap const*,PlayerInput const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x30/8];
};

class PlayerJudgeSandSink : public IJudge {
public:
    PlayerJudgeSandSink(IUsePlayerCollision const*,PlayerSandSinkAffect const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x18/8];
};

class PlayerJudgeSlopeSlide : public IJudge {
public:
    PlayerJudgeSlopeSlide(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x38/8];
};

#include "Player/PlayerJudgeStartHipDrop.h"

class PlayerJudgeStartRise : public IJudge {
public:
    PlayerJudgeStartRise(al::LiveActor const*,PlayerAreaChecker const*,IPlayerModelChanger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x20/8];
};

#include "Player/PlayerJudgeStartRolling.h"
#include "Player/PlayerJudgeStartSquat.h"
#include "Player/PlayerJudgeWallCatch.h"

namespace al {class CollisionPartsConnector;}
class PlayerJudgeWallCatchInputDir : public al::HioNode, public IJudge {
public:
    PlayerJudgeWallCatchInputDir(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerExternalVelocity const*,PlayerCounterForceRun const*,IUsePlayerHeightCheck const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }

    void validateFallJudge();
    void updateWallCatchEnviroment();
public:
    const al::LiveActor *mPlayer;
    const PlayerConst *mConst;
    const PlayerInput *mInput;
    const IUsePlayerCollision *mCollision;
    const IPlayerModelChanger *mModelChanger;
    const PlayerCarryKeeper *mCarryKeeper;
    const PlayerExternalVelocity *mExternalVelocity;
    const PlayerCounterForceRun *mCounterForceRun;
    const IUsePlayerHeightCheck *mHeightCheck;
    bool mIsJudge;
    const al::CollisionParts *_58;
    sead::Vector3f _60[3];
    int _84;
    int pad[2];
    al::CollisionPartsConnector *mCollisionPartsConnector;
    sead::Vector3f _98;
    int _A4;
};

class PlayerJudgeWallHitDown : public IJudge {
public:
    PlayerJudgeWallHitDown(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,PlayerTrigger const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x28/8];
};

class PlayerJudgeWallHitDownForceRun : public IJudge {
public:
    PlayerJudgeWallHitDownForceRun(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,PlayerCounterForceRun const*,PlayerTrigger const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x30/8];
};

class PlayerJudgeWallPush : public IJudge {
public:
    PlayerJudgeWallPush(al::LiveActor const*,IUsePlayerCollision const*,PlayerInput const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x20/8];
};

class PlayerJudgeWallHitDownRolling : public IJudge {
public:
    PlayerJudgeWallHitDownRolling(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,PlayerTrigger const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x28/8];
};

#include "Player/PlayerJudgeWallKeep.h"

class PlayerJudgeCameraSubjective : public IJudge {
public:
    PlayerJudgeCameraSubjective(al::LiveActor const*,IUsePlayerCollision const*,PlayerInput const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x28/8];
};

#include "Player/PlayerSpinCapAttack.h"

class PlayerJudgeDiveInWater : public IJudge {
public:
    PlayerJudgeDiveInWater(al::LiveActor const*,PlayerConst const*,IUsePlayerHeightCheck const*,IUsePlayerFallDistanceCheck const*);
    
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x28/8];
};

class PlayerActionDiveInWater {
public:
    PlayerActionDiveInWater(PlayerAnimator *,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerJudgeDiveInWater const*);

    bool isDiveInWaterAnim();
    void tryChangeDiveInWaterAnim();
    bool judgeEnableDiveInWater() const { return true; }
private:
    void* size[0x20/8];
};

class PlayerJudgeForceLand : public IJudge {
public:
    PlayerJudgeForceLand(IJudge const*,PlayerTrigger const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x18/8];
};

class PlayerJudgeOutInWater : public IJudge {
public:
    PlayerJudgeOutInWater(al::LiveActor const*,IJudge const*,IJudge const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

#include "Player/PlayerJudgeStartGroundSpin.h"

class PlayerJudgeWaterSurfaceRun : public IJudge {
public:
    PlayerJudgeWaterSurfaceRun(al::LiveActor const*,PlayerConst const*,al::WaterSurfaceFinder const*,PlayerCounterForceRun const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x28/8];
};

class PlayerJudgeStartWaterSurfaceRun : public IJudge {
public:
    PlayerJudgeStartWaterSurfaceRun(al::LiveActor const*,al::WaterSurfaceFinder const*,PlayerCounterForceRun const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x20/8];
};

#include "Player/PlayerJudgeSpeedCheckFall.h"
#include "Player/PlayerJudgeStartRun.h"

class PlayerJudgeCapCatch : public IJudge {
public:
    PlayerJudgeCapCatch(al::LiveActor const*,PlayerCounterAfterCapCatch const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x18/8];
};
class HackCapJudgeHoldHoveringJump : public IJudge {
public:
    HackCapJudgeHoldHoveringJump(al::LiveActor const*,PlayerColliderHakoniwa const*,HackCap const*,PlayerInput const*,PlayerConst const*);

    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x30/8];
};


class PlayerJointParamGroundPose;
class PlayerJointParamCenterDynamics;
#include "Player/PlayerStateWait.h"
class PlayerStateSandSink : public al::NerveStateBase {
public:
    PlayerStateSandSink(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerTrigger const*,IUsePlayerCollision *,PlayerAnimator *,IJudge *,PlayerJudgePreInputJump *);


private:
    void* size[0x78/8];
};

class ActorStateSandGeyser : public al::ActorStateBase {
public:
    ActorStateSandGeyser(al::LiveActor *);
public:
    al::HitSensor *mGeyserSensor = nullptr;
    void *size[1];
    bool _30;
};

class PlayerJointParamCapThrow;
#include "Player/PlayerStateSpinCap.h"
#include "Player/PlayerStateSquat.h"
#include "Player/PlayerStateSlope.h"

class PlayerStateRun2D : public al::NerveStateBase {
public:
    PlayerStateRun2D(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,PlayerAnimator *);
    bool isBrake() {return false;}
private:
    void* size[0x48/8];
};

#include "Player/PlayerStateRolling.h"

class PlayerStateCapCatchPop : public al::NerveStateBase {
public:
    PlayerStateCapCatchPop(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerAnimator *,HackCap *);
private:
    void* size[0x48/8];
};

class PlayerStateWallAir : public al::NerveStateBase {
public:
    PlayerStateWallAir(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerTrigger const*,IUsePlayerCollision *,IJudge *,PlayerJudgePreInputJump *,PlayerAnimator *,PlayerWallActionHistory *,PlayerActionDiveInWater *);
    void calcSnapMoveCutDir(sead::Vector3<float> *);
private:
    void* size[0x58/8];
};

class PlayerJointParamGrab;
class PlayerStateWallCatch : public al::NerveStateBase {
public:
    PlayerStateWallCatch(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision *,IUsePlayerCeilingCheck const*,PlayerModelHolder const*,PlayerAreaChecker const*,PlayerAnimator *,PlayerTrigger *,PlayerJudgePreInputJump *,PlayerJointParamGrab *);
    void setup(al::CollisionParts const*,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);
    bool isWallCatchForm();
    const sead::Vector3f& getCeilingCheckPos();

private:
    void* size[0xB8/8];
};

class PlayerStateGrabCeil : public al::NerveStateBase {
public:
    PlayerStateGrabCeil(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerModelHolder const*,PlayerTrigger *,IUsePlayerCollision *,PlayerAnimator *,PlayerJudgePreInputJump *,al::HitSensor *,PlayerJointParamGrab *);
    
    void setup(al::CollisionParts const*,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);
    bool isEnableNextGrabCeil();
private:
    void* size[0xC8/8];
};

class PlayerStatePoleClimb : public al::NerveStateBase {
public:
    PlayerStatePoleClimb(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerTrigger const*,PlayerModelHolder *,IUsePlayerCollision *,PlayerAnimator *,PlayerWallActionHistory *,PlayerJointParamHandLegAngle *,PlayerJudgePreInputJump *,PlayerActionDiveInWater *);

    void setup(al::CollisionParts const*,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float,float,char const*);
private:
    void* size[0xC0/8];
};

#include "Player/PlayerStateHipDrop.h"
#include "Player/PlayerStateHeadSliding.h"
#include "Player/PlayerStateLongJump.h"

class PlayerStateRise : public al::NerveStateBase {
public:
    PlayerStateRise(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerAnimator *);
private:
    void* size[0x38/8];
};

class PlayerJointParamSwim;
class PlayerStateSwim : public al::NerveStateBase {
public:
    PlayerStateSwim(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerTrigger const*,PlayerCarryKeeper const*,PlayerModelHolder const*,PlayerExternalVelocity const*,PlayerAnimator *,PlayerSpinCapAttack *,al::WaterSurfaceFinder const*,PlayerEffect *,PlayerJointParamSwim *);
    void tryReactionWaterIn() { CRASH; }
private:
    void* size[0x118/8];
};

class PlayerStateDamageLife : public al::NerveStateBase {
public:
    PlayerStateDamageLife(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,IUsePlayerCeilingCheck const*,PlayerAnimator *,PlayerTrigger *);
private:
    void* size[0x68/8];
};

class PlayerStateDamageSwim : public al::NerveStateBase {
public:
    PlayerStateDamageSwim(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerTrigger const*,PlayerAnimator *,al::WaterSurfaceFinder const*,PlayerEffect *,IJudge *,IJudge *);
private:
    void* size[0x70/8];
};

class PlayerStateDamageFire : public al::NerveStateBase {
public:
    PlayerStateDamageFire(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerAnimator *,IJudge *);
private:
    void* size[0x68/8];
};

class PlayerStatePress : public al::NerveStateBase {
public:
    PlayerStatePress(al::LiveActor *,IUsePlayerCollision const*,PlayerAnimator *);
private:
    void* size[0x60/8];
};

class PlayerStateBind : public al::NerveStateBase {
public:
    PlayerStateBind(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerPuppet const*,IUsePlayerCollision const*,PlayerTrigger *);
private:
    void* size[0x40/8];
};

class PlayerStateHack : public al::NerveStateBase {
public:
    PlayerStateHack(al::LiveActor *,PlayerHackKeeper *,IPlayerModelChanger *,PlayerAnimator *,HackCap *);
    bool isEnableModelSyncShowHide();
private:
    void* size[0xD0/8];
};

class PlayerStateEndHack : public al::NerveStateBase {
public:
    PlayerStateEndHack(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerTrigger *,PlayerAnimator *);
private:
    void* size[0x60/8];
};

class PlayerStateCameraSubjective : public al::NerveStateBase {
public:
    PlayerStateCameraSubjective(al::LiveActor *,al::CameraTicket *,IPlayerModelChanger *,PlayerCarryKeeper *,PlayerAnimator *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*);
private:
    void* size[0x60/8];
};

class PlayerStateAbyss : public al::NerveStateBase {
public:
    PlayerStateAbyss(al::LiveActor *,PlayerConst const*,PlayerRecoverySafetyPoint *,PlayerColliderHakoniwa *,PlayerAnimator *,al::LiveActor *);
private:
    void* size[0x40/8];
};

#include "Player/PlayerOxygen.h"

class PlayerCounterIceWater {
public:
    PlayerCounterIceWater(al::LiveActor *,al::ActorInitInfo const&,PlayerConst const*,IUsePlayerCollision const*,IJudge *);
    void clearIceWaterCount();
    void updateCount(bool, bool);
private:
    void* size[0x48/8];
};

class PlayerStainControl {
public:
    PlayerStainControl(al::LiveActor const*,al::LiveActor*,PlayerModelChangerHakoniwa const*,HackCap const*,PlayerEffect *);
private:
    void* size[0x88/8];
};

class PlayerJudgeRecoveryLifeFast : public IJudge {
public:
    PlayerJudgeRecoveryLifeFast(al::LiveActor const*,PlayerDamageKeeper const*,PlayerBindKeeper const*,IUsePlayerCollision const*,PlayerInput const*,PlayerHackKeeper const*,PlayerStateWait const*,PlayerStateSwim const*,IJudge const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x48/8];
};

class PlayerJudgeCameraInWater : public IJudge {
public:
    PlayerJudgeCameraInWater(al::LiveActor const*,IJudge const*,al::WaterSurfaceFinder const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x18/8];
};

class PlayerJudgeActiveCameraSubjective : public IJudge {
public:
    PlayerJudgeActiveCameraSubjective(PlayerStateCameraSubjective const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x8/8];
};

#include "Player/PlayerJudgeTalkGround.h"

class PlayerJudgeTalkSwim : public IJudge {
public:
    PlayerJudgeTalkSwim(PlayerHackKeeper const*,PlayerCarryKeeper const*,PlayerStateSwim const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x18/8];
};

class PlayerJudgeDead : public IJudge {
public:
    PlayerJudgeDead(al::LiveActor const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x8/8];
};

#include "Player/PlayerJudgeDrawForward.h"
#include "Player/PlayerJudgeSameNerve.h"

class PlayerJudgeFailureCameraSubjective : public IJudge {
public:
    PlayerJudgeFailureCameraSubjective(PlayerInput const*,IJudge const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x10/8];
};

class PlayerJudgeSafetyPointRecovery : public IJudge {
public:
    PlayerJudgeSafetyPointRecovery(IJudge *,PlayerStateAbyss const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x10/8];
};

class PlayerJudgeStatusPoleClimb : public IJudge {
public:
    PlayerJudgeStatusPoleClimb(IJudge const*,PlayerStatePoleClimb const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x10/8];
};

class PlayerJudgePlaySwitchOnAreaWaitAnim : public IJudge {
public:
    PlayerJudgePlaySwitchOnAreaWaitAnim(PlayerStateWait const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x8/8];
};

class PlayerJudgeSleep : public IJudge {
public:
    PlayerJudgeSleep(PlayerStateWait const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x8/8];
};

class PlayerJudgeEnableGuideArrow : public IJudge {
public:
    PlayerJudgeEnableGuideArrow(PlayerPuppet const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x8/8];
};

class PlayerJudgeEnablePeachAmiibo : public IJudge {
public:
    PlayerJudgeEnablePeachAmiibo(PlayerStateDamageFire const*);
    void reset() override { WARN_UNIMPL; }
    void update() override { WARN_UNIMPL; }
    bool judge() const override { WARN_UNIMPL;return false; }
private:
    void* size[0x8/8];
};

class WaterSurfaceShadow {
public:
    WaterSurfaceShadow(al::ActorInitInfo const&,char const*);
    void setScale(f32);
public:
    al::LiveActor *_0;
    al::WaterSurfaceFinder *_8;
    sead::Vector3f _10;
};

class PlayerInitInfo {
public:
    al::GamePadSystem *mGamePadSystem;
    const sead::Matrix34f *mViewMtx;
    int mPortNo;
    const char *strings[2];
    sead::Vector3f _20;
    sead::Quatf _34;
    int gap3;
    void *unk_size[];
};

class GaugeAir {
public:
    GaugeAir(char const*,al::LayoutInitInfo const&);
public:
    void* size[0x130/8];
};

#include "Player/PlayerInfo.h"
