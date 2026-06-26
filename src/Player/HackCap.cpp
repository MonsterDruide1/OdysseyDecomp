#include "Player/HackCap.h"

#include "Player/IUsePlayerCollision.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Project/Controller/PadRumbleKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/CapTargetInfo.h"
#include "Player/HackCapAboveGroundChecker.h"
#include "Player/HackCapJudgePreInputSeparateJump.h"
#include "Player/HackCapJudgePreInputSeparateThrow.h"
#include "Player/HackCapJointControlKeeper.h"
#include "Player/HackCapStateHide.h"
#include "Player/HackCapStateThrowStay.h"
#include "Player/HackCapThrowParam.h"
#include "Player/HackCapTrigger.h"
#include "Player/PlayerAreaChecker.h"
#include "Player/PlayerCapActionHistory.h"
#include "Player/PlayerColliderHackCap.h"
#include "Player/PlayerEyeSensorHitHolder.h"
#include "Player/PlayerExternalVelocity.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerJointControlKeeper.h"
#include "Player/PlayerPushReceiver.h"
#include "Player/PlayerSeparateCapFlag.h"
#include "Player/PlayerWallActionHistory.h"

namespace {
NERVE_IMPL(HackCap, Catch);
NERVE_IMPL(HackCap, ThrowStay);
NERVE_IMPL(HackCap, Hide);
NERVE_IMPL(HackCap, Return);
NERVE_IMPL(HackCap, LockOn);
NERVE_IMPL(HackCap, Rebound);
NERVE_IMPL(HackCap, Hack);
NERVE_IMPL(HackCap, SpinAttack);
NERVE_IMPL(HackCap, ThrowStart);
NERVE_IMPL(HackCap, Rescue);
NERVE_IMPL_(HackCap, TrampleReturn, Trample);
NERVE_IMPL(HackCap, ThrowTornado);
NERVE_IMPL(HackCap, ThrowSpiral);
NERVE_IMPL(HackCap, ThrowRolling);
NERVE_IMPL(HackCap, Throw);
NERVE_IMPL(HackCap, ThrowBrake);
NERVE_IMPL_(HackCap, Rethrow, Throw);
NERVE_IMPL(HackCap, ThrowAppend);
NERVE_IMPL(HackCap, ThrowRollingBrake);
NERVE_IMPL(HackCap, Trample);
NERVE_IMPL(HackCap, TrampleLockOn);
NERVE_IMPL(HackCap, Blow);

NERVES_MAKE_STRUCT(HackCap, Catch, ThrowStay, Hide, Return, LockOn, Rebound, Hack, SpinAttack,
                   ThrowStart, Rescue, TrampleReturn, ThrowTornado, ThrowSpiral, ThrowRolling,
                   Throw, ThrowBrake, Rethrow, ThrowAppend, ThrowRollingBrake, Trample,
                   TrampleLockOn, Blow);
}  // namespace

HackCap::HackCap(const al::LiveActor* playerActor, const char* typeName, const PlayerInput* input,
                 const PlayerAreaChecker* playerAreaChecker,
                 const PlayerWallActionHistory* playerWallActionHistory,
                 const PlayerCapActionHistory* playerCapActionHistory,
                 const PlayerEyeSensorHitHolder* playerEyeSensorHitHolder,
                 const PlayerSeparateCapFlag* playerSeparateCapFlag,
                 const IUsePlayerCollision* playerCollision,
                 const IUsePlayerHeightCheck* playerHeightCheck,
                 const PlayerWetControl* playerWetControl,
                 const PlayerJointControlKeeper* playerJointControlKeeper,
                 HackCapJudgePreInputSeparateThrow* capJudgePreInputSeparateThrow,
                 HackCapJudgePreInputSeparateJump* capJudgePreInputSeparateJump)
    : al::LiveActor(typeName), mPlayerActor(playerActor), mTypeName(typeName),
      mPlayerAreaChecker(playerAreaChecker),
      mPlayerWallActionHistory(playerWallActionHistory),
      mPlayerCapActionHistory(playerCapActionHistory),
      mPlayerSeparateCapFlag(playerSeparateCapFlag), mPlayerCollision(playerCollision),
      mPlayerHeightCheck(playerHeightCheck), mPlayerWetControl(playerWetControl),
      mPlayerJointControlKeeper(playerJointControlKeeper),
      mCapJudgePreInputSeparateThrow(capJudgePreInputSeparateThrow),
      mCapJudgePreInputSeparateJump(capJudgePreInputSeparateJump) {
    mCapTargetInfo2 = new CapTargetInfo();
    mPlayerColliderHackCap = new PlayerColliderHackCap(this);
    _2a2_flag = true;
}

