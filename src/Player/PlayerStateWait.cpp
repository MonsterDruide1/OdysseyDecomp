#include "Player/PlayerStateWait.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerTrigger.h"
#include "Stuff.h"
#include "Util/StageSceneFunction.h"
#include "basis/seadTypes.h"
#include "playerUtil.h"

namespace {
NERVE_IMPL(PlayerStateWait, WaitRelax);
NERVE_IMPL(PlayerStateWait, RequestAnimWait);
NERVE_IMPL(PlayerStateWait, Wait);
NERVE_IMPL(PlayerStateWait, LandStiffen);
NERVE_IMPL(PlayerStateWait, Land);
NERVE_IMPL(PlayerStateWait, WaitConnect2D);
NERVE_IMPL(PlayerStateWait, WaitSnoozeStart);
NERVE_IMPL(PlayerStateWait, WaitSnooze);
NERVE_IMPL(PlayerStateWait, WaitSleepStart);
NERVE_IMPL(PlayerStateWait, WaitSleep);
NERVE_IMPL(PlayerStateWait, AreaAnimWait);
NERVE_IMPL(PlayerStateWait, WaitRelaxStart);
NERVE_IMPL(PlayerStateWait, AreaAnimTurn);

NERVE_MAKE(PlayerStateWait, WaitRelax);
NERVE_MAKE(PlayerStateWait, RequestAnimWait);
NERVE_MAKE(PlayerStateWait, Wait);
NERVE_MAKE(PlayerStateWait, LandStiffen);
NERVE_MAKE(PlayerStateWait, Land);
NERVE_MAKE(PlayerStateWait, WaitConnect2D);
NERVE_MAKE(PlayerStateWait, WaitSnoozeStart);
NERVE_MAKE(PlayerStateWait, WaitSnooze);
NERVE_MAKE(PlayerStateWait, WaitSleepStart);
NERVE_MAKE(PlayerStateWait, WaitSleep);
NERVE_MAKE(PlayerStateWait, AreaAnimWait);
NERVE_MAKE(PlayerStateWait, WaitRelaxStart);
NERVE_MAKE(PlayerStateWait, AreaAnimTurn);
}  // namespace

PlayerStateWait::PlayerStateWait(al::LiveActor* player, PlayerConst const* pConst,
                                 IUsePlayerCollision const* collider,
                                 PlayerModelChangerHakoniwa const* modelChanger,
                                 PlayerJointControlKeeper const* jointControlKeeper,
                                 al::WaterSurfaceFinder const* waterSurfaceFinder,
                                 IUsePlayerHeightCheck const* heightCheck,
                                 IJudge const* judgeForceLand, PlayerAnimator* animator,
                                 PlayerTrigger* trigger, PlayerCapManHeroEyesControl* eyesControl,
                                 PlayerJointParamCenterDynamics* centerDynamics,
                                 PlayerJointParamGroundPose* groundPose)
    : al::ActorStateBase("待機", player), mConst(pConst), mCollider(collider),
      mModelChanger(modelChanger), mJointControlKeeper(jointControlKeeper),
      mWaterSurfaceFinder(waterSurfaceFinder), mHeightCheck(heightCheck),
      mJudgeForceLand(judgeForceLand), mAnimator(animator), mTrigger(trigger),
      mCapEyesControl(eyesControl), mJointParamCenterDynamics(centerDynamics),
      mJointParamGroundPose(groundPose) {
    _D0 = al::createCollisionPartsConnector(player, sead::Quatf::unit);
    initNerve(&Wait, 0);
}

PlayerStateWait::~PlayerStateWait() {
    CRASH
}

