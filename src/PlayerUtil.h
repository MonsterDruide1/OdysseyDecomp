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
#include "prim/seadSafeString.h"

namespace al {
class CameraTicket;
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

}

class CapFunction {
public:
    static void putOnCapPlayer(HackCap *,PlayerAnimator *);

};

class PlayerModelHolder {
public:
    struct Entry {
        sead::FixedSafeString<128> mName;
        al::LiveActor *mLiveActor;
    };


    PlayerModelHolder(s32);
    void registerModel(al::LiveActor*, const char*);
    void changeModel(const char*);
    al::LiveActor* findModelActor(char const*);
public:
    sead::PtrArray<Entry> mBuffer;
    Entry* currentModel;
    sead::FixedSafeString<128> _10;
};

class PlayerJointControlKeeper {
public:
    PlayerJointControlKeeper(al::LiveActor *,PlayerConst const*,bool,bool,bool);
public:
    void *gap[24];
    sead::Matrix34f* mtx;
    void *size_f0[5];
};

class PlayerCeilingCheck;
class CollisionShapeKeeper;
class IUsePlayerCeilingCheck {
    virtual void something();
};
class IUsePlayerHeightCheck {
    virtual void something();
};
class IUsePlayerFallDistanceCheck {
    virtual void something();
};
class PlayerColliderHakoniwa : public al::HioNode, public IUsePlayerCollision, public IUsePlayerCeilingCheck, public IUsePlayerHeightCheck, public IUsePlayerFallDistanceCheck {
public:
    PlayerColliderHakoniwa(al::LiveActor *,PlayerConst const*);
    void init(void);
    PlayerCollider* getPlayerCollider() const override;
public:
    al::LiveActor *mPlayer;
    const PlayerConst *mPlayerConst;
    PlayerCollider *mPlayerCollider;
    PlayerCeilingCheck *mPlayerCeilingCheck;
    bool mIsAboveGround;
    bool pad[3];
    float mGroundHeight;
    float mShadowDropHeight;
    sead::Vector3f mGroundNormal;
    sead::Vector3f unkFallDistance2;
    float mFallDistance;
    int mTimeSinceLastTouchGround;
    int pad2;
    void *mCollidedGroundSensorUnknownType;
    CollisionShapeKeeper *mShapeKeeperNormal;
    CollisionShapeKeeper *mShapeKeeperMini;
    CollisionShapeKeeper *mShapeKeeperSwim;
    CollisionShapeKeeper *mShapeKeeperGrabCeil;
    CollisionShapeKeeper *mShapeKeeperWallGrab;
    CollisionShapeKeeper *mShapeKeeperPoleClimb;
    CollisionShapeKeeper *mShapeKeeper2DNormal;
    CollisionShapeKeeper *mShapeKeeper2DMini;
    al::CollisionPartsFilterBase *mFilter2DOnly;
};
static_assert(sizeof(PlayerColliderHakoniwa) == 0xC0);

class IPlayerModelChanger {};
class PlayerModelChangerHakoniwa : public IPlayerModelChanger {
public:
    PlayerModelChangerHakoniwa(al::LiveActor const*,PlayerModelHolder *,PlayerPainPartsKeeper *,PlayerCostumeInfo *,IUseDimension const*);
private:
    void* size[0x58/8];
};

class PlayerAnimFrameCtrl;
class PlayerAnimator {
public:
    PlayerAnimator(PlayerModelHolder const*,al::ActorDitherAnimator *);
public:
    const PlayerModelHolder *mPlayerModelHolder;
    void *_8;
    void *_10;
    PlayerAnimFrameCtrl *mPlayerAnimFrameCtrl;
    sead::FixedSafeString<64> _18;
    int gap;
    sead::FixedSafeString<64> _78;
    int gap2;
    sead::FixedSafeString<64> _D0;
    int gap3;
    sead::FixedSafeString<64> _128;
    al::ActorDitherAnimator *mActorDitherAnimator;
    void *to_1a8[3];
    bool mIsNeedFullFaceAnim;
    bool pad[7];
};

class PlayerEffect {
public:
    PlayerEffect(al::LiveActor *,PlayerModelHolder const*,sead::Matrix34<float> const*);
private:
    void* size[0xC0/8];
};