void HackCap::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mTypeName, nullptr);
    al::initNerve(this, &NrvHackCap.Hide, 0);
    al::invalidateClipping(this);
    makeActorAlive();
}

void HackCap::hide(bool isHide) {
    mIsHide = isHide;
    if (isHide)
        al::hideModelIfShow(this);
    else
        al::showModelIfHide(this);
}

void HackCap::movement() {
    al::LiveActor::movement();
    mHackCapAboveGroundChecker->update();
    mCapJudgePreInputSeparateThrow->update();
    mCapJudgePreInputSeparateJump->update();
    mPlayerColliderHackCap->update();
    mHackCapJointControlKeeper->update();
}

void HackCap::updateShadowMaskOffset() {
    // NON_MATCHING
}

void HackCap::control() {
    // NON_MATCHING
}

bool HackCap::isFlying() const {
    return al::isNerve(this, &NrvHackCap.Throw) || al::isNerve(this, &NrvHackCap.ThrowBrake) ||
           al::isNerve(this, &NrvHackCap.ThrowSpiral) || al::isNerve(this, &NrvHackCap.ThrowTornado) ||
           al::isNerve(this, &NrvHackCap.ThrowRolling) ||
           al::isNerve(this, &NrvHackCap.ThrowRollingBrake) || al::isNerve(this, &NrvHackCap.ThrowStay) ||
           al::isNerve(this, &NrvHackCap.ThrowAppend) || al::isNerve(this, &NrvHackCap.Blow) ||
           al::isNerve(this, &NrvHackCap.Rebound) || al::isNerve(this, &NrvHackCap.Return);
}

void HackCap::updateTargetLayout() {
    // NON_MATCHING
}

void HackCap::updateCollider() {
    al::LiveActor::updateCollider();
}

void HackCap::updateFrameOutLayout() {
    // NON_MATCHING
}

void HackCap::attackSpin(al::HitSensor* self, al::HitSensor* other, f32) {
    // NON_MATCHING
}

void HackCap::prepareLockOn(al::HitSensor* sensor) {
    // NON_MATCHING
}

bool HackCap::sendMsgStartHack(al::HitSensor* source) {
    // NON_MATCHING
    return true;
}

bool HackCap::receiveRequestTransferHack(al::HitSensor* source, al::HitSensor* target) {
    // NON_MATCHING
    return false;
}

void HackCap::startThrowSeparatePlayHack(al::HitSensor* sensor, const sead::Vector3f&,
                                         const sead::Vector3f&, f32) {
    // NON_MATCHING
}

void HackCap::startHack() {
    al::setNerve(this, &NrvHackCap.Hack);
}

void HackCap::emitHackStartEffect() {
    // NON_MATCHING
}

void HackCap::noticeHackMarioEnter() {
    // NON_MATCHING
}

void HackCap::noticeHackDemoPuppetableEnd() {
    // NON_MATCHING
}

void HackCap::recordHack() {
    // NON_MATCHING
}

void HackCap::addHackStartDemo() {
    // NON_MATCHING
}

void HackCap::addLockOnKeepDemo() {
    // NON_MATCHING
}

void HackCap::syncHackDamageVisibility(bool visible) {
    mIsHackDamageVisible = visible;
}

void HackCap::endHack() {
    al::setNerve(this, &NrvHackCap.Hide);
}

void HackCap::startSpinAttack(const char*) {
    al::setNerve(this, &NrvHackCap.SpinAttack);
}

void HackCap::startThrow(bool, const sead::Vector3f&, const sead::Vector3f&, f32,
                         const sead::Vector2f&, const sead::Vector2f&, const sead::Vector3f&, bool,
                         const sead::Vector3f&, SwingHandType, bool, f32, s32) {
    al::setNerve(this, &NrvHackCap.Throw);
}