void PlayerStateWait::appear() {
    _E4 = mTrigger->isOn(PlayerTrigger::EDemoEndTrigger_val0);
    al::disconnectMtxConnector(_D0);
    _B4 = false;
    // call to function to read area animation data
    _A8 = _A4;
    mRequestAnimName = nullptr;
    mIsDead = false;
    // something for jointparamgroundpose

    bool isForceLand = rs::isJudge(mJudgeForceLand);
    if(isForceLand) {
        _CC = 0;
LABEL_3:
        sead::Vector3f a4 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalOrGravityDir(&a4, mActor, mCollider);
        sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
        al::alongVectorNormalH(&a1, al::getVelocity(mActor), -al::getGravity(mActor), a4);
        al::setVelocity(mActor, a1 - (mConst->getGravity() * a4));
        if(isForceLand) {
            al::setNerve(this, &LandStiffen);
        } else {
            // playeranimator stuff
            al::setNerve(this, &Land);
        }
        // calculate _C0
        WARN_UNIMPL;
        return;
    }

    // start hit reaction if something

    _CC = 0;
    if(rs::isJustLand(mCollider) && !mModelChanger->is2DModel() && !mTrigger->isOn(PlayerTrigger::EActionTrigger_val24)) {
        goto LABEL_3;
    }

    // more ParamCenterDynamics

    // calculate _C0
    WARN_UNIMPL;
    if(al::isInAreaObj(mActor, "PlayerMoveSmallPlanetArea")) {
        al::setNerve(this, &WaitConnect2D);
    } else {
        al::setNerve(this, &Wait);
    }
}
void PlayerStateWait::kill() {
    // stage switch
    // bird on nose
    // bgm situation
    mIsDead = true;
}
void PlayerStateWait::control() {
    WARN_UNIMPL;
}
bool PlayerStateWait::isWait() const {
    if(mIsDead) return false;
    return !al::isNerve(this, &LandStiffen) && !al::isNerve(this, &Land);
}
bool PlayerStateWait::isEnableLookAt() const {
    if(mIsDead) return false;
    return !al::isNerve(this, &LandStiffen) && 
    !al::isNerve(this, &Land) && 
    !al::isNerve(this, &WaitSnoozeStart) && 
    !al::isNerve(this, &WaitSnooze) &&
    !al::isNerve(this, &WaitSleepStart) &&
    !al::isNerve(this, &WaitSleep);
}
bool PlayerStateWait::isEnableCancelAction() const {
    if(al::isNerve(this, &AreaAnimTurn)) {
        if(_B4) return false;
    } else {
        bool isNerve = al::isNerve(this, &AreaAnimWait);
        if(_B4 && isNerve) return false;
    }

    if(al::isNerve(this, &LandStiffen)) {
        return _B0 >= 0.0f && al::isGreaterEqualStep(this, _B0);
    }

    return true;
}
bool PlayerStateWait::isEnableCancelHipDropJump() const {
    if(al::isNerve(this, &AreaAnimTurn)) {
        if(_B4) return false;
    } else {
        bool isNerve = al::isNerve(this, &AreaAnimWait);
        if(_B4 && isNerve) return false;
    }

    if(al::isNerve(this, &LandStiffen)) {
        return _B0 >= 0.0f && al::isInRangeStep(this, mConst->getJumpHipDropPermitBeginFrame(), mConst->getJumpHipDropPermitEndFrame());
    }
    return false;
}
bool PlayerStateWait::isEnableRecoveryLife() const {
    CRASH
}
bool PlayerStateWait::isEnableReactionCapCatch() const {
    CRASH
}
bool PlayerStateWait::isAreaAnimCapManHeroEyes() const {
    CRASH
}
bool PlayerStateWait::isPlaySwitchOnAreaAnim() const {
    CRASH
}
bool PlayerStateWait::isLandStain() const {
    return !mIsDead && al::isNerve(this, &LandStiffen) && rs::isCollidedGround(mCollider);
}
bool PlayerStateWait::isSleep() const {
    CRASH
}
bool PlayerStateWait::attackHipDropKnockDown(al::HitSensor*, al::HitSensor*) {
    CRASH
}
bool PlayerStateWait::tryClearIgnoreSwitchOnAreaAnim() {
    if(!mIsIgnoreSwitchOnAreaAnim) return true;
    if(!al::tryFindAreaObj(mActor, "PlayerAnimArea", al::getTrans(mActor)))
        return false;
    mIsIgnoreSwitchOnAreaAnim = false;
    return true;
}
bool PlayerStateWait::requestAnimName(char const*) {
    CRASH
}
void PlayerStateWait::noticeCarryStart() {
    CRASH
}
void PlayerStateWait::initSceneStartAnim() {
    // animation and warmth level
}
bool PlayerStateWait::tryConnectWait(){
    // MtxConnector
    WARN_UNIMPL;
    return false;
}
f32 PlayerStateWait::getInverseKinematicsRate() const {
    if(mIsDead || al::isNerve(this, &WaitSleep))
        return 0.0f;

    if(!al::isNerve(this, &WaitSleepStart))
        return 1.0f;

    return sead::Mathf::clamp(1.0f-al::calcNerveRate(this, 120), 0.0f, 1.0f);
}
//bool PlayerStateWait::tryUpdateAreaAnim() {}
//bool PlayerStateWait::tryChangeRequestAnim() {}
//bool PlayerStateWait::tryChangeAreaAnim() {}
bool PlayerStateWait::tryGetSpecialStatusAnimName(char const**) {
    CRASH
}
void PlayerStateWait::exeLandStiffen() {
    CRASH
}
void PlayerStateWait::exeLand() {
    f32 v20 = 0.0f;
    if(al::isFirstStep(this)) {
        // animation stuff
        rs::landGroundPoseAndSnap(mActor, mTrigger, &v20, mCollider, mConst);
    }
    rs::waitGround(mActor, mCollider, mConst->getGravity()+v20, mConst->getFallSpeedMax(), mConst->getSlerpQuatRateWait(), mConst->getWaitPoseDegreeMax());

    WARN_UNIMPL;
    // switch to `NrvWait` if animation is ended
}
void PlayerStateWait::exeWait() {
    tryUpdateAreaAnim();
    if(tryChangeRequestAnim() || tryChangeAreaAnim())
        return;

    if(al::isFirstStep(this)) {
        // animation stuff
    }
    sead::Vector3f v20 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrGravityDir(&v20, mActor, mCollider);
    rs::waitGround(mActor, mCollider, mConst->getGravity(), mConst->getFallSpeedMax(), mConst->getSlerpQuatRateWait(), mConst->getWaitPoseDegreeMax());

    // switch to other nerves on something
}
void PlayerStateWait::exeWaitConnect2D() {
    CRASH
}
void PlayerStateWait::exeWaitRelaxStart() {
    CRASH
}
void PlayerStateWait::exeWaitRelax() {
    CRASH
}
void PlayerStateWait::exeWaitSnoozeStart() {
    CRASH
}
void PlayerStateWait::exeWaitSnooze() {
    CRASH
}
void PlayerStateWait::exeWaitSleepStart() {
    CRASH
}
void PlayerStateWait::exeWaitSleep() {
    CRASH
}
void PlayerStateWait::exeAreaAnimTurn() {
    CRASH
}
void PlayerStateWait::exeAreaAnimWait() {
    CRASH
}
void PlayerStateWait::exeRequestAnimWait() {
    CRASH
}