class PlayerContinuousJump {
public:
    PlayerContinuousJump(PlayerConst const*);
private:
    void* size[0x20/8];
};

class PlayerContinuousLongJump {
public:
    PlayerContinuousLongJump(PlayerConst const*);
private:
    void* size[0x10/8];
};

class PlayerDamageKeeper {
public:
    PlayerDamageKeeper(al::LiveActor *,PlayerEffect *);
private:
    void* size[0x30/8];
};

class PlayerDemoActionFlag {
public:
    PlayerDemoActionFlag();
private:
    void* size[0x18/8];
};

class PlayerCounterAfterCapCatch;
class PlayerCapActionHistory {
public:
    PlayerCapActionHistory(al::LiveActor const*,PlayerConst const*,PlayerTrigger const*,IUsePlayerCollision const*);
public:
    const al::LiveActor *mActor;
    const PlayerConst *mConst;
    const PlayerTrigger *mTrigger;
    const IUsePlayerCollision *mCollision;
    PlayerCounterAfterCapCatch *mCounterAfterCapCatch;
    bool isLimitHeight;
    bool pad[3];
    sead::Vector3f mHeightLimit;
    bool mCanCapJump;
    bool gap3;
    bool pad2[6];
};

class PlayerAreaChecker {
public:
    PlayerAreaChecker(al::LiveActor const*,PlayerModelHolder const*);
private:
    void* size[0x10/8];
};

class WorldEndBorderKeeper {
public:
    WorldEndBorderKeeper(al::LiveActor const*);
private:
    void* size[0x60/8];
};

class PlayerWallActionHistory {
public:
    PlayerWallActionHistory();
private:
    void* size[0x38/8];
};

class PlayerCounterAfterUpperPunch {
public:
    PlayerCounterAfterUpperPunch();
private:
    int size[1];
};

class PlayerCounterForceRun {
public:
    PlayerCounterForceRun();
public:
    s32 _0;
    float _4;
};

class PlayerCounterQuickTurnJump {
public:
    PlayerCounterQuickTurnJump(PlayerConst const*,PlayerTrigger const*);
private:
    void* size[0x18/8];
};

class PlayerJumpMessageRequest {
public:
    PlayerJumpMessageRequest();
private:
    void* size[0x28/8];
};

class PlayerSandSinkAffect {
public:
    PlayerSandSinkAffect(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision *,PlayerEffect *);
private:
    void* size[0x38/8];
};

class PlayerEyeSensorHitHolder {
public:
    PlayerEyeSensorHitHolder(int);
    void createTargetMarkerBuffer(int);
private:
    void* size[0x48/8];
};

class PlayerPushReceiver {
public:
    PlayerPushReceiver(al::LiveActor* const);
private:
    void* size[0x38/8];
};

class PlayerHitPush {
public:
    PlayerHitPush(al::LiveActor* const, PlayerConst* const);
private:
    void* size[0x20/8];
};

class PlayerExternalVelocity {
public:
    PlayerExternalVelocity(al::LiveActor const*,IUsePlayerCollision const*,sead::Vector3f const*);
private:
    void* size[0x98/8];
};

class PlayerWetControl {
public:
    PlayerWetControl(al::LiveActor const*,al::LiveActor*,PlayerAreaChecker const*);
public:
    void *gap[1];
    PlayerEffect *mEffect;
    void *size[5];
};

class PlayerSeparateCapFlag {
public:
    PlayerSeparateCapFlag();
private:
    void* size[0x10/8];
};

class HackCapJudgePreInputSeparateThrow {
public:
    HackCapJudgePreInputSeparateThrow(PlayerInput const*,PlayerSeparateCapFlag const*);
private:
    void* size[0x20/8];
};
class HackCapJudgePreInputSeparateJump {
public:
    HackCapJudgePreInputSeparateJump(PlayerInput const*,PlayerSeparateCapFlag const*);
private:
    void* size[0x20/8];
};

class PlayerRippleGenerator {
public:
    PlayerRippleGenerator(al::LiveActor const*,al::LiveActor const*,PlayerModelHolder const*);
private:
    void* size[0x58/8];
};