void HackCap::startThrowSeparatePlay(const sead::Vector3f&, const sead::Vector3f&, f32, bool) {
    al::setNerve(this, &NrvHackCap.Throw);
}

void HackCap::startThrowSeparatePlayJump(const sead::Vector3f&, const sead::Vector3f&, f32) {
    al::setNerve(this, &NrvHackCap.Throw);
}

void HackCap::startCatch(const char*, bool, const sead::Vector3f&) {
    al::setNerve(this, &NrvHackCap.Catch);
}

bool HackCap::isNoPutOnHide() const {
    return al::isNerve(this, &NrvHackCap.Hide) && !_2a2_flag;
}

void HackCap::forcePutOn() {
    hide(false);
    al::setNerve(this, &NrvHackCap.Hide);
}

void HackCap::forceHack(al::HitSensor* sensor, const CapTargetInfo* info) {
    // NON_MATCHING
}

void HackCap::resetLockOnParam() {
    // NON_MATCHING
    mLockOnCounter = 0;
}

void HackCap::setupStartLockOn() {
    // NON_MATCHING
}

bool HackCap::cancelCapState() {
    // NON_MATCHING
    return true;
}

bool HackCap::isEnableThrow() const {
    return al::isNerve(this, &NrvHackCap.SpinAttack);
}

bool HackCap::isEnableSpinAttack() const {
    return !mIsHide && al::isNerve(this, &NrvHackCap.Hide);
}

bool HackCap::isSpinAttack() const {
    return al::isNerve(this, &NrvHackCap.SpinAttack);
}

bool HackCap::tryReturn(bool, bool* returnResult) {
    // NON_MATCHING
    if (returnResult)
        *returnResult = true;
    return true;
}

void HackCap::updateCapPose() {
    // NON_MATCHING
}

void HackCap::followTarget() {
    // NON_MATCHING
}

void HackCap::syncPuppetSilhouette() {
    // NON_MATCHING
}

void HackCap::recordCapJump(PlayerWallActionHistory* history) {
    // NON_MATCHING
}

f32 HackCap::getFlyingSpeedMax() const {
    return mHackCapThrowParam ? 100.0f : 0.0f;
}

f32 HackCap::getThrowSpeed() const {
    return mHackCapThrowParam ? 60.0f : 0.0f;
}

bool HackCap::requestLockOnHitReaction(const CapTargetInfo* info, const char* name) {
    // NON_MATCHING
    return true;
}

void HackCap::startPuppet() {
    mPuppetFlags = 1;
    mIsHidePuppetCapSilhouette = false;
}

void HackCap::endPuppet() {
    mPuppetFlags = 0;
    mIsHidePuppetCapSilhouette = false;
    syncPuppetSilhouette();
}

void HackCap::hidePuppetCap() {
    // NON_MATCHING
}

void HackCap::showPuppetCap() {
    // NON_MATCHING
}

void HackCap::hidePuppetCapSilhouette() {
    mIsHidePuppetCapSilhouette = true;
}

void HackCap::showPuppetCapSilhouette() {
    mIsHidePuppetCapSilhouette = false;
}

void HackCap::startPuppetCheckpointWarp() {
    // NON_MATCHING
}

void HackCap::startHackShineGetDemo() {
    // NON_MATCHING
}

void HackCap::endHackThrowAndReturnHack() {
    // NON_MATCHING
}

void HackCap::endHackShineGetDemo() {
    // NON_MATCHING
}

void HackCap::calcHackFollowTrans(sead::Vector3f* out, bool) const {
    if (out)
        *out = al::getTrans(this);
}

void HackCap::makeFollowMtx(sead::Matrix34f* out) const {
    if (out)
        al::makeMtxRT(out, this);
}

void HackCap::updateCapEyeShowHide(bool, s32) {
    // NON_MATCHING
}

void HackCap::activateInvincibleEffect() {
    // NON_MATCHING
}

void HackCap::syncInvincibleEffect(bool) {
    // NON_MATCHING
}

void HackCap::updateSeparateMode(const PlayerSeparateCapFlag* flag) {
    mIsSeparateFlying = flag ? flag->isSeparateMode() : false;
}

bool HackCap::isEnableRescuePlayer() const {
    return false;
}