class PlayerRecoverySafetyPoint {
public:
    PlayerRecoverySafetyPoint(al::LiveActor const*,HackCap const*,al::ActorInitInfo const&,IUseDimension const*,al::CollisionPartsFilterBase *,al::HitSensor *);
private:
    void* size[0xB8/8];
};

class IUsePlayerPuppet {};
class PlayerPuppet : public IUsePlayerPuppet {
public:
    PlayerPuppet(al::LiveActor *,HackCap *,PlayerAnimator *,IUsePlayerCollision *,ActorDimensionKeeper *,IPlayerModelChanger *,WorldEndBorderKeeper *,PlayerCounterForceRun *,PlayerDamageKeeper *,PlayerEffect *,PlayerInput const*,PlayerConst const*);
private:
    void* size[0xD0/8];
};

class PlayerBindKeeper {
public:
    PlayerBindKeeper(al::HitSensor *,IUsePlayerPuppet *);
private:
    void* size[0xD0/8];
};

class PlayerJointParamHandLegAngle;
class IUsePlayerCeilingCheck;
class PlayerCarryKeeper {
public:
    PlayerCarryKeeper(al::LiveActor const*,al::HitSensor *,PlayerAnimator *,IPlayerModelChanger const*,IUsePlayerCeilingCheck const*,PlayerJointParamHandLegAngle *);
    void startCancelAndRelease();
    bool isCarry();
private:
    void* size[0xD0/8];
};

class PlayerSeCtrl {
public:
    PlayerSeCtrl(al::LiveActor const*,PlayerAnimator const*,HackCap const*,PlayerModelChangerHakoniwa const*,al::LiveActor const*,PlayerExternalVelocity const*);
private:
    void* size[0x198/8];
};

class IUsePlayerHeightCheck;
class PlayerHackKeeper {
public:
    PlayerHackKeeper(al::LiveActor *,HackCap *,PlayerRecoverySafetyPoint *,PlayerInput const*,sead::Matrix34<float> const*,PlayerDamageKeeper const*,IPlayerModelChanger const*,IUsePlayerHeightCheck const*);
    void createHackModel(al::ActorInitInfo const&);
private:
    void* size[0xD0/8];
};

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
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerCounterAfterCapCatch;
class PlayerJudgeCapCatchPop : public IJudge {
public:
    PlayerJudgeCapCatchPop(al::LiveActor const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCapActionHistory const*,PlayerCounterAfterCapCatch const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x38/8];
};

class PlayerJudgeDeadWipeStart : public IJudge {
public:
    PlayerJudgeDeadWipeStart(al::LiveActor const*,PlayerConst const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x20/8];
};

class PlayerJudgeDirectRolling : public IJudge {
public:
    PlayerJudgeDirectRolling(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerCounterForceRun const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x48/8];
};

class PlayerJudgeEnableStandUp : public IJudge {
public:
    PlayerJudgeEnableStandUp(IUsePlayerCollision const*,IUsePlayerCeilingCheck const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x18/8];
};