bool HackCap::isRescuePlayer() const {
    return al::isNerve(this, &NrvHackCap.Rescue);
}

bool HackCap::isEnableHackThrow(bool*) const {
    // NON_MATCHING
    return false;
}

bool HackCap::isSeparateHipDropLand() const {
    return false;
}

bool HackCap::isSeparateHide() const {
    return mIsHide;
}

bool HackCap::isSeparateThrowFlying() const {
    return mIsSeparateFlying;
}

void HackCap::startRescuePlayer() {
    al::setNerve(this, &NrvHackCap.Rescue);
}

void HackCap::prepareCooperateThrow() {
    // NON_MATCHING
}

void HackCap::requestForceFollowSeparateHide() {
    // NON_MATCHING
}

f32 HackCap::calcSeparateHideSpeedH(const sead::Vector3f&) const {
    return 0.0f;
}

void HackCap::updateModelAlphaForSnapShot() {
    // NON_MATCHING
}

s32 HackCap::getPadRumblePort() const {
    return mPadRumbleKeeper->getPort();
}

bool HackCap::isEnableThrowSeparate() const {
    return mIsSeparateFlying;
}

bool HackCap::isHoldInputKeepLockOn() const {
    if (_298_1)
        return mInput->isHoldAction();
    return mInput->isHoldCapAction();
}

bool HackCap::isRequestableReturn() const {
    return isFlying();
}

bool HackCap::isLockOnEnableHackTarget() const {
    return false;
}

bool HackCap::isWaitHackLockOn() const {
    return false;
}

bool HackCap::isCatched() const {
    return (al::isNerve(this, &NrvHackCap.Catch) || al::isNerve(this, &NrvHackCap.Hide)) &&
           mIsHackDamageVisible;
}

bool HackCap::isHide() const {
    return al::isNerve(this, &NrvHackCap.Hide);
}

bool HackCap::isPutOn() const {
    return al::isNerve(this, &NrvHackCap.Hide) && _2a2_flag;
}

bool HackCap::isLockOnInterpolate() const {
    return (al::isNerve(this, &NrvHackCap.LockOn) || al::isNerve(this, &NrvHackCap.Hack) ||
            al::isNerve(this, &NrvHackCap.TrampleLockOn)) &&
           mLockOnCounter < 4;
}

bool HackCap::isEnablePreInput() const {
    return mCapJudgePreInputSeparateThrow &&
           mCapJudgePreInputSeparateThrow->isEnablePreInput();
}

bool HackCap::isForceCapTouchJump() const {
    if (_298_1)
        return false;
    u32 flag = *reinterpret_cast<const u32*>(mPlayerSeparateCapFlag);
    if ((flag & 0xff0000) == 0 && (flag & 0xff) == 0)
        return false;
    return reinterpret_cast<const HackCapStateThrowStay*>(mInput)->isHomingPlayerJump();
}

bool HackCap::isHackInvalidSeparatePlay() const {
    return mCapTargetInfo1 && mCapTargetInfo1->_7e;
}

void HackCap::exeHide() {
    // NON_MATCHING
}

void HackCap::exeLockOn() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::updateThrowJoint() {
    // NON_MATCHING
}

void HackCap::exeHack() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::exeSpinAttack() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::exeCatch() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::exeTrample() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::isHoldSpinCapStay() const {
    if (_298_1)
        return mInput->isHoldAction();
    u32 flag = *reinterpret_cast<const u32*>(mPlayerSeparateCapFlag);
    if ((flag & 0xff0000) != 0 || (flag & 0xff) != 0)
        return _510;
    return mInput->isHoldSpinCap();
}

void HackCap::exeTrampleLockOn() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::exeRescue() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}



void HackCap::exeThrowStart() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::isThrowTypeSpiral() const {
    // NON_MATCHING
    return false;
}

bool HackCap::isThrowTypeRolling() const {
    // NON_MATCHING
    return false;
}

void HackCap::setupThrowStart() {
    // NON_MATCHING
}

f32 HackCap::getThrowHeight() const {
    return mHackCapThrowParam ? 100.0f : 0.0f;
}

bool HackCap::checkEnableThrowStartSpace(sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                                         const sead::Vector3f&, f32, f32, bool,
                                         const sead::Vector3f&) {
    return true;
}