class PlayerJudgeForceSlopeSlide : public IJudge {
public:
    PlayerJudgeForceSlopeSlide(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerJudgeForceRolling : public IJudge {
public:
    PlayerJudgeForceRolling(al::LiveActor const*,IUsePlayerCollision const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x18/8];
};

class PlayerJudgeGrabCeil : public al::HioNode, public IJudge {
public:
    PlayerJudgeGrabCeil(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerExternalVelocity const*);
    
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

class PlayerJudgeInvalidateInputFall : public IJudge {
public:
    PlayerJudgeInvalidateInputFall(al::LiveActor const*,PlayerAreaChecker const*,IUsePlayerCollision const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class IUsePlayerFallDistanceCheck;
class PlayerJudgeLongFall : public IJudge {
public:
    PlayerJudgeLongFall(al::LiveActor const*,PlayerConst const*,IUsePlayerFallDistanceCheck const*,PlayerHackKeeper const*,PlayerBindKeeper const*,IPlayerModelChanger const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
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
    bool mIsJudge;
    const al::CollisionParts *mCollisionParts;
    sead::Vector3f _58;
    sead::Vector3f _64;
    sead::Vector3f _70;
    float _7C;
    float mPoleCodeAngleOffsetCeiling;
    const char *mMaterialCodeCeiling;
};
static_assert(sizeof(PlayerJudgePoleClimb) == 0x90);

class PlayerJudgePreInputCapThrow : public IJudge {
public:
    PlayerJudgePreInputCapThrow(PlayerConst const*,PlayerInput const*,PlayerCarryKeeper const*,HackCap const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x60/8];
};

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

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x30/8];
};

class PlayerJudgeSandSink : public IJudge {
public:
    PlayerJudgeSandSink(IUsePlayerCollision const*,PlayerSandSinkAffect const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x18/8];
};

class PlayerJudgeSlopeSlide : public IJudge {
public:
    PlayerJudgeSlopeSlide(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x38/8];
};

class PlayerJudgeStartHipDrop : public IJudge {
public:
    PlayerJudgeStartHipDrop(PlayerConst const*,PlayerInput const*,IUsePlayerHeightCheck const*,IPlayerModelChanger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerJudgeStartRise : public IJudge {
public:
    PlayerJudgeStartRise(al::LiveActor const*,PlayerAreaChecker const*,IPlayerModelChanger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x20/8];
};

class PlayerJudgeStartRolling : public IJudge {
public:
    PlayerJudgeStartRolling(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x38/8];
};

class PlayerJudgeStartSquat : public IJudge {
public:
    PlayerJudgeStartSquat(PlayerInput const*,PlayerCounterForceRun const*,PlayerCarryKeeper const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x20/8];
};

class PlayerJudgeWallCatch : public al::HioNode, public IJudge {
public:
    PlayerJudgeWallCatch(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerExternalVelocity const*,PlayerInput const*,PlayerTrigger const*,PlayerCounterForceRun const*);

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
    const PlayerCounterForceRun *mCounterForceRun;
    bool mIsJudge;
    al::CollisionParts *mCollidedWallPart;
    sead::Vector3f _60;
    sead::Vector3f _6C;
    sead::Vector3f _78;
};

class PlayerJudgeWallCatchInputDir : public IJudge {
public:
    PlayerJudgeWallCatchInputDir(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerExternalVelocity const*,PlayerCounterForceRun const*,IUsePlayerHeightCheck const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0xA8/8];
};

class PlayerJudgeWallHitDown : public IJudge {
public:
    PlayerJudgeWallHitDown(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,PlayerTrigger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerJudgeWallHitDownForceRun : public IJudge {
public:
    PlayerJudgeWallHitDownForceRun(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,PlayerCounterForceRun const*,PlayerTrigger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x30/8];
};

class PlayerJudgeWallHitDownRolling : public IJudge {
public:
    PlayerJudgeWallHitDownRolling(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,PlayerTrigger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerJudgeWallKeep : public IJudge {
public:
    PlayerJudgeWallKeep(al::LiveActor const*,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,IUsePlayerHeightCheck const*,PlayerCounterAfterUpperPunch const*,PlayerWallActionHistory const*,PlayerCarryKeeper const*,PlayerTrigger const*,PlayerCounterForceRun const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x68/8];
};

class PlayerJudgeCameraSubjective : public IJudge {
public:
    PlayerJudgeCameraSubjective(al::LiveActor const*,IUsePlayerCollision const*,PlayerInput const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerSpinCapAttack : public IJudge {
public:
    PlayerSpinCapAttack(HackCap *,PlayerConst const*,PlayerTrigger const*,PlayerInput const*,PlayerCounterAfterCapCatch const*,PlayerJudgePreInputCapThrow const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x110/8];
};

class PlayerJudgeDiveInWater : public IJudge {
public:
    PlayerJudgeDiveInWater(al::LiveActor const*,PlayerConst const*,IUsePlayerHeightCheck const*,IUsePlayerFallDistanceCheck const*);
    
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x28/8];
};

class PlayerActionDiveInWater {
public:
    PlayerActionDiveInWater(PlayerAnimator *,IPlayerModelChanger const*,PlayerCarryKeeper const*,PlayerJudgeDiveInWater const*);

    bool isDiveInWaterAnim();
private:
    void* size[0x20/8];
};

class PlayerJudgeForceLand : public IJudge {
public:
    PlayerJudgeForceLand(IJudge const*,PlayerTrigger const*);

    void reset() override;
    void update() override;
    bool judge() const override;
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

class PlayerJudgePreInputJump : public IJudge {
public:
    PlayerJudgePreInputJump(PlayerConst const*,PlayerInput const*,IJudge *);

    void reset() override;
    void update() override;
    bool judge() const override;
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

class PlayerJudgeSpeedCheckFall : public IJudge {
public:
    PlayerJudgeSpeedCheckFall(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*,IJudge const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x30/8];
};

class PlayerJudgeStartRun : public IJudge {
public:
    PlayerJudgeStartRun(al::LiveActor const*,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerCounterForceRun const*,IJudge const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x30/8];
};
class PlayerJudgeCapCatch : public IJudge {
public:
    PlayerJudgeCapCatch(al::LiveActor const*,PlayerCounterAfterCapCatch const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x18/8];
};
class HackCapJudgeHoldHoveringJump : public IJudge {
public:
    HackCapJudgeHoldHoveringJump(al::LiveActor const*,PlayerColliderHakoniwa const*,HackCap const*,PlayerInput const*,PlayerConst const*);

    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x30/8];
};


class PlayerJointParamGroundPose;
class PlayerJointParamCenterDynamics;
class PlayerStateWait : public al::NerveStateBase {
public:
    PlayerStateWait(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerModelChangerHakoniwa const*,PlayerJointControlKeeper const*,al::WaterSurfaceFinder const*,IUsePlayerHeightCheck const*,IJudge const*,PlayerAnimator *,PlayerTrigger *,PlayerCapManHeroEyesControl *,PlayerJointParamCenterDynamics *,PlayerJointParamGroundPose *);

private:
    void* size[0xE8/8];
};
class PlayerStateSandSink : public al::NerveStateBase {
public:
    PlayerStateSandSink(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerTrigger const*,IUsePlayerCollision *,PlayerAnimator *,IJudge *,PlayerJudgePreInputJump *);


private:
    void* size[0x78/8];
};

class ActorStateSandGeyser : public al::NerveStateBase {
public:
    ActorStateSandGeyser(al::LiveActor *);
private:
    void* size[0x38/8];
};

class PlayerJointParamCapThrow;
class PlayerStateSpinCap : public al::NerveStateBase {
public:
    PlayerStateSpinCap(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerCounterForceRun const*,al::WaterSurfaceFinder const*,IUsePlayerCollision const*,PlayerTrigger *,PlayerSpinCapAttack *,PlayerAnimator *,PlayerJointParamCapThrow *);

private:
    void* size[0xB0/8];
};

class PlayerStateSquat : public al::NerveStateBase {
public:
    PlayerStateSquat(al::LiveActor *,PlayerConst const*,PlayerAnimator *,PlayerInput const*,IUsePlayerCollision const*,IPlayerModelChanger const*,PlayerTrigger const*,IJudge *,IJudge *,IJudge *);
private:
    void* size[0x78/8];
};

class PlayerStateRunHakoniwa2D3D : public al::NerveStateBase {
public:
    PlayerStateRunHakoniwa2D3D(al::LiveActor *,PlayerConst const*,IUseDimension const*,PlayerInput const*,IUsePlayerCollision const*,al::WaterSurfaceFinder const*,PlayerCounterForceRun const*,PlayerCounterQuickTurnJump const*,PlayerTrigger *,PlayerAnimator *,PlayerEffect *,PlayerJointParamCenterDynamics *,bool);
    
private:
    void* size[0x78/8];
};

class PlayerStateSlope : public al::NerveStateBase {
public:
    PlayerStateSlope(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,PlayerAnimator *);
private:
    void* size[0x50/8];
};

class PlayerStateRolling : public al::NerveStateBase {
public:
    PlayerStateRolling(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,PlayerTrigger *,PlayerAnimator *,PlayerEffect *,PlayerJudgeStartRolling *,IJudge *,PlayerJudgePreInputJump *,PlayerJudgePreInputCapThrow *,IJudge *,PlayerContinuousLongJump *,PlayerSeCtrl *);
private:
    void* size[0xB8/8];
};

class PlayerStateJump : public al::NerveStateBase {
public:
    PlayerStateJump(al::LiveActor *,PlayerConst const*,IPlayerModelChanger const*,IUseDimension const*,PlayerInput const*,PlayerJumpMessageRequest const*,IJudge const*,PlayerCounterForceRun const*,PlayerJointControlKeeper const*,IUsePlayerCollision const*,PlayerTrigger *,PlayerContinuousJump *,PlayerAnimator *,PlayerActionDiveInWater *,HackCap *,IJudge*,bool);
private:
    void* size[0x100/8];
};

class PlayerStateCapCatchPop : public al::NerveStateBase {
public:
    PlayerStateCapCatchPop(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerAnimator *,HackCap *);
private:
    void* size[0x48/8];
};

class PlayerStateWallAir : public al::NerveStateBase {
public:
    PlayerStateWallAir(al::LiveActor *,PlayerConst const*,PlayerInput const*,PlayerTrigger const*,IUsePlayerCollision *,IJudge *,PlayerJudgePreInputJump *,PlayerAnimator *,PlayerWallActionHistory *,PlayerActionDiveInWater *);
private:
    void* size[0x58/8];
};

class PlayerJointParamGrab;
class PlayerStateWallCatch : public al::NerveStateBase {
public:
    PlayerStateWallCatch(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision *,IUsePlayerCeilingCheck const*,PlayerModelHolder const*,PlayerAreaChecker const*,PlayerAnimator *,PlayerTrigger *,PlayerJudgePreInputJump *,PlayerJointParamGrab *);
    void setup(al::CollisionParts const*,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);

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

class PlayerStateHipDrop : public al::NerveStateBase {
public:
    PlayerStateHipDrop(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,PlayerAnimator *,PlayerTrigger *);
private:
    void* size[0x60/8];
};

class PlayerStateHeadSliding : public al::NerveStateBase {
public:
    PlayerStateHeadSliding(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerActionDiveInWater const*,PlayerAnimator *);
private:
    void* size[0x50/8];
};

class PlayerStateLongJump : public al::NerveStateBase {
public:
    PlayerStateLongJump(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,PlayerTrigger *,PlayerAnimator *,PlayerContinuousLongJump *,PlayerActionDiveInWater *);
private:
    void* size[0x58/8];
};

class PlayerStateFallHakoniwa : public al::NerveStateBase {
public:
    PlayerStateFallHakoniwa(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerInput const*,PlayerTrigger const*,PlayerAreaChecker const*,IUseDimension const*,PlayerAnimator *);
private:
    void* size[0x80/8];
};

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

class PlayerOxygen {
public:
    PlayerOxygen();
    void setup(int,int,int,int);
    void reset();
    void reduce();
    void recovery();
    bool isTriggerDamage();
private:
    int* size[0x1C/4];
};

class PlayerCounterIceWater {
public:
    PlayerCounterIceWater(al::LiveActor *,al::ActorInitInfo const&,PlayerConst const*,IUsePlayerCollision const*,IJudge *);
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
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x18/8];
};

class PlayerJudgeActiveCameraSubjective : public IJudge {
public:
    PlayerJudgeActiveCameraSubjective(PlayerStateCameraSubjective const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x8/8];
};

class PlayerJudgeTalkGround : public IJudge {
public:
    PlayerJudgeTalkGround(al::LiveActor const*,IPlayerModelChanger const*,PlayerHackKeeper const*,PlayerCarryKeeper const*,IUsePlayerCollision const*,PlayerInput const*,PlayerConst const*,PlayerStateWait const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x40/8];
};

class PlayerJudgeTalkSwim : public IJudge {
public:
    PlayerJudgeTalkSwim(PlayerHackKeeper const*,PlayerCarryKeeper const*,PlayerStateSwim const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x18/8];
};

class PlayerJudgeDead : public IJudge {
public:
    PlayerJudgeDead(al::LiveActor const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x8/8];
};

class PlayerJudgeDrawForward : public IJudge {
public:
    PlayerJudgeDrawForward(al::LiveActor const*,PlayerJudgeDeadWipeStart const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x10/8];
};

class PlayerJudgeSameNerve : public IJudge {
public:
    PlayerJudgeSameNerve(al::IUseNerve const*,al::Nerve const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x10/8];
};

class PlayerJudgeFailureCameraSubjective : public IJudge {
public:
    PlayerJudgeFailureCameraSubjective(PlayerInput const*,IJudge const*);
    void reset() override;
    void update() override;
    bool judge() const override;
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
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x10/8];
};

class PlayerJudgePlaySwitchOnAreaWaitAnim : public IJudge {
public:
    PlayerJudgePlaySwitchOnAreaWaitAnim(PlayerStateWait const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x8/8];
};

class PlayerJudgeSleep : public IJudge {
public:
    PlayerJudgeSleep(PlayerStateWait const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x8/8];
};

class PlayerJudgeEnableGuideArrow : public IJudge {
public:
    PlayerJudgeEnableGuideArrow(PlayerPuppet const*);
    void reset() override;
    void update() override;
    bool judge() const override;
private:
    void* size[0x8/8];
};

class PlayerJudgeEnablePeachAmiibo : public IJudge {
public:
    PlayerJudgeEnablePeachAmiibo(PlayerStateDamageFire const*);
    void reset() override;
    void update() override;
    bool judge() const override;
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

class GaugeAir {
public:
    GaugeAir(char const*,al::LayoutInitInfo const&);
public:
    void* size[0x130/8];
};

struct PlayerInfo {
  PlayerModelChangerHakoniwa *mPlayerModelChangerHakoniwa;
  PlayerOxygen *mPlayerOxygen;
  PlayerAnimator *mPlayerAnimator;
  PlayerBindKeeper *mPlayerBindKeeper;
  PlayerDamageKeeper *mPlayerDamageKeeper;
  PlayerDemoActionFlag *mPlayerDemoActionFlag;
  PlayerEquipmentUser *mPlayerEquipmentUser;
  HackCap *mHackCap;
  WorldEndBorderKeeper *mWorldEndBorderKeeper;
  PlayerCarryKeeper *mPlayerCarryKeeper;
  PlayerJointControlKeeper *mPlayerJointControlKeeper;
  PlayerCounterIceWater *mPlayerCounterIceWater;
  PlayerStainControl *mPlayerStainControl;
  al::FootPrintHolder *mFootPrintHolder;
  al::HitSensor *mBodyHitSensor;
  PlayerFormSensorCollisionArranger *mPlayerFormSensorCollisionArranger;
  PlayerInput *mPlayerInput;
  IUsePlayerCeilingCheck *mCeilingCheck;
  PlayerModelHolder *mPlayerModelHolder;
  PlayerHackKeeper *mPlayerHackKeeper;
  PlayerCapManHeroEyesControl *mPlayerCapManHeroEyesControl;
  PlayerRecoverySafetyPoint *mPlayerRecoverySafetyPoint;
  PlayerCostumeInfo *mCostumeInfo;
  PlayerJudgeCameraInWater *mPlayerJudgeCameraInWater;
  PlayerJudgeTalkGround *mPlayerJudgeTalkGround;
  PlayerJudgeTalkSwim *mPlayerJudgeTalkSwim;
  PlayerJudgeDead *mPlayerJudgeDead;
  PlayerJudgeDeadWipeStart *mPlayerJudgeDeadWipeStart;
  PlayerJudgeDrawForward *mPlayerJudgeDrawForward;
  PlayerJudgeSameNerve *mPlayerJudgeSameNervePoleClimb;
  PlayerJudgeSameNerve *mPlayerJudgeSameNerveGrabCeil;
  PlayerJudgeSameNerve *mPlayerJudgeSameNerveWallCatch;
  PlayerJudgeActiveCameraSubjective *mPlayerJudgeActiveCameraSubjective;
  PlayerJudgeSameNerve *mPlayerJudgeSameNerveSwim;
  PlayerJudgeFailureCameraSubjective *mPlayerJudgeFailureCameraSubjective;
  PlayerJudgeSafetyPointRecovery *mPlayerJudgeSafetyPointRecovery;
  PlayerJudgeStatusPoleClimb *mPlayerJudgeStatusPoleClimb;
  PlayerJudgePlaySwitchOnAreaWaitAnim *mPlayerJudgePlaySwitchOnAreaWaitAnim;
  PlayerJudgeSleep *mPlayerJudgeSleep;
  PlayerJudgeEnableGuideArrow *mPlayerJudgeEnableGuideArrow;
  PlayerJudgeEnablePeachAmiibo *mPlayerJudgeEnablePeachAmiibo;
  bool mIsMoon;
};