void HackCap::updateWaterArea() {
    // NON_MATCHING
}

f32 HackCap::getThrowRange() const {
    return mHackCapThrowParam ? 500.0f : 0.0f;
}

s32 HackCap::getThrowBrakeTime() const {
    return mHackCapThrowParam ? 10 : 0;
}

void HackCap::startThrowCapEyeThrowAction() {
    // NON_MATCHING
}

void HackCap::exeThrow() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryCollideReflectReaction() {
    // NON_MATCHING
    return false;
}

bool HackCap::tryCollideWallReaction() {
    // NON_MATCHING
    return false;
}

bool HackCap::changeThrowParamInWater(s32, bool) {
    // NON_MATCHING
    return false;
}

void HackCap::addCurveOffset() {
    // NON_MATCHING
}

void HackCap::exeThrowBrake() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryAppendAttack() {
    // NON_MATCHING
    return false;
}

void HackCap::exeThrowSpiral() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryCollideWallReactionSpiral() {
    // NON_MATCHING
    return false;
}

void HackCap::endThrowSpiral() {
    // NON_MATCHING
}

void HackCap::exeThrowTornado() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryCollideWallReactionReflect() {
    // NON_MATCHING
    return false;
}

void HackCap::exeThrowRolling() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryCollideWallReactionRollingGround() {
    // NON_MATCHING
    return false;
}

void HackCap::rollingGround() {
    // NON_MATCHING
}

void HackCap::exeThrowRollingBrake() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::exeThrowStay() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryChangeSeparateThrow() {
    // NON_MATCHING
    return false;
}

f32 HackCap::getThrowBackSpeed() const {
    return mHackCapThrowParam ? 30.0f : 0.0f;
}

void HackCap::updateLavaSurfaceMove() {
    // NON_MATCHING
}

bool HackCap::tryCollideWallReactionStay() {
    // NON_MATCHING
    return false;
}

bool HackCap::isEnableHackThrowAutoCatch() const {
    return false;
}

s32 HackCap::getThrowStayTime() const {
    return mHackCapThrowParam ? 120 : 0;
}

s32 HackCap::getThrowStayTimeMax() const {
    return mHackCapThrowParam ? 300 : 0;
}

void HackCap::exeThrowAppend() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

f32 HackCap::getThrowSpeedAppend() const {
    return mHackCapThrowParam ? 80.0f : 0.0f;
}

f32 HackCap::getThrowRangeAppend() const {
    return mHackCapThrowParam ? 800.0f : 0.0f;
}

void HackCap::exeBlow() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

bool HackCap::tryCollideWallLockOn() {
    // NON_MATCHING
    return false;
}

void HackCap::endHackThrowAndReturnHackOrHide() {
    if (mIsSeparateFlying)
        endHackThrowAndReturnHack();
    else
        hide(false);
}

void HackCap::clearThrowType() {
    // NON_MATCHING
}

void HackCap::exeRebound() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::exeReturn() {
    // NON_MATCHING
    if (al::isFirstStep(this)) {
    }
}

void HackCap::calcReturnTargetPos(sead::Vector3f* out) const {
    if (out)
        *out = al::getTrans(mPlayerActor);
}

void HackCap::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    // NON_MATCHING
}

bool HackCap::stayRollingOrReflect() {
    return false;
}

bool HackCap::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    // NON_MATCHING
    return false;
}

void HackCap::endMove() {
    // NON_MATCHING
}

bool HackCap::isEnableCapTouchJumpInput() const {
    return false;
}

void HackCap::prepareTransferLockOn(al::HitSensor*) {
    // NON_MATCHING
}

void HackCap::collideThrowStartArrow(al::HitSensor*, const sead::Vector3f&, const sead::Vector3f&,
                                     const sead::Vector3f&) {
    // NON_MATCHING
}

bool HackCap::trySendAttackCollideAndReaction(bool*) {
    // NON_MATCHING
    return false;
}

bool HackCap::stayWallHit() {
    // NON_MATCHING
    return false;
}

void HackCap::endHackThrow() {
    mIsSeparateFlying = false;
    mPlayerBodySensor = nullptr;
}
