#include "Player/HackCap.h"

#include <prim/seadDelegate.h>

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Controller/InputFunction.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Controller/PadRumbleDirector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Obj/PartsModel.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Obj/ActorDitherAnimator.h"
#include "Player/HackCapAboveGroundChecker.h"
#include "Player/IUsePlayerHeightCheck.h"
#include "Player/HackCapJointControlKeeper.h"
#include "Player/HackCapFunction.h"
#include "Player/HackCapJudgePreInputSeparateThrow.h"
#include "Player/HackCapTrigger.h"
#include "Player/PlayerAreaChecker.h"
#include "Player/PlayerCapActionHistory.h"
#include "Player/PlayerWallActionHistory.h"
#include "Player/PlayerColliderHackCap.h"
#include "Player/PlayerEyeSensorHitHolder.h"
#include "Player/PlayerWetControl.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/JudgeUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorParamHolder.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Player/CapTargetInfo.h"
#include "Player/HackCapStateHide.h"
#include "Player/HackCapStateThrowStay.h"
#include "Player/HackCapThrowParam.h"
#include "Player/PlayerCapFunction.h"
#include "Player/PlayerExternalVelocity.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerJointControlKeeper.h"
#include "Player/PlayerPushReceiver.h"
#include "Player/PlayerSeparateCapFlag.h"
#include "Project/Controller/PadRumbleKeeper.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/ScenePlayerCameraFunction.h"
#include "Util/SensorMsgFunction.h"
#include "Util/SpecialBuildUtil.h"

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
NERVE_END_IMPL(HackCap, ThrowSpiral);
NERVE_IMPL(HackCap, ThrowRolling);
NERVE_IMPL(HackCap, Throw);
NERVE_IMPL(HackCap, ThrowBrake);
NERVE_IMPL_(HackCap, Rethrow, Throw);
NERVE_IMPL(HackCap, ThrowAppend);
NERVE_IMPL(HackCap, ThrowRollingBrake);
NERVE_IMPL(HackCap, Trample);
NERVE_IMPL(HackCap, TrampleLockOn);
NERVE_IMPL(HackCap, Blow);

// Nerve order reconstructed from the nerve execute functions' addresses in
// HackCap.o and verified against isNerve/setNerve call sites (e.g. Hide at
// +0x10, LockOn at +0x20, Hack at +0x30, Rescue at +0x48, Throw at +0x70,
// Blow at +0xa8).
NERVES_MAKE_STRUCT(HackCap, Catch, ThrowStay, Hide, Return, LockOn, Rebound, Hack, SpinAttack,
                   ThrowStart, Rescue, TrampleReturn, ThrowTornado, ThrowSpiral, ThrowRolling,
                   Throw, ThrowBrake, Rethrow, ThrowAppend, ThrowRollingBrake, Trample,
                   TrampleLockOn, Blow);

// Throw range in the separate-throw state (flag _00 set, _02 clear),
// indexed by _29a. Values from .rodata at 0x1886a78.
const f32 sThrowRange[2] = {700.0f, 550.0f};

// Throw-range append per throw type, indexed by _2a5[0].
// Values from .rodata at 0x1886a80.
const f32 sThrowRangeAppend[5] = {500.0f, 300.0f, 500.0f, 200.0f, 180.0f};
// Throw-joint rotate-Y base, indexed by _4d0.
// Values from .rodata at 0x1886a28.
const f32 sThrowJointRotateY[2] = {35.0f, 45.0f};

// Aligns Cappy's pose to the player's during the spin attack: if the
// ground-up direction and the player's front direction are (nearly)
// parallel, Cappy just copies the player's pose; otherwise a matrix is
// built from the ground-up, front and player position. (Unlabeled helper
// at 0x3fee60 in HackCap.o; called by exeSpinAttack. Marked noinline so
// it stays a separate function like the original: exeSpinAttack is just
// the two member loads plus a tail call to it.)
__attribute__((noinline)) void updateSpinAttackPose(HackCap* hackCap,
                                                   const al::LiveActor* playerActor,
                                                   const PlayerJointControlKeeper* jointControlKeeper) {
    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, playerActor);
    sead::Vector3f groundPoseUp = sead::Vector3f::zero;
    jointControlKeeper->calcGroundPoseUp(&groundPoseUp);
    if (al::isParallelDirection(groundPoseUp, frontDir)) {
        al::copyPose(hackCap, playerActor);
    } else {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        al::makeMtxUpFrontPos(&mtx, groundPoseUp, frontDir, al::getTrans(playerActor));
        al::updatePoseMtx(hackCap, &mtx);
    }
}

// Collision-info getter (unlabeled helper at 0x409abc in HackCap.o): tries
// wall, then ceiling, then ground. For each surface that is collided, it
// gets the sensor and calls the delegate (sendMsgCapAttackCollide) with
// (sensor, attackSensor); if the delegate accepts, it outputs the collision
// parts, the surface normal and the surface position, and returns true.
// Returns false if no surface accepts.
__attribute__((noinline)) bool tryGetCollidedInfo(
    const al::CollisionParts** outParts, sead::Vector3f* outNormal, sead::Vector3f* outPos,
    const IUsePlayerCollision* collider, al::HitSensor* attackSensor,
    const sead::Delegate2RFunc<al::HitSensor*, al::HitSensor*, bool>* delegate) {
    al::HitSensor* sensor;
    if (rs::isCollidedWall(collider)) {
        sensor = rs::tryGetCollidedWallSensor(collider);
        if ((*delegate)(sensor, attackSensor)) {
            *outParts = rs::getCollidedWallCollisionParts(collider);
            __builtin_memcpy(outNormal, &rs::getCollidedWallNormal(collider),
                             sizeof(sead::Vector3f));
            __builtin_memcpy(outPos, &rs::getCollidedWallPos(collider), sizeof(sead::Vector3f));
            return true;
        }
    }
    if (rs::isCollidedCeiling(collider)) {
        sensor = rs::tryGetCollidedCeilingSensor(collider);
        if ((*delegate)(sensor, attackSensor)) {
            *outParts = rs::getCollidedCeilingCollisionParts(collider);
            __builtin_memcpy(outNormal, &rs::getCollidedCeilingNormal(collider),
                             sizeof(sead::Vector3f));
            __builtin_memcpy(outPos, &rs::getCollidedCeilingPos(collider), sizeof(sead::Vector3f));
            return true;
        }
    }
    if (rs::isCollidedGround(collider)) {
        sensor = rs::tryGetCollidedGroundSensor(collider);
        if ((*delegate)(sensor, attackSensor)) {
            *outParts = rs::getCollidedGroundCollisionParts(collider);
            __builtin_memcpy(outNormal, &rs::getCollidedGroundNormal(collider),
                             sizeof(sead::Vector3f));
            __builtin_memcpy(outPos, &rs::getCollidedGroundPos(collider), sizeof(sead::Vector3f));
            return true;
        }
    }
    return false;
}

// Spin-attack lock-on gate (unlabeled helper at 0x3fbfcc in HackCap.o;
// called by attackSpin). Returns true when the direction from `self` to
// `other` is more than 90 degrees away from the player's front, i.e. the
// target is behind the player, so the spin attack should not lock on to
// it. The cos(pi/2) threshold reproduces the original's cosf call plus the
// folded constant, the same pattern as tryReflectThrow's sin(pi/3).
__attribute__((noinline)) bool isTargetBehindPlayer(const al::LiveActor* playerActor,
                                                    const al::HitSensor* self,
                                                    const al::HitSensor* other) {
    sead::Vector3f dir = sead::Vector3f::zero;
    if (!al::calcDirBetweenSensorsH(&dir, self, other))
        return false;
    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, playerActor);
    al::verticalizeVec(&frontDir, al::getGravity(playerActor), frontDir);
    if (!al::tryNormalizeOrZero(&frontDir))
        return false;
    if (dir.dot(frontDir) < sead::Mathf::cos(1.5707964f))
        return true;
    return false;
}

// Raycast helper for calcThrowMove (unlabeled helper at 0x409f20 in HackCap.o;
// called by calcThrowMove when the bool flag param is true). Casts a ray from
// the actor's position along -dir3 (scaled by 200); if it hits a surface,
// reports the hit through `out` and decides whether the throw is close
// enough to the surface (via the height check) and aligned (via the dot
// checks) to count as a hit, updating the step counter. The counter is an
// s32 stored in the memory calcThrowMove receives as an f32* (the _28c
// member, declared f32 in HackCap.h but used as an int counter here and in
// calcThrowMove's calcRate01 call) -- the reinterpretation is real: the
// helper does converge(*counter, 5, 1) and stores ints into it, and
// calcThrowMove reads it back with scvtf (int-to-float).
__attribute__((noinline)) bool checkThrowMoveHit(
    bool* out, sead::Vector3f& hitPos, sead::Vector3f& hitNormal, s32* counter,
    const al::LiveActor* actor, const sead::Vector3f& dir1, const sead::Vector3f& dir2,
    const sead::Vector3f& dir3, f32 height) {
    *out = false;
    sead::Vector3f rayDir = dir3 * -200.0f;
    if (alCollisionUtil::getHitPosAndNormalOnArrow(actor, &hitPos, &hitNormal,
                                                   al::getTrans(actor), rayDir, nullptr,
                                                   nullptr)) {
        *out = true;
        sead::Vector3f diff = al::getTrans(actor) - hitPos;
        if (al::isNearZeroOrLess(diff.dot(dir3) - height)) {
            *counter = 0;
            return true;
        }
        if (dir1.dot(dir2) <= 0.0f && dir2.dot(hitNormal) > 0.0f) {
            *counter = 0;
            return true;
        }
    }
    *counter = al::converge(*counter, 5, 1);
    return false;
}

}  // namespace

// Reflect helper (unlabeled helper at 0x409624 in HackCap.o): reflects the
// throw direction/velocity off a surface. Returns false if the throw
// direction is too aligned with the surface normal (within 30 degrees of
// it, i.e. |dot| >= sin(60 deg)), or if the reflected direction cannot be
// normalized. Otherwise it moves the cap off the surface, updates dir/pos,
// sets the velocity to the reflected direction scaled by the original
// speed, and returns true. When the direction is more than 5 degrees off
// the surface normal and effects are enabled, it also plays the hit
// effect and the "HitCol" SE. It has external linkage (not in the anonymous
// namespace) so the compiler cannot constant-propagate the isEnableEffect
// flag from the single currently-implemented caller (which passes true);
// the original preserves the flag in a callee-saved register and tests it
// before the hit effect, like updateEquipmentHatVisibility's isKeepHat.
__attribute__((noinline)) bool tryReflectThrow(HackCap* cap, sead::Vector3f& dir,
                                              sead::Vector3f& pos, const sead::Vector3f& normal,
                                              const sead::Vector3f& hitPos, bool isEnableEffect) {
    if (sead::Mathf::abs(dir.dot(normal)) >= sead::Mathf::sin(1.0471975f))
        return false;
    sead::Vector3f velocity;
    __builtin_memcpy(&velocity, &al::getVelocity(cap), sizeof(sead::Vector3f));
    f32 speed = velocity.length();
    al::verticalizeVec(&velocity, normal, velocity);
    sead::Vector3f normalizedVel(0.0f, 0.0f, 0.0f);
    if (!al::tryNormalizeOrZero(&normalizedVel, velocity))
        return false;
    sead::Vector3f alongVec(0.0f, 0.0f, 0.0f);
    al::alongVectorNormalH(&alongVec, pos, dir, normalizedVel);
    if (!al::tryNormalizeOrZero(&alongVec))
        return false;
    *al::getTransPtr(cap) -= normal;
    if (sead::Mathf::abs(dir.dot(normal)) > 0.08715574f && isEnableEffect) {
        al::startHitReactionHitEffect(cap, "コリジョン衝突方向転換", hitPos);
        al::startSeWithParam(cap, sead::SafeString("HitCol"), sead::Mathf::abs(dir.dot(normal)),
                             "");
    }
    __builtin_memcpy(&dir, &normalizedVel, sizeof(sead::Vector3f));
    __builtin_memcpy(&pos, &alongVec, sizeof(sead::Vector3f));
    al::setVelocity(cap, normalizedVel * speed);
    return true;
}

// Manages the equipment hat and its depth shadow when Mario is hidden or
// shown during a hack: hides the hat (and kills it unless told not to)
// when hidden; restores it (and the depth shadow, per the target's
// depth-shadow flag) when shown. (Unlabeled helper at 0x409c78 in
// HackCap.o; called by exeHack and several other HackCap functions.
// Marked noinline so it stays a separate function like the original.
// It has external linkage (not in the anonymous namespace) so the
// compiler cannot constant-propagate the isKeepHat flag from the single
// currently-implemented caller; the original has six callers with
// different flag values.)
__attribute__((noinline)) void updateEquipmentHatVisibility(bool isHide, al::LiveActor* hat,
                                                             al::LiveActor* hatDepthShadow,
                                                             CapTargetInfo* targetInfo, bool isKeepHat) {
    if (isHide) {
        if (al::isDead(hat))
            return;
        al::validateShadow(hat);
        if (al::isAlive(hatDepthShadow))
            hatDepthShadow->kill();
        al::hideModelIfShow(hat);
        if (isKeepHat)
            return;
        hat->kill();
    } else {
        if (al::isAlive(hat) && !al::isHideModel(hat))
            return;
        if (al::isDead(hat))
            hat->appear();
        al::showModelIfHide(hat);
        al::invalidateShadow(hat);
        if (targetInfo->isUseDepthShadow()) {
            if (!al::isDead(hatDepthShadow))
                return;
            hatDepthShadow->appear();
        } else {
            if (!al::isAlive(hatDepthShadow))
                return;
            hatDepthShadow->kill();
        }
    }
}

HackCap::HackCap(const al::LiveActor* playerActor, const char* name, const PlayerInput* input,
                 const PlayerAreaChecker* areaChecker, const PlayerWallActionHistory* wallHistory,
                 const PlayerCapActionHistory* capHistory, const PlayerEyeSensorHitHolder* eyeHolder,
                 const PlayerSeparateCapFlag* separateFlag, const IUsePlayerCollision* collision,
                 const IUsePlayerHeightCheck* heightCheck, const PlayerWetControl* wetControl,
                 const PlayerJointControlKeeper* jointKeeper,
                 HackCapJudgePreInputSeparateThrow* judgeThrow,
                 HackCapJudgePreInputSeparateJump* judgeJump)
    : al::LiveActor("とりつき帽子"),
      mEquipmentHat(nullptr),
      mEquipmentHatDepthShadow(nullptr),
      mLockOnCapEyes(nullptr),
      mThrowingHatEyes(nullptr),
      mPlayerActor(playerActor),
      mPlayerCollision(collision),
      mTypeName(name),
      mPlayerAreaChecker(areaChecker),
      mPlayerSeparateCapFlag(separateFlag),
      mPlayerHeightCheck(heightCheck),
      mPlayerWetControl(wetControl),
      mPlayerJointControlKeeper(jointKeeper),
      mPlayerColliderHackCap(nullptr),
      mWaterSurfaceFinder(nullptr),
      mPadRumbleKeeper(nullptr),
      mSelfPlayerWetControl(nullptr),
      mHackCapTrigger(nullptr),
      mActorDitherAnimator(nullptr),
      mHackCapAboveGroundChecker(nullptr),
      _1a0_arr(nullptr),
      _1a8(0),
      _1ac(0),
      _1b0(0),
      _1b4(0),
      _1b8_arr(nullptr),
      _1c0(0),
      _1c4(0),
      _1b8_current(0),
      _1d0(nullptr),
      mAttackSensor(nullptr),
      mSpiralTailPositions{sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 0.0f),
                           sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 0.0f)},
      mHackCapThrowParam(nullptr),
      mCapTargetInfo1(nullptr),
      mCapTargetInfo2(new CapTargetInfo()),
      _238(SwingHandType::Val0),
      _23c(0),
      _240(sead::Vector3f(0.0f, 0.0f, 0.0f)),
      _24c(sead::Vector3f(0.0f, 0.0f, 0.0f)),
      _258(sead::Vector3f(0.0f, 0.0f, 0.0f)),
      _264(0.0f),
      _268(sead::Vector2f(0.0f, 0.0f)),
      _270(0.0f),
      _274(0.0f),
      _278(0),
      _27c(0),
      _280(0.0f),
      _284(0),
      _288(0.0f),
      _28c(0.0f),
      _290(0),
      _294(0),
      _298(false),
      _299(false),
      _29a(false),
      _29b(false),
      _29c(false),
      _29d(false),
      _29e(false),
      _29f(false),
      _2a0{0, 0, 1, 0},
      mIsSeparateFlying(false),
      _2a5{},
      _2a8(false),
      _2a9{},
      mPlayerWallActionHistory(wallHistory),
      mPlayerCapActionHistory(capHistory),
      mInput(input),
      mPlayerEyeSensorHitHolder(const_cast<PlayerEyeSensorHitHolder*>(eyeHolder)),
      mCapEyeSensorHitHolder(new PlayerEyeSensorHitHolder(64)),
      _2d8(0.0f),
      _2dc(0),
      mHackCapJointControlKeeper(nullptr),
      _2e8(sead::Vector3f::zero),
      _2f4(sead::Vector3f::zero),
      _300(sead::Vector3f::zero),
      _30c(false),
      _30d(false),
      _30e(false),
      _30f(0),
      _310(sead::SafeStringBase<char>::cEmptyString),
      _3a8(sead::SafeStringBase<char>::cEmptyString),
      _440(sead::Matrix34f::ident),
      _470(sead::Matrix34f::ident),
      _4a0(sead::Matrix34f::ident),
      _4d0(false),
      _4d4(sead::Vector3f(0.0f, 0.0f, 0.0f)),
      _4e0(sead::Vector3f(0.0f, 0.0f, 0.0f)),
      _4ec(false),
      _4f0(0),
      mCapExternalVelocity(nullptr),
      mCapPushReceiver(nullptr),
      mPlayerBodySensor(nullptr),
      _510(false),
      _514(sead::Vector3f(0.0f, 0.0f, 0.0f)),
      mCapJudgePreInputSeparateThrow(judgeThrow),
      mCapJudgePreInputSeparateJump(judgeJump),
      _530(false),
      _534(0),
      mWallPos(sead::Vector3f::zero),
      mWallNormal(sead::Vector3f::zero),
      mWallVelocity(sead::Vector3f::zero),
      _560{},
      _580(0),
      mLockOnCounter(0),
      _588(sead::Matrix34f::ident),
      mIsHide(false),
      mIsHackDamageVisible(false),
      _5ba(false),
      _5bb(false),
      _5bc(0),
      mTargetLayout(nullptr),
      mTargetSensor(nullptr),
      mTargetSensorForLayout(nullptr),
      mTargetLayoutCounter(0),
      _5dc{},
      mOcclusionCullingJudge(nullptr),
      _5e8(false),
      _5e9{},
      _5f0(nullptr),
      mIsPuppet(false),
      _5f9(false),
      mIsHidePuppetCapSilhouette(false),
      mStateThrowStay(nullptr),
      mStateHide(nullptr) {
    void* buf1 = new (nullptr, 8, std::nothrow) void*[4];
    if (buf1) {
        _1b0 = 0;
        _1a0_arr = buf1;
        _1a8 = 4;
    }
    void* buf2 = new (nullptr, 8, std::nothrow) void*[8];
    if (buf2) {
        _1b8_current = 0;
        _1b8_arr = buf2;
        _1c0 = 8;
    }
}

void HackCap::init(const al::ActorInitInfo& info) {
    sead::FixedSafeString<128> capModelName;
    PlayerFunction::createCapModelName(&capModelName, mTypeName);
    al::Resource* resource = PlayerFunction::initCapModelActor(this, info, capModelName.cstr());

    mPlayerColliderHackCap = new PlayerColliderHackCap(this);
    mPlayerColliderHackCap->init();
    mThrowingHatEyes = al::getSubActor(this, "目(投げ帽子)");
    al::startAction(mThrowingHatEyes, "Appear");
    mHackCapThrowParam = new HackCapThrowParam(this);
    mAttackSensor = al::getHitSensor(this, "Attack");
    al::setHitSensorPosPtr(this, "SpiralTail1", &mSpiralTailPositions[0]);
    al::setHitSensorPosPtr(this, "SpiralTail2", &mSpiralTailPositions[1]);
    al::setHitSensorPosPtr(this, "SpiralTail3", &mSpiralTailPositions[2]);
    al::setHitSensorPosPtr(this, "SpiralTail4", &mSpiralTailPositions[3]);
    al::setHitSensorPosPtr(this, "SpiralTail5", &mSpiralTailPositions[4]);
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mSelfPlayerWetControl = new PlayerWetControl(this, this, mPlayerAreaChecker);
    mPadRumbleKeeper = al::createPadRumbleKeeper(this, 0);
    mHackCapTrigger = new HackCapTrigger();
    mActorDitherAnimator = al::ActorDitherAnimator::tryCreate(this, resource, nullptr);
    mHackCapAboveGroundChecker = new HackCapAboveGroundChecker(this);
    mHackCapJointControlKeeper = new HackCapJointControlKeeper();
    mHackCapJointControlKeeper->initCapJointControl(this);

    mEquipmentHat = new al::LiveActor("装着表示用帽子");
    al::initActorWithArchiveName(mEquipmentHat, info, sead::SafeString("MarioCap"), "Display");
    al::setEffectFollowPosPtr(mEquipmentHat, "InvincibleStart", al::getTransPtr(mEquipmentHat));
    al::setEffectFollowPosPtr(mEquipmentHat, "Invincible", al::getTransPtr(mEquipmentHat));
    mHackCapJointControlKeeper->initDisplayCapJointControl(mEquipmentHat);
    al::invalidateOcclusionQuery(mEquipmentHat);
    mEquipmentHat->makeActorDead();
    al::hideModelIfShow(mEquipmentHat);
    al::hideSilhouetteModelIfShow(mEquipmentHat);
    mEquipmentHatDepthShadow = al::getSubActor(mEquipmentHat, "デプスシャドウ");
    al::invalidateOcclusionQuery(mEquipmentHatDepthShadow);
    mLockOnCapEyes = al::getSubActor(mEquipmentHat, "ロックオン用キャップの目");
    al::invalidateOcclusionQuery(mLockOnCapEyes);
    al::startAction(mLockOnCapEyes, "Wait");

    al::setEffectFollowMtxPtr(this, "FlyRollingGrounding", &_440);
    al::setEffectFollowMtxPtr(this, "WaterSurfaceInOut", &_470);
    al::setEffectNamedMtxPtr(this, "WaterSurface", &_4a0);

    mCapExternalVelocity = new PlayerExternalVelocity(this, nullptr, nullptr);
    mCapExternalVelocity->mExternalDrag = {0.94f, 0.94f};
    mCapExternalVelocity->mAreaDrag = {0.94f, 0.94f};
    mCapExternalVelocity->mSensitivity = {0.7f, 0.7f};
    mCapPushReceiver = new PlayerPushReceiver(this);

    mTargetLayout = new al::SimpleLayoutAppearWaitEnd("ターゲット", "TargetMarker",
                                                        al::getLayoutInitInfo(info), nullptr, false);
    mOcclusionCullingJudge = al::getLayoutInitInfo(info).getOcclusionCullingJudge();
    _5f0 = new al::SimpleLayoutAppearWaitEnd("画面外帽子ガイド", "BalloonFrame",
                                              al::getLayoutInitInfo(info), nullptr, false);

    al::initNerve(this, &NrvHackCap.Catch, 2);
    mStateThrowStay = new HackCapStateThrowStay(this, mPlayerColliderHackCap, mPlayerActor,
                                                mPlayerSeparateCapFlag, mInput, mPlayerCollision,
                                                mCapEyeSensorHitHolder, mHackCapTrigger,
                                                mHackCapJointControlKeeper,
                                                mCapJudgePreInputSeparateJump, &_299);
    mStateHide = new HackCapStateHide(this, mPlayerColliderHackCap, mPlayerActor,
                                      mPlayerSeparateCapFlag, mInput,
                                      static_cast<al::PartsModel*>(mThrowingHatEyes),
                                      mHackCapJointControlKeeper);
    al::initNerveState(this, mStateThrowStay, &NrvHackCap.ThrowStay, "投げ中待機");
    al::initNerveState(this, mStateHide, &NrvHackCap.Hide, "隠れ");
    makeActorAlive();
    hide(false);
}

void HackCap::updateShadowMaskOffset() {
    if (!isThrow() && !al::isNerve(this, &NrvHackCap.Return) && !al::isNerve(this, &NrvHackCap.Rebound))
        return;
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    al::makeMtxRT(&mtx, this);
    f32 shadowLength =
        sead::Mathf::clamp(1.0f - sead::Mathf::abs(mtx.getBase(1).dot(sead::Vector3f::ey)), 0.0f, 1.0f) * 30.0f +
        10.0f;
    mtx.transpose();
    sead::Vector3f shadowOffset = sead::Vector3f::ey * -shadowLength;
    shadowOffset.rotate(mtx);
    f32 dropLength = 2000.0f;
    if (!mPlayerAreaChecker->isInShadowLength(&dropLength, al::getTrans(this))) {
        if (mHackCapAboveGroundChecker->_8)
            dropLength =
                sead::Mathf::min(2600.0f, sead::Mathf::max(0.0f, mHackCapAboveGroundChecker->_24) + 1.3f);
    }
    al::setShadowMaskOffset(this, shadowOffset, "本体");
    al::setShadowMaskDropLength(this, dropLength, "本体");
}

bool HackCap::isFlying() const {
    if (isThrow())
        return true;
    if (al::isNerve(this, &NrvHackCap.Return))
        return true;
    return al::isNerve(this, &NrvHackCap.Rebound);
}

void HackCap::startThrowSeparatePlayJump(const sead::Vector3f& dir, const sead::Vector3f& pos,
                                         f32 speed) {
    mHackCapTrigger->set(HackCapTrigger::EAfterMovementTrigger::Val0);
    startThrowSeparatePlay(dir, pos, speed, false);
}

void HackCap::startThrow(bool isSeparate, const sead::Vector3f& dir, const sead::Vector3f& pos,
                         f32 speed, const sead::Vector2f& vecA, const sead::Vector2f& vecB,
                         const sead::Vector3f& vecC, bool isFixed, const sead::Vector3f& vecD,
                         SwingHandType swingHand, bool isSeparateThrow, f32 paramF, s32 paramS) {
    al::showModelIfHide(this);
    sead::Vector3f throwDir = dir;
    f32 rate = al::calcRate01(_2d8 > 0.0f ? _2d8 : -_2d8, 2.0f, 9.0f);
    f32 maxThrowSpeed = rs::isKidsMode(this) ? 80.0f : 45.0f;
    bool isKidsA = rs::isKidsMode(this);
    bool isKidsB = rs::isKidsMode(this);
    f32 baseSpeed = rate * maxThrowSpeed;
    bool isDeepDown = mInput->isMoveDeepDown();
    f32 speedA = (baseSpeed + (isKidsB ? 30.0f : 20.0f)) * speed;
    speedA = speedA > maxThrowSpeed ? maxThrowSpeed : speedA;
    f32 speedB = (baseSpeed + (isKidsA ? 15.0f : 7.5f)) * speed;
    speedB = speedB > maxThrowSpeed ? maxThrowSpeed : speedB;
    if (!isDeepDown)
        speed = speedA;
    else
        speed = speedB;
    mPlayerEyeSensorHitHolder->findNearestSensorDirH(&throwDir, rs::getPlayerBodyPos(this), dir,
                                                     pos, speed, 0.0f);
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, -throwDir, pos);
    al::updatePoseQuat(this, quat);
    _514 = sead::Vector3f::zero;
    _2a5[2] = false;
    _530 = false;
    _534 = 0;
    _298 = false;
    _5e8 = false;
    _2a9[0] = false;
    _299 = isSeparateThrow;
    _510 = ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0) &
           ((mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0);
    mWallPos = sead::Vector3f::zero;
    mWallNormal = sead::Vector3f::zero;
    mWallVelocity = sead::Vector3f::zero;
    _560[3] = nullptr;
    _560[2] = nullptr;
    _560[1] = nullptr;
    _560[0] = nullptr;
    _238 = swingHand;
    _23c = paramS;
    _29e = isSeparate;
    _29f = isFixed;
    _4d0 = !al::isNearZero(vecC, 0.001f);
    _4d4 = sead::Vector3f::zero;
    _4e0 = sead::Vector3f::zero;
    _240 = throwDir;
    _24c = pos;
    _258 = vecD;
    *mHackCapJointControlKeeper = HackCapJointControlKeeper();
    _28c = 0.0f;
    _29a = rs::isPlayerInWater(this);
    al::WaterSurfaceFinder* waterFinder = mWaterSurfaceFinder;
    const sead::Vector3f& trans = al::getTrans(this);
    sead::Vector3f negGravity = -al::getGravity(this);
    waterFinder->update(trans, negGravity, 100.0f);
    _29d = _29a;
    _29b = false;
    _29c = mWaterSurfaceFinder->isFoundSurface();
    mSelfPlayerWetControl->reset();
    if (!al::isNerve(this, &NrvHackCap.Hack))
        mSelfPlayerWetControl->_24 = mPlayerWetControl->_24;
    _278 = 0;
    _4ec = false;
    _4f0 = 0;
    mCapExternalVelocity->reset();
    _264 = 0.0f;
    _268 = sead::Vector2f::zero;
    if (mInput->isThrowTypeLeftRight(vecA))
        _264 = vecA.x > 0.0f ? -1.0f : 1.0f;
    if (_4d0)
        _264 = vecC.y > 0.0f ? 1.0f : -1.0f;
    if (mInput->isThrowTypeSpiral(vecB)) {
        if (vecB.x > 0.0f)
            _268 = sead::Vector2f::ex * -1.0f;
        else
            _268 = sead::Vector2f::ex;
    } else if (mInput->isThrowTypeRolling(vecB)) {
        if (vecB.y > 0.0f)
            _268 = sead::Vector2f::ey;
        else
            _268 = sead::Vector2f::ey * -1.0f;
    }
    _298 = _264 > 0.0f || _268.x > 0.0f;
    if (_2a0[1]) {
        mHackCapJointControlKeeper->mRotateZ = paramF;
        mHackCapJointControlKeeper->_c = 0;
        al::setShadowMaskIntensity(this, "東京", 0.2f);
        al::setShadowMaskDropLength(this, 2000.0f, "東京");
        al::setNerve(this, &NrvHackCap.ThrowAppend);
    } else {
        al::invalidateHitSensors(this);
        al::validateHitSensor(this, "Catch");
        al::hideModelIfShow(this);
        al::setNerve(this, &NrvHackCap.Rebound);
    }
}

void HackCap::startThrowSeparatePlayHack(al::HitSensor* sensor, const sead::Vector3f& dir,
                                         const sead::Vector3f& pos, f32 speed) {
    if (!al::isNerve(this, &NrvHackCap.Hack))
        return;
    mPlayerBodySensor = sensor;
    mIsSeparateFlying = true;
    mEquipmentHat->kill();
    mLockOnCapEyes->kill();
    startThrowSeparatePlay(dir, pos, speed, false);
}

void HackCap::startThrowSeparatePlay(const sead::Vector3f& dir, const sead::Vector3f& pos,
                                     f32 speed, bool isSeparate) {
    sead::Vector3f hidePos(0.0f, 0.0f, 0.0f);
    if (al::isNerve(this, &NrvHackCap.Hide))
        mStateHide->calcSeparateThrowOffset(&hidePos);
    else {
        const sead::Vector3f& bodyPos = rs::getPlayerBodyPos(this);
        const sead::Vector3f& playerPos = rs::getPlayerPos(this);
        hidePos.x = bodyPos.x - playerPos.x;
        hidePos.y = bodyPos.y - playerPos.y;
        hidePos.z = bodyPos.z - playerPos.z;
    }
    al::showModelIfHide(this);
    al::startAction(mThrowingHatEyes, "Appear");
    al::setScaleAll(this, 1.0f);
    _2a0[1] = true;
    _2a0[0] = true;
    _2a0[2] = false;
    _2a0[3] = false;
    _2a8 = false;
    _299 = false;
    _2d8 = mInput->_50;
    _1b8_current = 0;
    _1c4 = 0;
    _1ac = 0;
    _1b0 = 0;
    _240 = sead::Vector3f::zero;
    _24c = sead::Vector3f::zero;
    al::copyPose(this, mPlayerActor);
    al::setVelocityZero(this);
    HackCapFunction::resetPositionAndCollision(this, mPlayerColliderHackCap);
    al::setNerve(this, &NrvHackCap.SpinAttack);
    startThrow(false, dir, pos, speed, sead::Vector2f::zero, sead::Vector2f::zero,
               sead::Vector3f::zero, isSeparate, hidePos, static_cast<SwingHandType>(0), false,
               0.0f, 1);
}

void HackCap::startSpinAttack(const char* actionName) {
    al::showModelIfHide(this);
    al::startAction(mThrowingHatEyes, "Appear");
    al::setScaleAll(this, 1.0f);
    // _2a0[0] = _2a0[1] = true, _2a0[2] = _2a0[3] = false, as a single
    // word store (0x101) like the original; the four separate byte
    // assignments do not merge into one store with this compiler.
    *reinterpret_cast<s32*>(_2a0) = 0x101;
    _2a8 = false;
    bool isSeparateThrow = false;
    if (actionName) {
        isSeparateThrow = (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                          (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
    }
    _299 = isSeparateThrow;
    _2d8 = mInput->_50;
    _1b8_current = 0;
    _1c4 = 0;
    _1ac = 0;
    _1b0 = 0;
    _240 = sead::Vector3f::zero;
    _24c = sead::Vector3f::zero;
    al::copyPose(this, mPlayerActor);
    al::setVelocityZero(this);
    if (actionName) {
        al::startAction(this, actionName);
        alPadRumbleFunction::startPadRumbleWithVolume(this, "EmBlowHitCommonS", 0.2f, 0.6f, -1);
    }
    HackCapFunction::resetPositionAndCollision(this, mPlayerColliderHackCap);
    al::setNerve(this, &NrvHackCap.SpinAttack);
}

bool HackCap::stayRollingOrReflect() {
    if (_299) {
        if (mInput->isHoldAction()) {
            _294 = 20;
            return stayWallHit();
        }
        al::setNerve(this, &NrvHackCap.Rebound);
        return true;
    }
    bool isSeparateThrow = (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                           (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
    if (!isSeparateThrow) {
        if (mInput->isHoldSpinCap()) {
            _294 = 20;
            return stayWallHit();
        }
        al::setNerve(this, &NrvHackCap.Rebound);
        return true;
    }
    if (mStateThrowStay->isEnableSendHipDropMsg())
        return true;
    if (_299) {
        if (mInput->isHoldAction())
            return true;
    } else if (!isSeparateThrow) {
        if (mInput->isHoldSpinCap())
            return true;
    } else if (_510) {
        return true;
    }
    al::setNerve(this, &NrvHackCap.Rebound);
    return true;
}

void HackCap::forcePutOn() {
    hide(true);
    al::tryKillEmitterAndParticleAll(this);
    _2a0[2] = true;
    _2a0[3] = true;
    static_cast<al::SimpleLayoutAppearWaitEnd*>(_5f0)->kill();
}

void HackCap::forceHack(al::HitSensor* sensor, const CapTargetInfo* targetInfo) {
    static_cast<al::SimpleLayoutAppearWaitEnd*>(_5f0)->kill();
    mCapExternalVelocity->reset();
    al::makeMtxRT(&_588, this);
    mCapTargetInfo1 = nullptr;
    _580 = 0;
    _2a5[1] = false;
    al::setVelocityZero(this);
    al::offCollide(this);
    al::invalidateHitSensors(this);
    al::validateHitSensor(this, "Trample");
    _29e = false;
    _274 = 0;
    _264 = 0.0f;
    _268.set(0.0f, 0.0f);
    _270 = 0.0f;
    al::tryStartActionIfNotPlaying(mThrowingHatEyes, _298 ? "SpinL" : "SpinR");
    _1d0 = sensor;
    mCapTargetInfo1 = const_cast<CapTargetInfo*>(targetInfo);
    al::startAction(this, "NoAction");
    al::LiveActor* equipmentHat = mEquipmentHat;
    al::LiveActor* equipmentHatDepthShadow = mEquipmentHatDepthShadow;
    al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
    al::startAction(equipmentHat, "NoAction");
    al::startAction(equipmentHatDepthShadow, "NoAction");
    if (al::isAlive(lockOnCapEyes) && !al::tryStartAction(lockOnCapEyes, "NoAction"))
        al::tryStartActionIfNotPlaying(lockOnCapEyes, "Wait");
    f32 frameMax = al::getActionFrameMax(this, "NoAction");
    al::setActionFrame(this, frameMax);
    al::setActionFrame(mEquipmentHat, frameMax);
    al::setActionFrame(mEquipmentHatDepthShadow, frameMax);
    al::setNerve(this, &NrvHackCap.LockOn);
    mIsHide = false;
    mIsHackDamageVisible = false;
    _5ba = false;
    _5bb = true;
    _5bc = 0;
    al::invalidateHitSensor(this, "Trample");
    al::setNerve(this, &NrvHackCap.Hack);
}

bool HackCap::cancelCapState() {
    if (al::isNerve(this, &NrvHackCap.SpinAttack) || al::isNerve(this, &NrvHackCap.Hide) ||
        al::isNerve(this, &NrvHackCap.Catch))
        goto checkHide;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 &&
        al::isNerve(this, &NrvHackCap.ThrowStay) && !mIsSeparateFlying &&
        al::calcDistance(this, mPlayerActor) <= 200.0f)
        goto checkHide;
    if (!al::isNerve(this, &NrvHackCap.SpinAttack))
        return false;
checkHide:
    if (al::isNerve(this, &NrvHackCap.Hide))
        return false;
    if (!al::isNerve(this, &NrvHackCap.ThrowStay) || _299 ||
        (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0 ||
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0) {
        hide(false);
        return true;
    }
    return false;
}

bool HackCap::isEnableHackThrow(bool* isEnable) const {
    if (!al::isNerve(this, &NrvHackCap.Hack))
        return false;
    *isEnable = !mCapTargetInfo1->_7e;
    return mCapTargetInfo1->mIsInvalidHackThrow == 0;
}

bool HackCap::isSeparateHipDropLand() const {
    if (rs::isJustLand(mPlayerColliderHackCap))
        return mStateThrowStay->isEnableSendHipDropMsg();
    return false;
}

bool HackCap::isRequestableReturn() const {
    if (!mCapTargetInfo1)
        return false;
    bool isRequestable = _2a5[1];
    if (!al::isNerve(this, &NrvHackCap.LockOn))
        return false;
    bool isLessStep = al::isLessEqualStep(this, 5);
    return isRequestable & (isLessStep ^ 1);
}

bool HackCap::isLockOnInterpolate() const {
    if (al::isNerve(this, &NrvHackCap.LockOn) || al::isNerve(this, &NrvHackCap.Hack) ||
        al::isNerve(this, &NrvHackCap.TrampleLockOn))
        return mLockOnCounter < 4;
    return false;
}

void HackCap::control() {
    _294 = al::converge(_294, 0, 1);
    if (_30c) {
        if (!isThrow() && !al::isNerve(this, &NrvHackCap.Return) &&
            !al::isNerve(this, &NrvHackCap.Rebound)) {
            al::tryDeleteEffect(this, "FlyRollingGrounding");
            _30c = false;
        }
        else if (al::isNerve(this, &NrvHackCap.Return)) {
            al::tryDeleteEffect(this, "FlyRollingGrounding");
            _30c = false;
        }
    }
    if (_30d && !isThrow() && !al::isNerve(this, &NrvHackCap.Return) &&
        !al::isNerve(this, &NrvHackCap.Rebound)) {
        al::tryDeleteEffect(this, _310.cstr());
        _30d = false;
        _310.copy(sead::SafeStringBase<char>::cEmptyString);
        al::tryStopSe(this, "FlyWaterSurface", -1, nullptr);
        al::tryStopSe(this, "FlyWaterSurfaceIce", -1, nullptr);
    }
    if (_30e && !al::isNerve(this, &NrvHackCap.ThrowStay)) {
        al::tryDeleteEffect(this, _3a8.cstr());
        _30e = false;
        _3a8.copy(sead::SafeStringBase<char>::cEmptyString);
        al::tryStopSe(this, "StayWaterSurface", -1, nullptr);
    }
    if (_530)
        _534 = al::converge(_534, 0, 1);
    updateTargetLayout();
}

void HackCap::prepareLockOn(al::HitSensor* sensor) {
    if (mIsSeparateFlying) {
        al::setNerve(this, &NrvHackCap.LockOn);
        rs::sendMsgTransferHack(mPlayerBodySensor, const_cast<al::HitSensor*>(_1d0));
        mIsSeparateFlying = false;
        mPlayerBodySensor = nullptr;
    }
    mCapExternalVelocity->reset();
    mCapTargetInfo1 = nullptr;
    _580 = 0;
    mLockOnCounter = 0;
    _2a5[1] = false;
    al::makeMtxRT(&_588, this);
    if (!rs::sendMsgInitCapTarget(sensor, mAttackSensor,
                                  (const CapTargetInfo**)&mCapTargetInfo1)) {
        CapTargetInfo* targetInfo = mCapTargetInfo2;
        al::LiveActor* host = al::getSensorHost(sensor);
        CapTargetInfoFunction::initIterCapTargetInfo(targetInfo, nullptr, host, nullptr);
        mCapTargetInfo1 = mCapTargetInfo2;
    }
    _1d0 = sensor;
    if (al::isNerve(this, &NrvHackCap.SpinAttack) || al::isNerve(this, &NrvHackCap.ThrowStart) ||
        al::isNerve(this, &NrvHackCap.Hide))
        al::startAction(this, "FlyingWaitR");
    _2a8 = mCapTargetInfo1->_7f;
    al::startHitReaction(this, "帽子セット");
    al::setVelocityZero(this);
    al::offCollide(this);
    al::invalidateHitSensors(this);
    al::validateHitSensor(this, "Trample");
    _29e = false;
    _264 = 0.0f;
    _268 = sead::Vector2f::zero;
    _270 = 0.0f;
    _274 = 0;
    if (_298)
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
    else
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
    al::setNerve(this, &NrvHackCap.LockOn);
}

void HackCap::exeThrowRollingBrake() {
    updateThrowJoint();
    if (changeThrowParamInWater(al::getNerveStep(this), true)) {
        al::setNerve(this, &NrvHackCap.ThrowRolling);
        return;
    }
    if (tryCollideReflectReaction())
        return;
    if (tryCollideWallReactionRollingGround())
        return;
    rollingGround();
    al::addVelocity(this, _240 * _280);
    al::scaleVelocityExceptDirection(this, _240, 0.94f);
    calcThrowRollingDirection(_240, _24c, mInput, mPlayerSeparateCapFlag, _299);
    if (tryAppendAttack()) {
        al::setNerve(this, &NrvHackCap.ThrowAppend);
        return;
    }
    const sead::Vector3f& velocity = al::getVelocity(this);
    if (!al::isNearZero(velocity, 0.001f) && !(_240.dot(velocity) < 0.0f))
        return;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && !_510) {
        al::setVelocityZero(this);
        al::startHitReaction(this, "おすそわけStay状態からのリターン");
        al::setNerve(this, &NrvHackCap.Return);
    } else {
        al::setNerve(this, &NrvHackCap.ThrowStay);
    }
}

void HackCap::exeThrowStart() {
    bool isNearZeroX = al::isNearZero(_268.x, 0.001f);
    bool isRollingThrow = al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f);
    bool isAfterMovementTrigger = mHackCapTrigger->isOn(HackCapTrigger::EAfterMovementTrigger::Val0);
    _290 = 0;
    _294 = 0;
    al::validateHitSensors(this);
    al::invalidateHitSensor(this, "SpiralTail1");
    al::invalidateHitSensor(this, "SpiralTail2");
    al::invalidateHitSensor(this, "SpiralTail3");
    al::invalidateHitSensor(this, "SpiralTail4");
    al::invalidateHitSensor(this, "SpiralTail5");
    al::validateShadow(this);
    al::onCollide(this);
    const char* actionNameL = _29e ? "FlyingWaitCapCatchL" : "FlyingWaitL";
    const char* actionNameR = _29e ? "FlyingWaitCapCatchR" : "FlyingWaitR";
    const char* action = _264 > 0.0f ? actionNameL : actionNameR;
    al::startAction(this, action);

    sead::Vector3f targetPos;
    sead::Vector3f throwPos;
    sead::Vector3f checkPos(0.0f, 0.0f, 0.0f);
    sead::Vector3f checkPos2(0.0f, 0.0f, 0.0f);
    f32 radius = 0.0f;
    f32 throwDist = 150.0f;

    if (isNearZeroX) {
        radius = mPlayerColliderHackCap->getColliderRadius();
        targetPos = al::getTrans(mPlayerActor) + _258;
        throwPos = targetPos + _24c * mHackCapThrowParam->mHackThrowHeight->value;
        checkPos = throwPos + _240 * 150.0f;

        bool isSeparateThrow = false;
        if (!_299)
            isSeparateThrow = (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                              (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
        f32 range = radius + throwDist;

        if (mIsSeparateFlying) {
            sead::Matrix34f mtx = sead::Matrix34f::ident;
            mCapTargetInfo1->makeLockOnMtx(&mtx);
            if (mCapTargetInfo1->mIsEscapeLocalOffset)
                al::calcTransLocalOffsetByMtx(&targetPos, mtx, mCapTargetInfo1->mEscapeLocalOffset);
            else
                targetPos.set(mtx.m[0][3], mtx.m[1][3], mtx.m[2][3]);
            throwPos = targetPos - _240 * 75.0f;
            checkPos2 = throwPos;
            checkPos = throwPos + _240 * 225.0f;
            radius = range + 75.0f;
            throwDist = 225.0f;
        } else if (isSeparateThrow) {
            if (isAfterMovementTrigger) {
                throwPos = checkPos = checkPos2 = targetPos;
            } else {
                f32 dotThrow = _240.dot(_258);
                f32 dotUp = _258.dot(_24c);
                f32 height = sead::Mathf::min(dotUp, mPlayerColliderHackCap->getColliderRadius());
                height = sead::Mathf::min(height, mHackCapThrowParam->mHackThrowHeight->value);
                targetPos = al::getTrans(mPlayerActor) + _240 * dotThrow;
                radius = range - dotThrow;
                throwPos = targetPos + _24c * height;
                checkPos = throwPos + _240 * 150.0f;
            }
        } else if (isRollingThrow) {
            if (_268.y <= 0.0f && mPlayerHeightCheck->getGroundHeight() >= radius) {
                checkPos = targetPos;
                throwPos = checkPos;
            } else if (!checkEnableThrowStartSpace(&checkPos, &_240, &_24c, throwPos, range, 150.0f,
                                                   false, checkPos2)) {
                return;
            }
            sead::Vector3f crossDir;
            crossDir.setCross(_240, _24c);
            al::normalize(&crossDir);
            if (_268.y > 0.0f) {
                _240 = _24c;
                _24c = -crossDir;
                sead::Quatf quat = sead::Quatf::unit;
                al::makeQuatAxisRotation(&quat, _240, -al::getGravity(this), _24c, 1.0f);
                _240.rotate(quat);
                al::normalize(&_240);
                _24c.rotate(quat);
                al::normalize(&_24c);
            } else {
                _240 = -_24c;
                _24c = crossDir;
            }
            throwDist = 0.0f;
        } else {
            radius = range;
        }

        if (!checkEnableThrowStartSpace(&checkPos, &_240, &_24c, throwPos, radius, throwDist,
                                        isRollingThrow, checkPos2))
            return;

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, -_240, _24c);
        al::updatePoseQuat(this, quat);
        al::resetPosition(this, checkPos);
        rs::resetCollisionExpandCheck(mPlayerColliderHackCap);
    } else {
        sead::Vector3f crossDir;
        crossDir.setCross(_240, _24c);
        if (_268.x < 0.0f)
            crossDir = -crossDir;
        checkPos = al::getTrans(mPlayerActor) + _258 + _24c * mHackCapThrowParam->mHackThrowHeight->value;
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, -_240, _24c);
        al::updatePoseQuat(this, quat);
        al::resetPosition(this, checkPos);
        rs::resetCollisionExpandCheck(mPlayerColliderHackCap);
        sead::Vector3f throwDir = _240 + crossDir;
        al::normalize(&throwDir);
        targetPos = throwDir * 150.0f + checkPos;
        al::setTrans(this, targetPos);
    }

    if (al::isFirstStep(this)) {
        mHackCapAboveGroundChecker->update(-al::getGravity(this));
        mWaterSurfaceFinder->update(al::getTrans(this), -al::getGravity(this), 100.0f);
        _29c = mWaterSurfaceFinder->isFoundSurface();
    }
    updateWaterArea();

    f32 throwSpeed = getThrowSpeed();
    f32 throwRange = getThrowRange();
    s32 brakeTime = getThrowBrakeTime();
    f32 decel = throwSpeed / brakeTime;
    _280 = -decel;
    _27c = (s32)((throwRange - (throwSpeed * brakeTime + brakeTime * (brakeTime * (decel * -0.5f)))) /
                 throwSpeed);
    al::setVelocity(this, _240 * getThrowSpeed());
    _270 = getThrowRange();
    _274 = 0.0f;

    if (mIsSeparateFlying || (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0))
        al::startHitReaction(this, "おすそ分け投げ");
    if (_298)
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
    else
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");

    if (isAfterMovementTrigger) {
        mHackCapTrigger->set(HackCapTrigger::EAfterMovementTrigger::Val1);
        al::setNerve(this, &NrvHackCap.ThrowStay);
    } else if (_4d0) {
        alPadRumbleFunction::startPadRumbleWithVolume(this, "LandHipDrop_amfm", 0.7f, 0.7f, -1);
        _4d4 = _4e0 = al::getTrans(mPlayerActor);
        al::setNerve(this, &NrvHackCap.ThrowTornado);
    } else if (isRollingThrow) {
        alPadRumbleFunction::startPadRumbleWithVolume(this, "LandHipDrop_amfm", 0.7f, 0.7f, -1);
        al::setNerve(this, &NrvHackCap.ThrowRolling);
    } else if (!isNearZeroX) {
        if (_268.x > 0.0f)
            alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 0.0f, 1.0f, -1);
        else
            alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 1.0f, 0.0f, -1);
        alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveLandHipDrop_Delay40", 0.25f, 0.0f, -1);
        al::setVelocityZero(this);
        al::setNerve(this, &NrvHackCap.ThrowSpiral);
    } else {
        switch (_238) {
        case SwingHandType::Val2:
            alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 0.15f, 0.75f, -1);
            break;
        case SwingHandType::Val1:
            alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 0.75f, 0.15f, -1);
            break;
        default:
            alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 0.0f, 0.1f, -1);
            break;
        }
        al::setNerve(this, &NrvHackCap.Throw);
    }
}

void HackCap::exeThrowStay() {
    _510 = HackCapFunction::isKeepSeparateHold(mPlayerSeparateCapFlag, mCapJudgePreInputSeparateThrow,
                                               _510) &
           1;
    if (al::isFirstStep(this)) {
        al::startAction(this, _264 > 0.0f ? "StayL" : "StayR");
        _4d0 = false;
        if (mStateThrowStay->_70)
            al::setVelocityZero(this);
    }
    if (_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && !mInput->isNoInputSeparateCap()) {
        _299 = false;
        _1b0 = 0;
        _1ac = 0;
        _510 = true;
        rs::resetJudge(mCapJudgePreInputSeparateThrow);
    }
    updateWaterArea();
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        bool isHit = false;
        f32 height = mHackCapThrowParam->mHackThrowHeight->value;
        sead::Vector3f up = -al::getGravity(this);
        sead::Vector3f hitPos(0.0f, 0.0f, 0.0f);
        sead::Vector3f hitNormal(0.0f, 0.0f, 0.0f);
        if (!checkThrowMoveHit(&isHit, hitPos, hitNormal, (s32*)&_28c, this, up, _240, _24c,
                               height)) {
            f32 rate = al::calcRate01((f32)*(s32*)&_28c, 2.0f, 5.0f) * 0.5f;
            sead::Vector3f turned = up;
            al::turnVecToVecRate(&turned, _24c, turned, rate);
            al::normalize(&turned);
            sead::Vector3f along(0.0f, 0.0f, 0.0f);
            al::alongVectorNormalH(&along, _240, _24c, turned);
            al::normalize(&along);
            if (isHit) {
                bool isDirUp = al::isNearZeroOrGreater(_240.dot(up), 0.1f);
                f32 absTurnedHit = sead::Mathf::abs(turned.dot(hitNormal));
                f32 cosVal = sead::Mathf::cos(0.08715574f);
                f32 absTurnedUp = sead::Mathf::abs(turned.dot(up));
                if (isDirUp && absTurnedHit < cosVal &&
                    absTurnedUp < hitNormal.dot(up)) {
                    f32 ease = al::easeOut(al::calcRate01((f32)*(s32*)&_28c, 0.0f, 5.0f));
                    sead::Vector3f diff = al::getTrans(this) - hitPos;
                    f32 adjust = sead::Mathf::min(0.0f, height - diff.dot(hitNormal));
                    *al::getTransPtr(this) += hitNormal * (ease * adjust);
                }
            }
            _240 = along;
            _24c = turned;
        }
        sead::Vector3f normal(0.0f, 0.0f, 0.0f);
        bool isCollide = false;
        if (rs::isCollidedWallFace(mPlayerColliderHackCap)) {
            sead::Vector3f wallNormal = rs::getCollidedWallNormal(mPlayerColliderHackCap);
            if (wallNormal.dot(_24c) >= 0.5f &&
                wallNormal.dot(al::getGravity(this)) < -0.08715574f) {
                normal = wallNormal;
                isCollide = true;
            }
        }
        if (!isCollide && rs::isCollidedGround(mPlayerColliderHackCap)) {
            normal = rs::getCollidedGroundNormal(mPlayerColliderHackCap);
            isCollide = true;
        }
        sead::Vector3f frontDir = _240;
        if (isCollide && normal.dot(_24c) > 0.0f) {
            sead::Vector3f along(0.0f, 0.0f, 0.0f);
            al::alongVectorNormalH(&along, _240, _24c, normal);
            al::normalize(&along);
            _240 = along;
            _24c = normal;
            _28c = 0.0f;
            frontDir = along;
        }
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, -frontDir, _24c);
        al::slerpQuat(&quat, al::getQuat(this), quat, 1.0f);
        al::updatePoseQuat(this, quat);
    }
    sead::Vector3f targetPos;
    if (mIsSeparateFlying) {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        mCapTargetInfo1->makeLockOnMtx(&mtx);
        targetPos = mtx.getTranslation();
    } else {
        targetPos = al::getTrans(mPlayerActor);
    }
    mStateThrowStay->_71 = _29a;
    mStateThrowStay->_72 = _29b;
    mStateThrowStay->_74 = targetPos;
    mStateThrowStay->_a0 = mHackCapThrowParam->mHackThrowHeight->value;
    f32 speed;
    if (_29f) {
        speed = 60.0f;
    } else if (al::isNearZero(_268.x, 0.1f) && !al::isNearZero(_268.y, 0.1f)) {
        speed = mHackCapThrowParam->mRollSpeed->value;
    } else if (_29a) {
        speed = mHackCapThrowParam->mWaterMaxSpeed->value;
    } else if (_29e || _4d0) {
        speed = mHackCapThrowParam->mContinuousThrowSpeed->value;
    } else {
        speed = mHackCapThrowParam->mMaxVel->value;
    }
    mStateThrowStay->_a4 = speed;
    mStateThrowStay->_a8 =
        (_29a ? mHackCapThrowParam->mWaterMaxRetSpeed : mHackCapThrowParam->mMaxRetSpeed)
            ->value;
    mStateThrowStay->_b0 = _264 > 0.0f ? "StayL" : "StayR";
    bool isUpdateNerve = al::updateNerveState(this);
    updateLavaSurfaceMove();
    updateThrowJoint();
    if (!_5e8) {
        if (tryCollideReflectReaction())
            return;
        if (tryCollideWallReactionStay())
            return;
        if (tryAppendAttack()) {
            al::setNerve(this, &NrvHackCap.ThrowAppend);
            return;
        }
    }
    bool isHold;
    if (_299) {
        isHold = mInput->isHoldAction();
    } else if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
               (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        isHold = _510 != 0;
    } else {
        isHold = mInput->isHoldSpinCap();
    }
    bool isLight = false;
    if (!_299) {
        isLight = (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                  (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
    }
    al::holdSeWithParam(this, isLight ? "PgStayLightLv" : "PgStayLv", isHold ? 2.0f : 1.0f, "");
    if (!isLight && !_2a9[0] && isHold) {
        _2a9[0] = true;
        al::startHitReaction(this, "Stay状態開始");
    }
    if (mIsSeparateFlying) {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        mCapTargetInfo1->makeLockOnMtx(&mtx);
        sead::Vector3f targetPos2 = mtx.getTranslation();
        if ((targetPos2 - al::getTrans(this)).length() < 100.0f) {
            endHackThrowAndReturnHack();
            return;
        }
    }
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        if (isUpdateNerve || !isHold) {
            al::startHitReaction(this, isLight ? "おすそわけStay状態からのリターン"
                                               : "Stay状態からのリターン");
            al::setNerve(this, &NrvHackCap.Return);
        }
        return;
    }
    if (al::isLessEqualStep(this, _29f ? 3 : mHackCapThrowParam->mEndpointStopTime->value))
        return;
    if (!isHold) {
        al::startHitReaction(this, isLight ? "おすそわけStay状態からのリターン"
                                           : "Stay状態からのリターン");
        al::setNerve(this, &NrvHackCap.Return);
        return;
    }
    if (al::isLessEqualStep(this, !_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                                        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0
                                    ? 120
                                    : mHackCapThrowParam->mMaxEndpointStopTime->value))
        return;
    al::startHitReaction(this, isLight ? "おすそわけStay状態からのリターン"
                                       : "Stay状態からのリターン");
    al::setNerve(this, &NrvHackCap.Return);
}

bool HackCap::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(msg))
        return true;
    if (al::isSensorName(self, "Trample")) {
        if (rs::isMsgPlayerCapTrample(msg) || rs::isMsgPlayerCapHipDrop(msg)) {
            CapTargetInfo* targetInfo = mCapTargetInfo1;
            s32 lockOnCounter = _580;
            bool isHold;
            if (_299)
                isHold = mInput->isHoldAction();
            else if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0 ||
                     (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
                isHold = mInput->isHoldSpinCap();
            else
                isHold = _510;
            if (al::isNerve(this, &NrvHackCap.ThrowStay)) {
                if (!isHold)
                    return false;
            } else {
                bool isLockOn = al::isNerve(this, &NrvHackCap.LockOn);
                if (targetInfo == nullptr)
                    return false;
                if (!isLockOn)
                    return false;
                if (lockOnCounter < 10)
                    return false;
                if (!targetInfo->_74)
                    return false;
            }
            if (mPlayerWallActionHistory->mIsJumpStored)
                return false;
            sead::Vector3f vecBetween(0.0f, 0.0f, 0.0f);
            al::calcVecBetweenSensors(&vecBetween, self, other);
            sead::Vector3f vecH(0.0f, 0.0f, 0.0f);
            sead::Vector3f vecV(0.0f, 0.0f, 0.0f);
            al::separateVectorHV(&vecH, &vecV, al::getGravity(mPlayerActor), vecBetween);
            if (vecV.length() > 70.0f)
                return false;
            if (vecH.length() > (al::isNerve(this, &NrvHackCap.LockOn) ? 150.0f : 200.0f))
                return false;
            _29e = false;
            _274 = 0.0f;
            _264 = 0.0f;
            _268.set(0.0f, 0.0f);
            _270 = 0.0f;
            if (_298)
                al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
            else
                al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
            if (al::isNerve(this, &NrvHackCap.ThrowStay)) {
                if (mPlayerCapActionHistory->mIsCapJumpPossible &&
                    (_299 || (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0 ||
                     (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0 ||
                     !mStateThrowStay->isHomingPlayerJump())) {
                    rs::requestHitReactionToAttacker(msg, other, al::getSensorPos(other));
                    al::setNerve(this, &NrvHackCap.Trample);
                    return true;
                }
                return false;
            }
            rs::requestHitReactionToAttacker(msg, other, al::getSensorPos(other));
            al::setNerve(this, &NrvHackCap.TrampleLockOn);
            return true;
        }
        return false;
    }
    if (al::isSensorName(self, "Catch")) {
        if (rs::isMsgPlayerCapCatch(msg)) {
            bool isHold;
            if (_299)
                isHold = mInput->isHoldAction();
            else if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0 ||
                     (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
                isHold = mInput->isHoldSpinCap();
            else
                isHold = _510;
            if (!al::isSensorPlayer(other))
                return false;
            if (al::getSensorHost(other) != mPlayerActor)
                return false;
            const PlayerSeparateCapFlag* separateCapFlag = mPlayerSeparateCapFlag;
            s32 throwStep = _294;
            bool isFlying = _299;
            if (al::isNerve(this, &NrvHackCap.Hack))
                return false;
            if (al::isNerve(this, &NrvHackCap.Throw) && al::isLessStep(this, 20))
                return false;
            if (al::isNerve(this, &NrvHackCap.ThrowRolling) && al::isLessStep(this, 20))
                return false;
            if (al::isNerve(this, &NrvHackCap.ThrowTornado) && al::isLessStep(this, 20))
                return false;
            if (al::isNerve(this, &NrvHackCap.Rethrow) && al::isLessStep(this, 20))
                return false;
            if (al::isNerve(this, &NrvHackCap.ThrowSpiral))
                return false;
            if (al::isNerve(this, &NrvHackCap.ThrowStay) && isHold) {
                if (isFlying || (separateCapFlag->_flagsWord & 0xff0000) != 0 ||
                    (separateCapFlag->_flagsWord & 0xff) == 0) {
                    if (throwStep > 0)
                        return false;
                } else {
                    bool isHack = rs::isPlayerHack(this);
                    if (throwStep > 0)
                        return false;
                    if (isHack)
                        return false;
                }
            }
            if (isThrow() || al::isNerve(this, &NrvHackCap.Return) ||
                al::isNerve(this, &NrvHackCap.Rebound)) {
                _2a0[2] = true;
                _2a0[3] = true;
                al::offCollide(this);
                al::invalidateShadow(this);
                al::invalidateHitSensors(this);
                mCapTargetInfo1 = nullptr;
                _1d0 = nullptr;
                _2a8 = false;
                al::setVelocityZero(this);
                return true;
            }
            return false;
        }
        if (rs::isMsgPlayerCapTouchJump(msg)) {
            if (mPlayerWallActionHistory->mIsJumpStored)
                return false;
            if (_294 > 0)
                return false;
            if (!mPlayerCapActionHistory->mIsCapJumpPossible)
                return false;
            if (!al::isNerve(this, &NrvHackCap.ThrowStay))
                return false;
            if (!isEnableCapTouchJumpInput())
                return false;
            if (_530 && _534 != 0) {
                bool isSeparate = !_299 &&
                                  (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                                  (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
                if (!isSeparate && rs::isPlayerCollidedGround(this))
                    return false;
            }
            if (mStateThrowStay->isEnableTouchJumpTransWarp()) {
                al::resetPosition(this, al::getTrans(mPlayerActor));
                rs::resetCollisionExpandCheck(mPlayerColliderHackCap);
            }
            if (mStateThrowStay->isEnableKeepStayTouchJump())
                al::setNerve(this, &NrvHackCap.Trample);
            else
                al::setNerve(this, &NrvHackCap.TrampleReturn);
            return true;
        }
    }
    if (al::isSensorName(self, "Push")) {
        if (_299)
            return false;
        if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0)
            return false;
        if ((mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
            return false;
        if (!al::isNerve(this, &NrvHackCap.ThrowStay))
            return false;
        if (_294 > 0)
            return false;
        return mCapPushReceiver->receivePushMsgCap(msg, other, self, 20.0f);
    }
    if (rs::isMsgCapRethrow(msg)) {
        if (_5e8)
            return false;
        if (!isThrow() && !al::isNerve(this, &NrvHackCap.SpinAttack)) {
            if (al::isNerve(this, &NrvHackCap.Catch)) {
                if (!_2a5[2])
                    return false;
            } else if (!al::isNerve(this, &NrvHackCap.Return) ||
                       !rs::isCapRethrowReturnOnly(msg)) {
                return false;
            }
        }
        if (!isThrow() && !al::isNerve(this, &NrvHackCap.Return)) {
            sead::Vector3f frontDir(0.0f, 0.0f, 0.0f);
            al::calcFrontDir(&frontDir, this);
            sead::Vector3f upDir(0.0f, 0.0f, 0.0f);
            al::calcUpDir(&upDir, this);
            al::showModelIfHide(this);
            al::startAction(mThrowingHatEyes, "Appear");
            al::setScaleAll(this, 1.0f);
            _2a0[0] = true;
            _2a0[1] = true;
            _2a0[2] = false;
            _2a0[3] = false;
            _2a8 = false;
            _299 = false;
            _2d8 = mInput->_50;
            _1ac = 0;
            _1b0 = 0;
            _1c4 = 0;
            _1b8_current = 0;
            al::copyPose(this, mPlayerActor);
            _240.set(0.0f, 0.0f, 0.0f);
            al::setVelocityZero(this);
            HackCapFunction::resetPositionAndCollision(this, mPlayerColliderHackCap);
            al::setNerve(this, &NrvHackCap.SpinAttack);
            startThrow(false, frontDir, upDir, 0.0f, sead::Vector2f::zero, sead::Vector2f::zero,
                       sead::Vector3f::zero, false, sead::Vector3f::zero, SwingHandType::Val0, true,
                       0.0f, 1);
            _290 = 0;
            al::validateHitSensors(this);
            al::invalidateHitSensor(this, "SpiralTail1");
            al::invalidateHitSensor(this, "SpiralTail2");
            al::invalidateHitSensor(this, "SpiralTail3");
            al::invalidateHitSensor(this, "SpiralTail4");
            al::invalidateHitSensor(this, "SpiralTail5");
            al::validateShadow(this);
            al::onCollide(this);
            const char* actionNameL = _29e ? "FlyingWaitCapCatchL" : "FlyingWaitL";
            const char* actionNameR = _29e ? "FlyingWaitCapCatchR" : "FlyingWaitR";
            al::startAction(this, _264 > 0.0f ? actionNameL : actionNameR);
        }
        rs::tryGetCapRethrowPos(al::getTransPtr(this), msg);
        sead::Vector3f rethrowFront(0.0f, 0.0f, 0.0f);
        if (rs::tryGetCapRethrowFront(&rethrowFront, msg)) {
            if (al::isNearZero(_240, 0.001f)) {
                al::calcFrontDir(&_240, this);
                al::calcUpDir(&_24c, this);
            }
            sead::Vector3f rethrowUp(0.0f, 0.0f, 0.0f);
            if (rs::tryGetCapRethrowUp(&rethrowUp, msg)) {
                _240 = rethrowFront;
                _24c = rethrowUp;
            } else if (!al::isNearDirection(_240, rethrowFront)) {
                if (al::isReverseDirection(_240, rethrowFront)) {
                    _240 = rethrowFront;
                } else {
                    sead::Quatf quat = sead::Quatf::unit;
                    al::makeQuatRotationRate(&quat, _240, rethrowFront, 1.0f);
                    al::rotateVectorQuat(&_240, quat);
                    al::normalize(&_240);
                    al::rotateVectorQuat(&_24c, quat);
                    al::normalize(&_24c);
                }
            }
            sead::Quatf quat = sead::Quatf::unit;
            al::makeQuatFrontUp(&quat, -_240, _24c);
            al::updatePoseQuat(this, quat);
        }
        _29e = false;
        _274 = 0.0f;
        _264 = 0.0f;
        _268.set(0.0f, 0.0f);
        _270 = 0.0f;
        if (_298)
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        else
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        HackCapFunction::resetPositionAndCollision(this, mPlayerColliderHackCap);
        updateWaterArea();
        bool isReturnOnly = rs::isCapRethrowReturnOnly(msg);
        f32 throwRange = getThrowRange();
        f32 range = isReturnOnly ? throwRange : throwRange * 2.5f;
        f32 throwSpeed = getThrowSpeed();
        s32 brakeTime = getThrowBrakeTime();
        f32 decel = throwSpeed / brakeTime;
        _280 = -decel;
        _27c = (s32)((range - (throwSpeed * brakeTime + brakeTime * (brakeTime * (decel * -0.5f)))) /
                     throwSpeed);
        al::setVelocity(this, _240 * getThrowSpeed());
        al::showModelIfHide(this);
        mEquipmentHat->kill();
        mLockOnCapEyes->kill();
        al::onCollide(this);
        _299 = true;
        al::setNerve(this, &NrvHackCap.Rethrow);
        return true;
    }
    if (!_5e8) {
        if (isThrow() || al::isNerve(this, &NrvHackCap.Return) ||
            al::isNerve(this, &NrvHackCap.Rebound) || al::isNerve(this, &NrvHackCap.Trample) ||
            al::isNerve(this, &NrvHackCap.TrampleReturn)) {
            if (mCapExternalVelocity->receiveMsgCap(msg, other, self)) {
                if (al::isNerve(this, &NrvHackCap.Blow))
                    return true;
                _4ec = al::isNerve(this, &NrvHackCap.Throw) ||
                       al::isNerve(this, &NrvHackCap.Rethrow) ||
                       al::isNerve(this, &NrvHackCap.ThrowBrake) ||
                       al::isNerve(this, &NrvHackCap.ThrowTornado) ||
                       al::isNerve(this, &NrvHackCap.ThrowRolling) ||
                       al::isNerve(this, &NrvHackCap.ThrowRollingBrake) ||
                       al::isNerve(this, &NrvHackCap.ThrowAppend);
                if (al::isNerve(this, &NrvHackCap.ThrowSpiral)) {
                    sead::Vector3f* velocity = al::getVelocityPtr(this);
                    f32 length = velocity->length();
                    if (length > 0.0f) {
                        f32 scale = 30.0f / length;
                        velocity->x *= scale;
                        velocity->y *= scale;
                        velocity->z *= scale;
                    }
                }
                al::setNerve(this, &NrvHackCap.Blow);
                return true;
            }
        }
    }
    if (rs::isMsgJangoRemoveCap(msg)) {
        if (isThrow() || al::isNerve(this, &NrvHackCap.Return)) {
            if (mIsSeparateFlying) {
                al::setNerve(this, &NrvHackCap.LockOn);
                rs::sendMsgTransferHack(mPlayerBodySensor, const_cast<al::HitSensor*>(_1d0));
                mIsSeparateFlying = false;
                mPlayerBodySensor = nullptr;
            }
            hide(true);
            al::tryKillEmitterAndParticleAll(this);
            _2a0[2] = true;
            _2a0[3] = true;
            static_cast<al::SimpleLayoutAppearWaitEnd*>(_5f0)->kill();
            return true;
        }
    }
    return false;
}

void HackCap::exeThrow() {
    _278++;
    _510 = HackCapFunction::isKeepSeparateHold(mPlayerSeparateCapFlag, mCapJudgePreInputSeparateThrow,
                                               _510) &
           1;
    if (al::isFirstStep(this)) {
        const char* actionNameL = _29e ? "FlyingWaitCapCatchL" : "FlyingWaitL";
        const char* actionNameR = _29e ? "FlyingWaitCapCatchR" : "FlyingWaitR";
        const char* action = _264 > 0.0f ? actionNameL : actionNameR;
        al::tryStartActionIfNotPlaying(this, action);
        if (al::isNerve(this, &NrvHackCap.Throw)) {
            al::startHitReaction(this, "帽子投げ");
            switch (_23c) {
            case 0:
                al::startHitReaction(this, "帽子投げボイス[3段目]");
                break;
            case 1:
                al::startHitReaction(this, "帽子投げボイス[1段目]");
                break;
            case 2:
                al::startHitReaction(this, "帽子投げボイス[2段目]");
                break;
            }
        }
    }
    updateThrowJoint();
    if (tryCollideReflectReaction())
        return;
    if (tryCollideWallReaction())
        return;
    calcThrowMove(_240, _24c, &_28c, mPlayerColliderHackCap, true,
                  mHackCapThrowParam->mHackThrowHeight->value);
    changeThrowParamInWater(al::getNerveStep(this), false);
    calcThrowRollingDirection(_240, _24c, mInput, mPlayerSeparateCapFlag, _299);
    addCurveOffset();
    al::scaleVelocityExceptDirection(this, _240, 0.94f);
    if (!al::isLessStep(this, _27c))
        al::setNerve(this, &NrvHackCap.ThrowBrake);
}

void HackCap::exeThrowBrake() {
    _510 = HackCapFunction::isKeepSeparateHold(mPlayerSeparateCapFlag, mCapJudgePreInputSeparateThrow,
                                               _510) &
           1;
    if (al::isFirstStep(this)) {
        const char* actionNameL = _29e ? "FlyingWaitCapCatchL" : "FlyingWaitL";
        const char* actionNameR = _29e ? "FlyingWaitCapCatchR" : "FlyingWaitR";
        const char* action = _264 > 0.0f ? actionNameL : actionNameR;
        al::tryStartActionIfNotPlaying(this, action);
    }
    updateThrowJoint();
    if (tryCollideReflectReaction())
        return;
    if (tryCollideWallReaction())
        return;
    calcThrowMove(_240, _24c, &_28c, mPlayerColliderHackCap, true,
                  mHackCapThrowParam->mHackThrowHeight->value);
    if (changeThrowParamInWater(al::getNerveStep(this), true)) {
        al::setNerve(this, &NrvHackCap.Rethrow);
        return;
    }
    if (!calcThrowRollingDirection(_240, _24c, mInput, mPlayerSeparateCapFlag, _299)) {
        al::addVelocity(this, _240 * _280);
        al::scaleVelocityExceptDirection(this, _240, 0.94f);
    }
    if (tryAppendAttack()) {
        al::setNerve(this, &NrvHackCap.ThrowAppend);
        return;
    }
    addCurveOffset();
    const sead::Vector3f& velocity = al::getVelocity(this);
    bool isStopping = al::isNearZero(velocity, 0.001f) || (_240.dot(velocity) < 0.0f);
    s32 step;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0 ||
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0) {
        if (isStopping) {
            mHackCapTrigger->set(HackCapTrigger::EAfterMovementTrigger::Val2);
            al::setNerve(this, &NrvHackCap.ThrowStay);
        }
        return;
    }
    if (_29f) {
        step = 2;
    } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        step = _268.y > 0.0f ? mHackCapThrowParam->mRollBrakeTimeTop->value
                             : mHackCapThrowParam->mRollBrakeTimeBottom->value;
    } else {
        step = _29a ? mHackCapThrowParam->mWaterBreakTime->value
                    : mHackCapThrowParam->mBreakTime->value;
    }
    bool isOverStep = al::isGreaterEqualStep(this, step);
    if (isStopping || isOverStep) {
        mHackCapTrigger->set(HackCapTrigger::EAfterMovementTrigger::Val2);
        al::setNerve(this, &NrvHackCap.ThrowStay);
    }
}

void HackCap::updateWaterArea() {
    bool isInWater = mPlayerAreaChecker->isInWater(al::getTrans(this));
    sead::Vector3f up = -al::getGravity(this);
    mWaterSurfaceFinder->update(al::getTrans(this), up, 100.0f);
    bool isFoundSurface = mWaterSurfaceFinder->isFoundSurface();
    f32 distance = mWaterSurfaceFinder->getDistance();
    _29a = isInWater && (!isFoundSurface || distance >= 30.0f);
    al::updateMaterialCodeWater(this, _29a);
    bool isWet = _29a ? false : mSelfPlayerWetControl->isWet();
    al::updateMaterialCodeWet(this, isWet);
    if (_29a)
        mSelfPlayerWetControl->recordInWater();
    else if (isFoundSurface)
        mSelfPlayerWetControl->recordWaterSurface();
    if (_29d || _29a)
        goto cleanup;
    if (!isThrow() && !al::isNerve(this, &NrvHackCap.Return) &&
        !al::isNerve(this, &NrvHackCap.Rebound))
        goto cleanup;
    {
        bool isInWaterIce = false;
        if (isFoundSurface) {
            sead::Vector3f checkPos =
                mWaterSurfaceFinder->getSurfacePosition() + al::getGravity(this) * 5.0f;
            isInWaterIce = mPlayerAreaChecker->isInWaterIce(checkPos);
        }
        bool isRolling = al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f);
        sead::Vector3f frontDir(0.0f, 0.0f, 0.0f);
        sead::Vector3f upDir = -al::getGravity(this);
        al::verticalizeVec(&frontDir, upDir, al::getVelocity(this));
        bool isFrontDirValid = al::tryNormalizeOrZero(&frontDir);
        if (!isFrontDirValid) {
            al::calcFrontDir(&frontDir, this);
            al::verticalizeVec(&frontDir, upDir, frontDir);
            if (!al::tryNormalizeOrZero(&frontDir))
                goto cleanup;
            isFrontDirValid = false;
        }
        sead::Vector3f effectPos(0.0f, 0.0f, 0.0f);
        if (isFoundSurface) {
            if (mHackCapAboveGroundChecker->_8) {
                if (mWaterSurfaceFinder->getDistance() < 50.0f) {
                    if (mHackCapAboveGroundChecker->_24 > -mWaterSurfaceFinder->getDistance()) {
                        effectPos = mWaterSurfaceFinder->getSurfacePosition();
                    } else {
                        goto cleanup;
                    }
                } else {
                    goto cleanup;
                }
            } else {
                if (mWaterSurfaceFinder->getDistance() < 50.0f) {
                    effectPos = mWaterSurfaceFinder->getSurfacePosition();
                } else {
                    goto cleanup;
                }
            }
        } else {
            const al::ArrowHitInfo* hitInfo = nullptr;
            sead::Vector3f rayDir = al::getGravity(this) * 100.0f;
            if (!alCollisionUtil::getFirstPolyOnArrow(this, &hitInfo, al::getTrans(this), rayDir,
                                                      nullptr, nullptr))
                goto cleanup;
            if (!al::isEqualString(al::getMaterialCodeName(hitInfo->operator*()), "Puddle"))
                goto cleanup;
            sead::Vector3f hitNormal = alCollisionUtil::getCollisionHitNormal(hitInfo->operator*());
            sead::Vector3f hitPos = alCollisionUtil::getCollisionHitPos(hitInfo->operator*());
            effectPos = hitPos + hitNormal * 0.0f;
        }
        al::makeMtxUpFrontPos(&_4a0, upDir, frontDir, effectPos);
        if (isFrontDirValid) {
            if (_30d) {
                goto checkStayEffect;
            }
            if (_30e) {
                al::tryDeleteEffect(this, _3a8.cstr());
                _3a8.copy(sead::SafeStringBase<char>::cEmptyString);
                al::tryStopSe(this, "StayWaterSurface", -1, nullptr);
                _30e = false;
            }
            _30d = true;
            al::isNearZero(_268.x, 0.001f);
            if (isInWaterIce) {
                if (isRolling)
                    al::startHitReaction(this, "氷水の水面移動(転がり)");
                else
                    al::startHitReaction(this, "氷水の水面移動");
            } else {
                if (isRolling)
                    al::startHitReaction(this, "水面移動(転がり)");
                else
                    al::startHitReaction(this, "水面移動");
            }
            if (isRolling)
                _310.copy("FlyRollingWaterSurface");
            else if (_268.x != 0.0f || !al::isNearZero(_268.x, 0.001f))
                _310.copy("FlySpiralWaterSurface");
            else
                _310.copy("FlyWaterSurface");
            al::emitEffect(this, _310.cstr(), nullptr);
            goto updateFlags;
        }
    checkStayEffect:
        if (_30e) {
            goto updateFlags;
        }
        if (_30d) {
            al::tryDeleteEffect(this, _310.cstr());
            _310.copy(sead::SafeStringBase<char>::cEmptyString);
            al::tryStopSe(this, "FlyWaterSurface", -1, nullptr);
            al::tryStopSe(this, "FlyWaterSurfaceIce", -1, nullptr);
            _30d = false;
        }
        _30e = true;
        if (isInWaterIce) {
            if (isRolling)
                al::startHitReaction(this, "氷水の水面待機(転がり)");
            else
                al::startHitReaction(this, "氷水の水面待機");
        } else {
            if (isRolling)
                al::startHitReaction(this, "水面待機(転がり)");
            else
                al::startHitReaction(this, "水面待機");
        }
        if (_264 > 0.0f)
            _3a8.format("%s%s", "R", "StayWaterSurface");
        else
            _3a8.format("%s%s", "L", "StayWaterSurface");
        al::emitEffect(this, _3a8.cstr(), nullptr);
    updateFlags:
        goto updateWaterFlags;
    }
cleanup:
    if (_30d) {
        al::tryDeleteEffect(this, _310.cstr());
        _310.copy(sead::SafeStringBase<char>::cEmptyString);
        al::tryStopSe(this, "FlyWaterSurface", -1, nullptr);
        al::tryStopSe(this, "FlyWaterSurfaceIce", -1, nullptr);
        _30d = false;
    }
    if (_30e) {
        al::tryDeleteEffect(this, _3a8.cstr());
        _3a8.copy(sead::SafeStringBase<char>::cEmptyString);
        al::tryStopSe(this, "StayWaterSurface", -1, nullptr);
        _30e = false;
    }
updateWaterFlags:
    {
        u16 flags = _29c;
        if ((flags & 0xff) != 0 && (mWaterSurfaceFinder->isFoundSurface() & 0xff) == 0)
            _29c = 0;
        u8 waterFlag = (flags >> 8) & 0xff;
        u8 inWater = isInWater ? 1 : 0;
        if (waterFlag != inWater) {
            _29c = (u16)((mWaterSurfaceFinder->isFoundSurface() & 0xff) | (inWater << 8));
            _29d = inWater;
            sead::Matrix34f mtx;
            f32 height = 0.0f;
            bool isCollided = rs::isCollidedGround(mPlayerCollision);
            bool isWaterIn = false;
            if (isInWater) {
                isWaterIn = rs::tryCalcEffectMtxWaterIn(&mtx, &height, this, mWaterSurfaceFinder,
                                                        isCollided, false);
                if (isWaterIn)
                    al::startHitReaction(this, "帽子入水");
            } else {
                isWaterIn = rs::tryCalcEffectMtxWaterOut(&mtx, &height, this, mWaterSurfaceFinder,
                                                         isCollided, false);
                if (isWaterIn)
                    al::startHitReaction(this, "帽子出水");
            }
        }
    }
}

void HackCap::exeThrowSpiral() {
    _278++;
    if (al::isFirstStep(this)) {
        _300 = _240;
        al::tryStartActionIfNotPlaying(this, "FlyingWaitSpiral");
        al::setVelocityZero(this);
    }

    _2f4 = al::getTrans(mPlayerActor) + _24c * mHackCapThrowParam->mHackThrowHeight->value;

    updateThrowJoint();
    updateWaterArea();
    if (tryCollideReflectReaction()) {
        return;
    }
    if (tryCollideWallReactionSpiral()) {
        return;
    }

    sead::Vector3f dir = _24c;
    calcThrowMove(_240, _24c, &_28c, mPlayerColliderHackCap, true,
                  mHackCapThrowParam->mHackThrowHeight->value);
    al::alongVectorNormalH(&_300, _300, dir, _24c);

    s32 step = al::getNerveStep(this);
    s32 startIdx = step - 9 < 0 ? 0 : step - 9;
    s32 stepPlus1 = step + 1;
    s32 endIdx = step + 1 > 10 ? 11 : step;

    if (stepPlus1 == 10) {
        al::validateHitSensor(this, "SpiralTail1");
        al::validateHitSensor(this, "SpiralTail2");
        al::validateHitSensor(this, "SpiralTail3");
        al::validateHitSensor(this, "SpiralTail4");
        al::validateHitSensor(this, "SpiralTail5");
        const sead::Vector3f& pos = al::getTrans(this);
        mSpiralTailPositions[0] = pos;
        mSpiralTailPositions[1] = pos;
        mSpiralTailPositions[2] = pos;
        mSpiralTailPositions[3] = pos;
        mSpiralTailPositions[4] = pos;
    } else if (step > 9) {
        sead::Vector3f playerPos = al::getTrans(mPlayerActor);
        {
            sead::Vector3f normDir(0.0f, 0.0f, 0.0f);
            mSpiralTailPositions[4] = mSpiralTailPositions[3];
            sead::Vector3f diff = playerPos - mSpiralTailPositions[3];
            al::tryNormalizeOrZero(&normDir, diff);
            mSpiralTailPositions[4] += normDir * 40.0f;
        }
        {
            sead::Vector3f normDir(0.0f, 0.0f, 0.0f);
            mSpiralTailPositions[3] = mSpiralTailPositions[2];
            sead::Vector3f diff = playerPos - mSpiralTailPositions[2];
            al::tryNormalizeOrZero(&normDir, diff);
            mSpiralTailPositions[3] += normDir * 40.0f;
        }
        {
            sead::Vector3f normDir(0.0f, 0.0f, 0.0f);
            mSpiralTailPositions[2] = mSpiralTailPositions[1];
            sead::Vector3f diff = playerPos - mSpiralTailPositions[1];
            al::tryNormalizeOrZero(&normDir, diff);
            mSpiralTailPositions[2] += normDir * 40.0f;
        }
        {
            sead::Vector3f normDir(0.0f, 0.0f, 0.0f);
            mSpiralTailPositions[1] = mSpiralTailPositions[0];
            sead::Vector3f diff = playerPos - mSpiralTailPositions[0];
            al::tryNormalizeOrZero(&normDir, diff);
            mSpiralTailPositions[1] += normDir * 40.0f;
        }
        mSpiralTailPositions[0] = al::getTrans(this);
    }

    f32 angle1 = (f32)endIdx * 3.0f + (f32)startIdx * 8.0f + 330.0f;
    f32 radius1 = sead::Mathf::min((f32)stepPlus1 * 60.0f + 175.0f, 750.0f);
    sead::Vector3f target1 = _300;
    al::rotateVectorDegree(&target1, target1, _268.x * _24c, angle1);
    target1 = target1 * radius1 + _2f4;

    sead::Vector3f target2(0.0f, 0.0f, 0.0f);
    if (step >= 9) {
        f32 angle2 = (f32)startIdx * 19.0f;
        f32 radius2 = sead::Mathf::min((f32)startIdx * 6.0f + 50.0f, 450.0f);
        target2 = _300;
        al::rotateVectorDegree(&target2, target2, _268.x * _24c, angle2);
        target2 = target2 * radius2 + _2f4;
    }

    f32 t = sead::Mathf::clamp((f32)startIdx * 0.125f, 0.0f, 1.0f);
    sead::Vector3f lerped(0.0f, 0.0f, 0.0f);
    al::lerpVec(&lerped, target1, target2, al::easeOut(t));
    sead::Vector3f vel = lerped - al::getTrans(this);
    al::limitLength(&vel, vel, 200.0f);
    al::setVelocity(this, vel);

    sead::Vector3f vert(0.0f, 0.0f, 0.0f);
    al::verticalizeVec(&vert, _24c, vel);
    if (al::tryNormalizeOrZero(&vert)) {
        _240 = vert;
    }

    if (stepPlus1 >= 105) {
        if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && _510) {
            al::setNerve(this, &NrvHackCap.ThrowStay);
        } else {
            al::setNerve(this, &NrvHackCap.Return);
        }
    }
}

void HackCap::calcThrowMove(sead::Vector3f& dir, sead::Vector3f& pos, f32* param,
                            PlayerColliderHackCap* collider, bool flag, f32 height) {
    if (flag) {
        bool isHit = false;
        sead::Vector3f up = -al::getGravity(this);
        sead::Vector3f hitPos(0.0f, 0.0f, 0.0f);
        sead::Vector3f hitNormal(0.0f, 0.0f, 0.0f);
        if (!checkThrowMoveHit(&isHit, hitPos, hitNormal, (s32*)param, this, up, dir, pos,
                               height)) {
            f32 rate = al::calcRate01((f32)*(s32*)param, 2.0f, 5.0f) * 0.5f;
            sead::Vector3f turned = up;
            if (pos.dot(up) < 0.0f) {
                turned.x = -turned.x;
                turned.y = -turned.y;
                turned.z = -turned.z;
            }
            al::turnVecToVecRate(&turned, pos, turned, rate);
            al::normalize(&turned);
            sead::Vector3f along(0.0f, 0.0f, 0.0f);
            al::alongVectorNormalH(&along, dir, pos, turned);
            al::normalize(&along);
            if (isHit) {
                bool isDirUp = al::isNearZeroOrGreater(dir.dot(up));
                f32 absTurnedHit = sead::Mathf::abs(turned.dot(hitNormal));
                f32 cosVal = sead::Mathf::cos(0.08715574f);
                f32 absTurnedUp = sead::Mathf::abs(turned.dot(up));
                if (isDirUp && absTurnedHit < cosVal &&
                    absTurnedUp < hitNormal.dot(up)) {
                    f32 ease = al::easeOut(al::calcRate01((f32)*(s32*)param, 0.0f, 5.0f));
                    sead::Vector3f diff = al::getTrans(this) - hitPos;
                    f32 adjust = sead::Mathf::min(0.0f, height - diff.dot(hitNormal));
                    *al::getTransPtr(this) += hitNormal * (ease * adjust);
                }
            }
            sead::Vector3f parallel(0.0f, 0.0f, 0.0f);
            sead::Vector3f vertical(0.0f, 0.0f, 0.0f);
            al::separateVelocityParallelVertical(&parallel, &vertical, this, dir);
            sead::Vector3f vel = turned * vertical.length() + along * parallel.length();
            al::setVelocity(this, vel);
            __builtin_memcpy(&dir, &along, sizeof(sead::Vector3f));
            __builtin_memcpy(&pos, &turned, sizeof(sead::Vector3f));
        }
    }

    sead::Vector3f collPos(0.0f, 0.0f, 0.0f);
    sead::Vector3f normal(0.0f, 0.0f, 0.0f);
    bool isCollided = false;
    if (rs::isCollidedWallFace(collider)) {
        __builtin_memcpy(&normal, &rs::getCollidedWallNormal(collider), sizeof(sead::Vector3f));
        __builtin_memcpy(&collPos, &rs::getCollidedWallPos(collider), sizeof(sead::Vector3f));
        if (pos.dot(normal) >= 0.5f && normal.dot(al::getGravity(this)) < -0.08715574f) {
            isCollided = true;
        }
    }
    if (!isCollided && rs::isCollidedGround(collider)) {
        __builtin_memcpy(&normal, &rs::getCollidedGroundNormal(collider), sizeof(sead::Vector3f));
        __builtin_memcpy(&collPos, &rs::getCollidedGroundPos(collider), sizeof(sead::Vector3f));
        isCollided = true;
    }
    if (isCollided && normal.dot(pos) > 0.0f) {
        sead::Vector3f along(0.0f, 0.0f, 0.0f);
        al::alongVectorNormalH(&along, dir, pos, normal);
        al::normalize(&along);
        sead::Vector3f parallel(0.0f, 0.0f, 0.0f);
        sead::Vector3f vertical(0.0f, 0.0f, 0.0f);
        al::separateVelocityParallelVertical(&parallel, &vertical, this, dir);
        sead::Vector3f vel = normal * vertical.length() + along * parallel.length();
        al::setVelocity(this, vel);
        if (al::calcAngleDegree(dir, along) >= 0.1f) {
            al::startHitReactionHitEffect(this, "コリジョン衝突方向転換", collPos);
            sead::Vector3f cross;
            cross.setCross(dir, along);
            f32 angle = sead::Mathf::atan2(cross.length(), dir.dot(along));
            al::startSeWithParam(this, "HitCol", angle, "");
        }
        __builtin_memcpy(&dir, &along, sizeof(sead::Vector3f));
        __builtin_memcpy(&pos, &normal, sizeof(sead::Vector3f));
        *param = 0.0f;
    }

    sead::Quatf quat = sead::Quatf::unit;
    sead::Vector3f front = -dir;
    al::makeQuatFrontUp(&quat, front, pos);
    al::slerpQuat(&quat, al::getQuat(this), quat, 1.0f);
    al::updatePoseQuat(this, quat);
}

bool HackCap::checkEnableThrowStartSpace(sead::Vector3f* outPos, sead::Vector3f* outDir,
                                         sead::Vector3f* outPos2, const sead::Vector3f& throwPos,
                                         f32 range, f32 throwDist, bool isRolling,
                                         const sead::Vector3f& checkPos2) {
    sead::Vector3f hitPos(0.0f, 0.0f, 0.0f);
    sead::Vector3f hitNormal(0.0f, 0.0f, 0.0f);
    al::HitSensor* hitSensor = nullptr;
    sead::Vector3f targetPos = throwPos;
    sead::Vector3f from = throwPos - _240 * 5.0f;
    sead::Vector3f dir = _240 * range;
    if (!alCollisionUtil::getHitPosAndNormalAndSensorOnArrow(this, &hitPos, &hitNormal, &hitSensor,
                                                             from, dir, nullptr, nullptr))
        return true;
    f32 radius = mPlayerColliderHackCap->getColliderRadius();
    if (al::isWallPolygon(hitNormal, al::getGravity(this)))
        goto fail;
    if (isRolling) {
        sead::Vector3f hitPos2(0.0f, 0.0f, 0.0f);
        sead::Vector3f hitNormal2(0.0f, 0.0f, 0.0f);
        al::HitSensor* hitSensor2 = nullptr;
        sead::Vector3f from2 = hitPos + _240 * 5.0f;
        sead::Vector3f dir2 = _240 * radius * -2.0f;
        if (alCollisionUtil::getHitPosAndNormalAndSensorOnArrow(this, &hitPos2, &hitNormal2,
                                                                 &hitSensor2, from2, dir2, nullptr,
                                                                 nullptr))
            goto fail;
        *outPos = hitPos - _240 * radius;
        return true;
    }
    {
        f32 throwHeight = mHackCapThrowParam->mHackThrowHeight->value;
        sead::Vector3f cross;
        cross.setCross(_240, _24c);
        al::normalize(&cross);
        sead::Vector3f vert1(0.0f, 0.0f, 0.0f);
        al::verticalizeVec(&vert1, cross, hitNormal);
        if (!al::tryNormalizeOrZero(&vert1))
            goto fail;
        sead::Vector3f vert2(0.0f, 0.0f, 0.0f);
        al::verticalizeVec(&vert2, vert1, _240);
        if (!al::tryNormalizeOrZero(&vert2))
            goto fail;
        f32 dist = sead::Mathf::max(0.0f, range - (hitPos - throwPos).dot(vert2));
        sead::Vector3f throwDir = hitPos + vert2 * dist + vert1 * throwHeight - throwPos;
        if (!al::tryNormalizeOrZero(&throwDir))
            goto fail;
        sead::Vector3f hitPos2(0.0f, 0.0f, 0.0f);
        sead::Vector3f hitNormal2(0.0f, 0.0f, 0.0f);
        al::HitSensor* hitSensor2 = nullptr;
        sead::Vector3f from2 = throwPos - throwDir * 5.0f;
        sead::Vector3f dir2 = throwDir * range;
        if (!alCollisionUtil::getHitPosAndNormalAndSensorOnArrow(this, &hitPos2, &hitNormal2,
                                                                  &hitSensor2, from2, dir2, nullptr,
                                                                  nullptr)) {
            sead::Quatf quat = sead::Quatf::unit;
            al::makeQuatAxisRotation(&quat, _240, throwDir, cross, 1.0f);
            outDir->setRotated(quat, _240);
            al::normalize(outDir);
            outPos2->setRotated(quat, _24c);
            al::normalize(outPos2);
            *outPos = throwPos + *outDir * throwDist;
            return true;
        }
        goto fail;
    }
fail:
    if (_299)
        goto collideFail;
    if (mPlayerSeparateCapFlag->_flagsWord & 0xff0000)
        goto collideFail;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        goto collideFail;
    if (mIsSeparateFlying) {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        mCapTargetInfo1->makeLockOnMtx(&mtx);
        if (mCapTargetInfo1->mIsEscapeLocalOffset)
            al::calcTransLocalOffsetByMtx(&targetPos, mtx, mCapTargetInfo1->mEscapeLocalOffset);
        else
            targetPos.set(mtx.m[0][3], mtx.m[1][3], mtx.m[2][3]);
        if ((hitPos - targetPos).dot(_240) <= 0.0f)
            goto collideFail;
    }
    {
        *outPos = checkPos2;
        f32 dist = (checkPos2 - hitPos).dot(hitNormal);
        if (dist >= radius)
            return true;
        *outPos = checkPos2 + hitNormal * (radius - dist);
        sead::Vector3f moveDir = *outPos - hitPos;
        if (!al::tryNormalizeOrZero(&moveDir))
            moveDir = hitNormal;
        sead::Vector3f hitPos3(0.0f, 0.0f, 0.0f);
        sead::Vector3f hitNormal3(0.0f, 0.0f, 0.0f);
        sead::Vector3f from3 = *outPos - hitNormal * dist;
        sead::Vector3f dir3 = hitNormal * (radius + dist);
        if (!alCollisionUtil::getHitPosAndNormalOnArrow(this, &hitPos3, &hitNormal3, from3, dir3,
                                                         nullptr, nullptr))
            return true;
        collideThrowStartArrow(hitSensor, hitPos, hitNormal, targetPos);
        if (_2a5[2])
            al::hideModelIfShow(this);
        return false;
    }
collideFail:
    collideThrowStartArrow(hitSensor, hitPos, hitNormal, targetPos);
    if (_2a5[2])
        al::hideModelIfShow(this);
    return false;
}

void HackCap::exeRebound() {
    _510 = HackCapFunction::isKeepSeparateHold(mPlayerSeparateCapFlag, mCapJudgePreInputSeparateThrow,
                                               _510);
    if (al::isFirstStep(this)) {
        al::startAction(this, "ReboundStart");
        _29e = false;
        _274 = 0;
        _264 = 0.0f;
        _268 = sead::Vector2f::zero;
        _270 = 0.0f;
        if (_298)
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        else
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        sead::Vector3f velocity = sead::Vector3f::zero;
        if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && _510) {
            sead::Vector3f verticalVel = sead::Vector3f::zero;
            al::verticalizeVec(&verticalVel, _24c, al::getVelocity(this));
            f32 length = verticalVel.length();
            f32 speedCap;
            if (_29f) {
                speedCap = 60.0f;
            } else if (!al::isNearZero(_268.x, 0.001f)) {
                if (_29a)
                    speedCap = mHackCapThrowParam->mWaterMaxSpeed->value;
                else if (_29e || _4d0)
                    speedCap = mHackCapThrowParam->mContinuousThrowSpeed->value;
                else
                    speedCap = mHackCapThrowParam->mMaxVel->value;
            } else if (!al::isNearZero(_268.y, 0.001f)) {
                speedCap = mHackCapThrowParam->mRollSpeed->value;
            } else {
                if (_29a)
                    speedCap = mHackCapThrowParam->mWaterMaxSpeed->value;
                else if (_29e || _4d0)
                    speedCap = mHackCapThrowParam->mContinuousThrowSpeed->value;
                else
                    speedCap = mHackCapThrowParam->mMaxVel->value;
            }
            f32 speed = length > speedCap ? length : speedCap;
            velocity = _240 * speed;
        }
        // Zeroes mRotateZ and _c together (single 8-byte store in the
        // original); two separate assignments do not combine.
        *reinterpret_cast<u64*>(&mHackCapJointControlKeeper->mRotateZ) = 0;
        al::setVelocity(this, velocity);
    }
    if (_4d0) {
        if (al::isGreaterEqualStep(this, 1)) {
            _290 = 3;
            sead::Vector3f diff = al::getTrans(this) - _4d4;
            if (diff.length() < mHackCapThrowParam->mTornadoMaxDist->value) {
                _278 += 3;
                _4e0 = al::getTrans(this);
                al::setNerve(this, &NrvHackCap.ThrowTornado);
            } else {
                al::setNerve(this, &NrvHackCap.Return);
            }
        }
        return;
    }
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        updateWaterArea();
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && _510) {
        if (al::isGreaterEqualStep(this, 7)) {
            mHackCapTrigger->set(HackCapTrigger::EAfterMovementTrigger::Val2);
            al::setNerve(this, &NrvHackCap.ThrowStay);
        }
        return;
    }
    if (al::isGreaterEqualStep(this, 3)) {
        _290 = 3;
        al::setNerve(this, &NrvHackCap.Return);
    }
}

void HackCap::exeReturn() {
    if (al::isFirstStep(this)) {
        if (al::isActionPlaying(this, "ReboundStart")) {
            al::startAction(this, "ReboundEnd");
        } else if (al::isNearZero(_268.x, 0.001f)) {
            al::tryStartActionIfNotPlaying(this, "FlyingWaitR");
        } else {
            al::tryStartActionIfNotPlaying(this, "FlyingWaitSpiral");
        }
        if (_298) {
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        } else {
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        }
        *reinterpret_cast<u64*>(&_284) = 0;
        al::showModelIfHide(this);
        mEquipmentHat->kill();
        mLockOnCapEyes->kill();
        _4d0 = false;
        al::setScaleAll(this, 1.0f);
    }

    if (_290 > 0) {
        _290--;
        if (_290 == 0) {
            al::onCollide(this);
        }
    }

    updateThrowJoint();

    if (tryCollideWallLockOn()) {
        return;
    }

    updateWaterArea();

    sead::Vector3f targetPos;
    if (mIsSeparateFlying) {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        mCapTargetInfo1->makeLockOnMtx(&mtx);
        targetPos = mtx.getTranslation();
    } else {
        targetPos = rs::getPlayerHeadPos(this);
    }

    sead::Vector3f upDir(0.0f, 0.0f, 0.0f);
    al::calcUpDir(&upDir, mPlayerActor);

    sead::Vector3f targetDir = targetPos - al::getTrans(this);
    sead::Vector3f dir(0.0f, 0.0f, 0.0f);
    if (al::tryNormalizeOrZero(&dir, targetDir)) {
        f32 dist = targetDir.length();
        sead::Vector3f dirXZ = targetDir;
        dirXZ.y = 0.0f;
        if (dirXZ.length() < 200.0f) {
            sead::Vector3f upDir2(0.0f, 0.0f, 0.0f);
            al::calcUpDir(&upDir2, this);
            _288 = sead::Mathf::clamp(_288, 0.0f, dist * 0.8f);
            if (!al::isParallelDirection(upDir2, dir, 0.01f)) {
                sead::Quatf quat = sead::Quatf::unit;
                al::makeQuatFrontUp(&quat, dir, upDir2);
                al::slerpQuat(&quat, al::getQuat(this), quat, 0.25f);
                al::updatePoseQuat(this, quat);
            }
        }
        sead::Vector3f frontDir = sead::Vector3f::ez;
        al::turnFrontSpherical(this, &frontDir, dir,
                               sead::Mathf::cos(sead::Mathf::deg2rad(
                                   mHackCapThrowParam->mTurnAngleLimit->value)));
        sead::Vector3f upDir3 = -al::getGravity(this);
        if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
            sead::Vector3f sideDir(0.0f, 0.0f, 0.0f);
            al::calcSideDir(&sideDir, this);
            upDir3 = frontDir.cross(sideDir);
            if (!al::tryNormalizeOrZero(&upDir3)) {
                al::calcUpDir(&upDir3, this);
            }
        }
        if (!al::isParallelDirection(frontDir, upDir3, 0.01f)) {
            sead::Quatf quat = sead::Quatf::unit;
            al::makeQuatFrontUp(&quat, frontDir, upDir3);
            al::updatePoseQuat(this, quat);
        }
        f32 speed = _288 + mHackCapThrowParam->mReturnStrength->value;
        f32 cap = (_29a ? mHackCapThrowParam->mWaterMaxRetSpeed : mHackCapThrowParam->mMaxRetSpeed)
                      ->value;
        _288 = sead::Mathf::clamp(speed, 0.0f, cap);
        sead::Vector3f parallelVel(0.0f, 0.0f, 0.0f);
        sead::Vector3f verticalVel(0.0f, 0.0f, 0.0f);
        al::separateVelocityParallelVertical(&parallelVel, &verticalVel, this, frontDir);
        sead::Vector3f vel =
            frontDir * _288 + verticalVel * (1.0f - al::calcNerveEaseInRate(this, 10));
        al::setVelocity(this, vel);
    }

    const sead::Vector3f& normal =
        rs::isCollidedWall(mPlayerColliderHackCap)
            ? rs::getCollidedWallNormal(mPlayerColliderHackCap)
            : rs::isCollidedCeiling(mPlayerColliderHackCap)
                  ? rs::getCollidedCeilingNormal(mPlayerColliderHackCap)
                  : rs::isCollidedGround(mPlayerColliderHackCap)
                        ? rs::getCollidedGroundNormal(mPlayerColliderHackCap)
                        : sead::Vector3f(0.0f, 0.0f, 0.0f);
    if (normal.dot(dir) < -0.08715574f) {
        _284++;
    } else {
        _284 = 0;
    }
    if (al::isGreaterStep(this, 180) || _284 > 45) {
        if (mIsSeparateFlying) {
            endHackThrowAndReturnHack();
        } else {
            hide(false);
        }
        return;
    }
    if (mIsSeparateFlying) {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        mCapTargetInfo1->makeLockOnMtx(&mtx);
        sead::Vector3f targetPos2 = mtx.getTranslation();
        if ((targetPos2 - al::getTrans(this)).length() < 100.0f) {
            if (mIsSeparateFlying) {
                endHackThrowAndReturnHack();
            } else {
                hide(false);
            }
            return;
        }
    }
    if (_290 == 0 && _299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && !mInput->isNoInputSeparateCap()) {
        _299 = false;
        _1b0 = 0;
        _1ac = 0;
        _510 = true;
        rs::resetJudge(mCapJudgePreInputSeparateThrow);
        al::calcFrontDir(&_240, this);
        _240 = -_240;
        al::normalize(&_240);
        al::calcUpDir(&_24c, this);
        al::normalize(&_24c);
        al::setNerve(this, &NrvHackCap.ThrowStay);
    }
}

void HackCap::exeLockOn() {
    _580 = al::converge(_580, 10, 1);
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(this, "NoAction"))
            al::copyAction(mEquipmentHat, this);
        updateEquipmentHatVisibility(false, mEquipmentHat, mEquipmentHatDepthShadow, mCapTargetInfo1,
                                     false);
        if (mCapTargetInfo1->mIsInvalidCapEye) {
            if (al::isAlive(mLockOnCapEyes))
                mLockOnCapEyes->kill();
        } else if (al::isDead(mLockOnCapEyes)) {
            mLockOnCapEyes->appear();
        }
        al::hideModelIfShow(this);
    }
    bool isLockOnTarget = false;
    if (mCapTargetInfo1->mIsLockOnOnly && mCapTargetInfo1->mIsLockOn) {
        isLockOnTarget = true;
        goto updateThrowJoint;
    }
    if (al::isNerve(this, &NrvHackCap.LockOn) || al::isNerve(this, &NrvHackCap.Hack) ||
        al::isNerve(this, &NrvHackCap.TrampleLockOn)) {
        if (mLockOnCounter > 3)
            goto checkAction;
        goto updateThrowJoint;
    }
    goto checkAction;
updateThrowJoint:
    updateThrowJoint();
checkAction:
    if (al::isNerve(this, &NrvHackCap.LockOn) || al::isNerve(this, &NrvHackCap.Hack) ||
        al::isNerve(this, &NrvHackCap.TrampleLockOn)) {
        if (mLockOnCounter >= 4)
            goto check2a6;
        if (_2a5[1])
            goto lockOnStartCheck;
        goto end;
    }
check2a6:
    if (_2a5[1])
        goto lockOnStartCheck;
    if (isLockOnTarget) {
        if (mCapTargetInfo1->mIsLockOnOnly) {
            al::startAction(this, "NoAction");
            al::startAction(mEquipmentHat, "LockOn");
            al::startAction(mEquipmentHatDepthShadow, "LockOn");
            if (al::isAlive(mLockOnCapEyes)) {
                if (!al::tryStartAction(mLockOnCapEyes, "LockOn"))
                    al::tryStartActionIfNotPlaying(mLockOnCapEyes, "Wait");
            }
        }
    } else {
        const char* action;
        if (mCapTargetInfo1->mIsLockOnStart)
            action = mCapTargetInfo1->mLockOnStartAnimName;
        else
            action = mCapTargetInfo1->mLockOnAnimName;
        al::startAction(this, "NoAction");
        al::startAction(mEquipmentHat, action);
        al::startAction(mEquipmentHatDepthShadow, action);
        if (al::isAlive(mLockOnCapEyes)) {
            if (!al::tryStartAction(mLockOnCapEyes, action))
                al::tryStartActionIfNotPlaying(mLockOnCapEyes, "Wait");
        }
    }
    _2a5[1] = true;
lockOnStartCheck:
    if (mCapTargetInfo1->mIsLockOnStart &&
        al::isActionPlaying(mEquipmentHat, mCapTargetInfo1->mLockOnStartAnimName) &&
        al::isActionEnd(mEquipmentHat)) {
        al::startAction(this, "NoAction");
        al::startAction(mEquipmentHat, mCapTargetInfo1->mLockOnAnimName);
        al::startAction(mEquipmentHatDepthShadow, mCapTargetInfo1->mLockOnAnimName);
        if (al::isAlive(mLockOnCapEyes)) {
            if (!al::tryStartAction(mLockOnCapEyes, mCapTargetInfo1->mLockOnAnimName))
                al::tryStartActionIfNotPlaying(mLockOnCapEyes, "Wait");
        }
    }
end:
    if (!mIsPuppet && !al::isDead(mCapTargetInfo1->mActor) &&
        rs::sendMsgCapKeepLockOn(const_cast<al::HitSensor*>(_1d0), mAttackSensor))
        return;
    tryReturn(false, nullptr);
}

void HackCap::exeBlow() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "FlyingWaitR");
    updateThrowJoint();
    al::scaleVelocity(this, 0.95f);
    if (al::isFirstStep(this))
        return;
    _4f0 = al::converge(_4f0, 0x258, 1);
    if (mCapExternalVelocity->isExistForce() && _4f0 < 0x258)
        return;
    sead::Vector3f forceDir;
    al::tryNormalizeOrZero(&forceDir, mCapExternalVelocity->mTotalVelocity);
    sead::Vector3f newVel =
        mCapExternalVelocity->mTotalVelocity + al::getVelocity(this);
    f32 forceLen = mCapExternalVelocity->mTotalVelocity.length();
    f32 dot = newVel.dot(forceDir);
    f32 absDot = sead::Mathf::abs(dot);
    if (tryCollideWallLockOn())
        return;
    if (_4f0 >= 0x258) {
        if (mIsSeparateFlying) {
            endHackThrowAndReturnHack();
            return;
        }
        hide(false);
        return;
    }
    if (forceLen > 10.0f && absDot > 10.0f)
        return;
    al::setVelocity(this, newVel);
    mCapExternalVelocity->reset();
    if (!_4ec || !al::isLessEqualStep(this, 10)) {
        _29e = false;
        _274 = 0;
        _268 = sead::Vector2f::zero;
        _264 = 0.0f;
        if (_298)
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        else
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        al::setNerve(this, &NrvHackCap.Return);
        return;
    }
    _27c = _27c - _278 - al::getNerveStep(this);
    if (_27c < 0)
        _27c = 0;
    if (_27c != 0) {
        f32 speed;
        f32 cap;
        if (_29f) {
            speed = forceLen + 60.0f;
            cap = 60.0f;
        } else {
            f32 param;
            if (_29a)
                param = mHackCapThrowParam->mMaxVel->value;
            else if (_29e || _4d0)
                param = mHackCapThrowParam->mContinuousThrowSpeed->value;
            else
                param = mHackCapThrowParam->mMaxVel->value;
            speed = forceLen + param;
            if (!al::isNearZero(_268.x, 0.1f)) {
                if (_29a)
                    cap = mHackCapThrowParam->mWaterMaxSpeed->value;
                else if (_29e || _4d0)
                    cap = mHackCapThrowParam->mContinuousThrowSpeed->value;
                else
                    cap = mHackCapThrowParam->mMaxVel->value;
            } else if (!al::isNearZero(_268.y, 0.1f)) {
                cap = mHackCapThrowParam->mRollSpeed->value;
            } else {
                if (_29a)
                    cap = mHackCapThrowParam->mWaterMaxSpeed->value;
                else if (_29e || _4d0)
                    cap = mHackCapThrowParam->mContinuousThrowSpeed->value;
                else
                    cap = mHackCapThrowParam->mMaxVel->value;
            }
        }
        sead::Vector3f addVel = _240 * speed;
        al::tryAddVelocityLimit(this, addVel, cap);
    }
    if (_4d0) {
        al::setNerve(this, &NrvHackCap.ThrowTornado);
        return;
    }
    if (!al::isNearZero(_268.x, 0.1f) || !al::isNearZero(_268.y, 0.1f)) {
        _29e = false;
        _274 = 0;
        _268 = sead::Vector2f::zero;
        _264 = 0.0f;
        if (_298)
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        else
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        if (_27c != 0)
            al::setNerve(this, &NrvHackCap.Rethrow);
        else
            al::setNerve(this, &NrvHackCap.ThrowBrake);
        return;
    }
    if (_27c != 0)
        al::setNerve(this, &NrvHackCap.ThrowRolling);
    else
        al::setNerve(this, &NrvHackCap.ThrowRollingBrake);
}

void HackCap::exeThrowAppend() {
    if (al::isFirstStep(this)) {
        const char* action;
        if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
            action = "FlyingWaitRolling";
        } else {
            action = _264 > 0.0f ? (_29e ? "FlyingWaitCapCatchL" : "FlyingWaitL")
                                 : (_29e ? "FlyingWaitCapCatchR" : "FlyingWaitR");
        }
        al::tryStartActionIfNotPlaying(this, action);
        al::startHitReaction(this, "帽子追加攻撃");
    }
    al::setVelocity(this, _2e8 * getThrowSpeedAppend());
    if (_238 == SwingHandType::Val2) {
        alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 0.15f, 0.75f, -1);
    } else {
        alPadRumbleFunction::startPadRumbleWithVolume(this, "WaveAttackOne1", 0.75f, 0.15f, -1);
    }
    updateThrowJoint();
    changeThrowParamInWater(al::getNerveStep(this), false);
    if (tryCollideReflectReaction())
        return;
    if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        if (tryCollideWallReactionRollingGround())
            return;
        rollingGround();
    } else {
        if (tryCollideWallReaction())
            return;
        calcThrowMove(_240, _24c, &_28c, mPlayerColliderHackCap, false,
                      mHackCapThrowParam->mHackThrowHeight->value);
    }
    s32 step = sead::Mathf::floor(getThrowRangeAppend() / getThrowSpeedAppend());
    if (!al::isGreaterEqualStep(this, step))
        return;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0 && _510) {
        mHackCapTrigger->set(HackCapTrigger::EAfterMovementTrigger::Val2);
        al::setNerve(this, &NrvHackCap.ThrowStay);
    } else {
        al::setNerve(this, &NrvHackCap.Return);
    }
}

void HackCap::exeThrowTornado() {
    _278++;
    if (al::isFirstStep(this)) {
        const char* action = _264 > 0.0f ? (_29e ? "FlyingWaitCapCatchL" : "FlyingWaitL")
                                         : (_29e ? "FlyingWaitCapCatchR" : "FlyingWaitR");
        al::tryStartActionIfNotPlaying(this, action);
        al::startHitReaction(this, "帽子投げ");
    }
    al::setVelocity(this, _240 * getThrowSpeed());
    _290 = al::converge(_290, 0, 1);
    updateThrowJoint();
    if (tryCollideReflectReaction())
        return;
    if (tryCollideWallReactionReflect())
        return;
    calcThrowMove(_240, _24c, &_28c, mPlayerColliderHackCap, true,
                  mHackCapThrowParam->mHackThrowHeight->value);
    changeThrowParamInWater(al::getNerveStep(this), false);
    calcThrowRollingDirection(_240, _24c, mInput, mPlayerSeparateCapFlag, _299);
    addCurveOffset();
    al::scaleVelocityExceptDirection(this, _240, 0.94f);
    f32 maxDist;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        maxDist = sThrowRange[_29a];
    } else if (_29f) {
        maxDist = 1200.0f;
    } else if (_4d0) {
        maxDist = mHackCapThrowParam->mTornadoDist->value;
    } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        maxDist = (_268.y > 0.0f ? mHackCapThrowParam->mRollDistTop
                                 : mHackCapThrowParam->mRollDistBottom)
                      ->value;
    } else {
        maxDist = (_29a ? mHackCapThrowParam->mWaterDist : mHackCapThrowParam->mMaxDist)->value;
    }
    f32 dist = (al::getTrans(this) - _4e0).length();
    if (maxDist < dist || _278 > mHackCapThrowParam->mTornadoReflectTime->value)
        al::setNerve(this, &NrvHackCap.ThrowStay);
}

void HackCap::exeTrample() {
    if (al::isFirstStep(this)) {
        sead::Vector3f dir(0.0f, 0.0f, 0.0f);
        al::calcJointFrontDir(&dir, this, "AllRoot");
        al::faceToDirection(this, dir);
        al::startAction(this, "Trample");
        al::startAction(mThrowingHatEyes, "Wait");
        al::setVelocityZero(this);
        HackCapJointControlKeeper* jointControlKeeper = mHackCapJointControlKeeper;
        jointControlKeeper->mRotateZ = 0.0f;
        jointControlKeeper->_c = 0;
        al::startHitReaction(this, "通常Trample");
        if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
            al::startHitReaction(this, "おすそ分けTrample");
    }
    if (al::isLessStep(this, 15))
        return;
    if (al::isNerve(this, &NrvHackCap.TrampleReturn) || _299 ||
        (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0 ||
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0 || !_510)
        al::setNerve(this, &NrvHackCap.Return);
    else
        al::setNerve(this, &NrvHackCap.ThrowStay);
}

void HackCap::exeTrampleLockOn() {
    if (al::isFirstStep(this)) {
        sead::Vector3f dir(0.0f, 0.0f, 0.0f);
        al::calcJointFrontDir(&dir, this, "AllRoot");
        al::faceToDirection(this, dir);
        al::LiveActor* equipmentHat = mEquipmentHat;
        al::LiveActor* equipmentHatDepthShadow = mEquipmentHatDepthShadow;
        al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
        al::startAction(equipmentHat, "TrampleLockOn");
        al::startAction(equipmentHatDepthShadow, "TrampleLockOn");
        if (al::isAlive(lockOnCapEyes) && !al::tryStartAction(lockOnCapEyes, "TrampleLockOn"))
            al::tryStartActionIfNotPlaying(lockOnCapEyes, "Wait");
    }
    if (al::isLessStep(this, 15))
        return;
    if (!mCapTargetInfo1->mIsLockOn) {
        const char* actionName = mCapTargetInfo1->mLockOnAnimName;
        al::LiveActor* equipmentHat = mEquipmentHat;
        al::LiveActor* equipmentHatDepthShadow = mEquipmentHatDepthShadow;
        al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
        al::startAction(equipmentHat, actionName);
        al::startAction(equipmentHatDepthShadow, actionName);
        if (al::isAlive(lockOnCapEyes) && !al::tryStartAction(lockOnCapEyes, actionName))
            al::tryStartActionIfNotPlaying(lockOnCapEyes, "Wait");
    }
    al::setNerve(this, &NrvHackCap.LockOn);
}

bool HackCap::calcThrowRollingDirection(sead::Vector3f& dir, sead::Vector3f& pos, const PlayerInput* input,
                                        const PlayerSeparateCapFlag* flag, bool isSeparate) {
    if (isSeparate) {
        return false;
    }
    if ((flag->_flagsWord & 0xff0000) != 0) {
        return false;
    }
    if ((flag->_flagsWord & 0xff) == 0) {
        return false;
    }
    sead::Vector3f moveInput(0.0f, 0.0f, 0.0f);
    sead::Vector3f normalized(0.0f, 0.0f, 0.0f);
    input->calcCapSeparateMoveInput(&moveInput, pos);
    if (!al::tryNormalizeOrZero(&normalized, moveInput)) {
        return false;
    }
    sead::Vector3f cross = dir.cross(normalized);
    f32 dot = pos.dot(cross);
    al::verticalizeVec(&moveInput, dir, moveInput);
    f32 angle = moveInput.length() * 40.0f;
    const sead::Vector3f& velocity = al::getVelocity(this);
    f32 velDot = dir.dot(velocity);
    sead::Vector3f axis = pos;
    if (dot < 0.0f) {
        axis.x = -axis.x;
        axis.y = -axis.y;
        axis.z = -axis.z;
    }
    al::rotateVectorDegree(&dir, dir, axis, angle);
    al::normalize(&dir);
    al::setVelocity(this, dir * velDot);
    return true;
}

void HackCap::exeThrowRolling() {
    _278++;
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "FlyingWaitRolling");
    updateThrowJoint();
    changeThrowParamInWater(al::getNerveStep(this), false);
    if (tryCollideReflectReaction())
        return;
    if (tryCollideWallReactionRollingGround())
        return;
    rollingGround();
    calcThrowRollingDirection(_240, _24c, mInput, mPlayerSeparateCapFlag, _299);
    al::scaleVelocityExceptDirection(this, _240, 0.94f);
    if (!al::isLessStep(this, _27c))
        al::setNerve(this, &NrvHackCap.ThrowRollingBrake);
}

void HackCap::calcThrowReflect(sead::Vector3f& dir, sead::Vector3f& pos, HackCap* cap,
                               const sead::Vector3f& normal) {
    sead::Vector3f origDir;
    __builtin_memcpy(&origDir, &dir, sizeof(sead::Vector3f));
    al::calcReflectionVector(&dir, normal, 1.0f, 0.0f);
    al::tryNormalizeOrZero(&dir);
    al::setVelocityZero(cap);
    if (al::isNearDirection(dir, origDir)) {
        __builtin_memcpy(&dir, &origDir, sizeof(sead::Vector3f));
    } else if (al::isReverseDirection(dir, origDir)) {
        dir = -origDir;
    } else {
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatRotationRate(&quat, origDir, dir, 1.0f);
        pos.rotate(quat);
        al::normalize(&pos);
    }
}

bool HackCap::tryCollideReflectReaction() {
    sead::Vector3f reflectNormal(0.0f, 0.0f, 0.0f);
    al::HitSensor* sensor = rs::tryGetCollidedGroundSensor(mPlayerColliderHackCap);
    if (sensor && rs::sendMsgCapReflectCollide(sensor, mAttackSensor)) {
        reflectNormal = rs::getCollidedGroundNormal(mPlayerColliderHackCap);
    } else if ((sensor = rs::tryGetCollidedWallSensor(mPlayerColliderHackCap)) &&
               rs::sendMsgCapReflectCollide(sensor, mAttackSensor)) {
        reflectNormal = rs::getCollidedWallNormal(mPlayerColliderHackCap);
    } else if ((sensor = rs::tryGetCollidedCeilingSensor(mPlayerColliderHackCap)) &&
               rs::sendMsgCapReflectCollide(sensor, mAttackSensor)) {
        reflectNormal = rs::getCollidedCeilingNormal(mPlayerColliderHackCap);
    } else {
        return false;
    }
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        calcThrowReflect(_240, _24c, this, reflectNormal);
    }
    al::setNerve(this, &NrvHackCap.Rebound);
    return true;
}

bool HackCap::tryCollideWallReactionSpiral() {
    if (tryCollideWallLockOn())
        return true;
    bool isHit = false;
    if (trySendAttackCollideAndReaction(&isHit))
        return isHit;
    if (!rs::isCollidedWall(mPlayerColliderHackCap))
        return false;
    sead::Vector3f wallNormal;
    __builtin_memcpy(&wallNormal, &rs::getCollidedWallNormal(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    sead::Vector3f wallPos;
    __builtin_memcpy(&wallPos, &rs::getCollidedWallPos(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    al::verticalizeVec(&wallNormal, al::getGravity(this), wallNormal);
    if (!al::tryNormalizeOrZero(&wallNormal))
        return false;
    if (!rs::sendMsgCapTouchWall(rs::tryGetCollidedWallSensor(mPlayerColliderHackCap), mAttackSensor,
                                 wallPos, wallNormal))
        return false;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        calcThrowReflect(_240, _24c, this, wallNormal);
    al::setNerve(this, &NrvHackCap.Rebound);
    return true;
}

bool HackCap::tryCollideWallReactionStay() {
    if (tryCollideWallLockOn())
        return true;
    bool isHit = false;
    if (trySendAttackCollideAndReaction(&isHit))
        return isHit;
    if (!rs::isCollidedWall(mPlayerColliderHackCap))
        return false;
    sead::Vector3f wallNormal;
    __builtin_memcpy(&wallNormal, &rs::getCollidedWallNormal(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    sead::Vector3f wallPos;
    __builtin_memcpy(&wallPos, &rs::getCollidedWallPos(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    al::verticalizeVec(&wallNormal, al::getGravity(this), wallNormal);
    if (!al::tryNormalizeOrZero(&wallNormal))
        return false;
    if (!rs::sendMsgCapTouchWall(rs::tryGetCollidedWallSensor(mPlayerColliderHackCap), mAttackSensor,
                                 wallPos, wallNormal))
        return false;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        calcThrowReflect(_240, _24c, this, wallNormal);
    al::setNerve(this, &NrvHackCap.Rebound);
    return true;
}

bool HackCap::tryCollideWallReactionReflect() {
    const al::CollisionParts* parts = nullptr;
    sead::Vector3f normal(0.0f, 0.0f, 0.0f);
    sead::Vector3f pos(0.0f, 0.0f, 0.0f);
    sead::Delegate2RFunc<al::HitSensor*, al::HitSensor*, bool> delegate(rs::sendMsgCapAttackCollide);
    tryGetCollidedInfo(&parts, &normal, &pos, mPlayerColliderHackCap, mAttackSensor, &delegate);
    if (!rs::isCollidedWall(mPlayerColliderHackCap))
        return false;
    sead::Vector3f wallNormal;
    __builtin_memcpy(&wallNormal, &rs::getCollidedWallNormal(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    sead::Vector3f wallPos;
    __builtin_memcpy(&wallPos, &rs::getCollidedWallPos(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    if (wallNormal.dot(al::getVelocity(this)) > 0.0f)
        return false;
    al::verticalizeVec(&wallNormal, al::getGravity(this), wallNormal);
    if (!al::tryNormalizeOrZero(&wallNormal))
        return false;
    if (rs::sendMsgCapTouchWall(rs::tryGetCollidedWallSensor(mPlayerColliderHackCap), mAttackSensor,
                                wallPos, wallNormal)) {
        calcThrowReflect(_240, _24c, this, rs::getCollidedWallNormal(mPlayerColliderHackCap));
    } else if (_24c.dot(wallNormal) < 0.5f ||
               wallNormal.dot(al::getGravity(this)) >= -0.08715574f) {
        if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
            return false;
    } else {
        if (!tryReflectThrow(this, _240, _24c, wallNormal, wallPos, true))
            return false;
        _270 = 0.0f;
        _274 = 0.0f;
        return false;
    }
    al::setNerve(this, &NrvHackCap.Rebound);
    return true;
}

void HackCap::collideThrowStartArrow(al::HitSensor* sensor, const sead::Vector3f& pos1,
                                     const sead::Vector3f& pos2, const sead::Vector3f& pos3) {
    if (_1b0) {
        al::LiveActor* host = al::getSensorHost(sensor);
        for (s32 i = 0; i < _1b0; i++) {
            s32 index = _1ac + i;
            if (index >= _1a8)
                index -= _1a8;
            if (static_cast<al::LiveActor**>(_1a0_arr)[index] == host)
                goto wallPath;
        }
    }
    if (rs::sendMsgCapStartLockOn(sensor, mAttackSensor)) {
        prepareLockOn(sensor);
        return;
    }
wallPath:
    if (al::isWallPolygon(pos2, al::getGravity(mPlayerActor)))
        rs::sendMsgCapTouchWall(sensor, mAttackSensor, pos1, pos2);
    rs::sendMsgCapReflectCollide(sensor, mAttackSensor);
    rs::sendMsgCapAttackCollide(sensor, mAttackSensor);
    _2a5[2] = true;
    al::resetPosition(this, pos3);
    rs::resetCollision(mPlayerColliderHackCap);
    al::setNerve(this, &NrvHackCap.Return);
}

void HackCap::updateThrowJoint() {
    f32 rotateY;
    if (!al::isNerve(this, &NrvHackCap.LockOn) ||
        ((al::isNerve(this, &NrvHackCap.LockOn) || al::isNerve(this, &NrvHackCap.Hack) ||
          al::isNerve(this, &NrvHackCap.TrampleLockOn)) &&
         mLockOnCounter < 4)) {
        rotateY = _29e ? 45.0f : sThrowJointRotateY[_4d0];
        if (_29a)
            rotateY = 20.0f;
    } else {
        rotateY = _29a ? 5.0f : 10.0f;
    }
    if (_298)
        rotateY = -rotateY;
    mHackCapJointControlKeeper->updateRotateY(rotateY);
    if (al::isNerve(this, &NrvHackCap.ThrowStay)) {
        mHackCapJointControlKeeper->updateRotateStayZ(0.25f);
    } else {
        mHackCapJointControlKeeper->updateRotateThrowZ(0.002f, 0.0f);
    }
}

void HackCap::addCurveOffset() {
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        return;
    }
    if (al::isNearZero(_270, 0.001f))
        return;
    f32 speed = al::calcSpeed(this);
    if (_274 <= 0.0f) {
        _274 += speed;
        return;
    }
    f32 t = _274 / _270;
    f32 u = (speed + _274) / _270;
    f32 curve1 = sead::Mathf::sin(sead::Mathf::deg2rad(fminf(u, 1.0f) * 180.0f)) * 40.0f;
    f32 curve2 = sead::Mathf::sin(sead::Mathf::deg2rad(fminf(t, 1.0f) * 180.0f)) * 40.0f;
    sead::Vector3f cross = _240.cross(_24c);
    f32 delta = curve1 - curve2;
    if (!al::isNearZero(mHackCapJointControlKeeper->mRotateZ, 0.001f)) {
        al::rotateVectorDegree(&cross, cross, _240, -mHackCapJointControlKeeper->mRotateZ);
    }
    al::normalize(&cross);
    al::setTrans(this, al::getTrans(this) + cross * delta * _264);
    _274 += speed;
}

bool HackCap::trySendAttackCollideAndReaction(bool* out) {
    const al::CollisionParts* outParts = nullptr;
    sead::Vector3f outNormal(0.0f, 0.0f, 0.0f);
    sead::Vector3f outPos(0.0f, 0.0f, 0.0f);

    bool isFirstPath;
    if (_299)
        isFirstPath = mInput->isHoldAction();
    else if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
             (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        isFirstPath = mInput->isHoldSpinCap();
    else
        isFirstPath = _510;

    if (isFirstPath) {
        sead::Delegate2RFunc<al::HitSensor*, al::HitSensor*, bool> delegate(
            rs::sendMsgCapAttackStayRollingCollide);
        if (tryGetCollidedInfo(&outParts, &outNormal, &outPos, mPlayerColliderHackCap, mAttackSensor,
                               &delegate)) {
            _560[0] = (void*)outParts;
            *(sead::Vector3f*)((char*)_560 + 8) = outPos;
            *(sead::Vector3f*)((char*)_560 + 20) = outNormal;
            *al::getTransPtr(this) -= outNormal;
            _294 = 20;
            *out = stayWallHit();
            return true;
        }
    }

    sead::Delegate2RFunc<al::HitSensor*, al::HitSensor*, bool> delegate(rs::sendMsgCapAttackCollide);
    if (!tryGetCollidedInfo(&outParts, &outNormal, &outPos, mPlayerColliderHackCap, mAttackSensor,
                            &delegate))
        return false;

    if (!al::isNearZero(_268.x, 0.001f))
        return true;
    if (!al::isWallPolygon(outNormal, al::getGravity(this))) {
        *out = true;
        al::setVelocityZero(this);
        al::setNerve(this, &NrvHackCap.Rebound);
        return true;
    }
    if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f))
        return true;
    if (_530)
        al::setVelocity(this, mWallVelocity);
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        if (_530)
            _530 = false;
        calcThrowReflect(_240, _24c, this, outNormal);
        *out = true;
        al::setVelocityZero(this);
        al::setNerve(this, &NrvHackCap.Rebound);
        return true;
    }
    if (tryReflectThrow(this, _240, _24c, outNormal, outPos, true)) {
        _560[0] = (void*)outParts;
        *(sead::Vector3f*)((char*)_560 + 8) = outPos;
        *(sead::Vector3f*)((char*)_560 + 20) = outNormal;
        _270 = 0.0f;
        _274 = 0.0f;
        if (_530) {
            _530 = false;
            *out = true;
            s32 new27c = _27c - _278;
            if (new27c < 0)
                new27c = 0;
            _27c = new27c;
            if (_27c == 0)
                al::setNerve(this, &NrvHackCap.ThrowBrake);
            else
                al::setNerve(this, &NrvHackCap.Rethrow);
        } else {
            *out = false;
        }
        return true;
    }
    *out = true;
    al::setVelocityZero(this);
    al::setNerve(this, &NrvHackCap.Rebound);
    return true;
}

void HackCap::rollingGround() {
    sead::Vector3f collPos(0.0f, 0.0f, 0.0f);
    sead::Vector3f collNormal(0.0f, 0.0f, 0.0f);
    if (rs::isOnGround(this, mPlayerColliderHackCap)) {
        collNormal = rs::getCollidedGroundNormal(mPlayerColliderHackCap);
        collPos = rs::getCollidedGroundPos(mPlayerColliderHackCap);
    } else if (rs::isCollidedWall(mPlayerColliderHackCap)) {
        collNormal = rs::getCollidedWallNormal(mPlayerColliderHackCap);
        collPos = rs::getCollidedWallPos(mPlayerColliderHackCap);
    } else if (rs::isCollidedCeiling(mPlayerColliderHackCap)) {
        collNormal = rs::getCollidedCeilingNormal(mPlayerColliderHackCap);
        collPos = rs::getCollidedCeilingPos(mPlayerColliderHackCap);
    }
    if (al::isNearZero(collNormal, 0.1f)) {
        al::verticalizeVec(&collNormal, _24c, collNormal);
        if (!al::tryNormalizeOrZero(&collNormal))
            return;
        f32 speed = 0.0f;
        sead::Vector3f velDir;
        if (al::separateScalarAndDirection(&speed, &velDir, al::getVelocity(this)))
            return;
        sead::Vector3f axis = collNormal.cross(_24c);
        al::normalize(&axis);
        f32 dot = axis.dot(_240);
        if (dot > 0.0f) {
            _240 = axis;
        } else {
            sead::Vector3f reflected = axis - _240 * (dot * 2.0f);
            al::normalize(&reflected);
            _240 = reflected;
        }
        al::setVelocity(this, _240 * speed);
        sead::Quatf quat = sead::Quatf::unit;
        sead::Vector3f negDir = -_240;
        al::makeQuatFrontUp(&quat, negDir, _24c);
        al::slerpQuat(&quat, al::getQuat(this), quat,
                      mHackCapThrowParam->mRollGroundGroundedPoseTrack->value);
        al::updatePoseQuat(this, quat);
        _30c = true;
        al::makeMtxUpFrontPos(&_440, _24c, _240, collPos);
        al::tryEmitEffect(this, "FlyRollingGrounding", nullptr);
        return;
    }
    const sead::Vector3f& gravity = al::getGravity(this);
    sead::Vector3f gravityDir = (_268.y <= 0.0f) ? -gravity : gravity;
    sead::Vector3f axis = gravityDir.cross(_240);
    f32 crossLen = axis.length();
    f32 dot = gravityDir.dot(_240);
    f32 angle = sead::Mathf::atan2(crossLen, dot);
    if (!al::tryNormalizeOrZero(&axis)) {
        angle = al::isNearZero(crossLen, 0.1f) ? 0.0f : 3.1415927f;
    }
    f32 rollAngle = angle * mHackCapThrowParam->mRollGroundAerialPoseTrack->value * 0.5f;
    f32 cosTheta = sead::Mathf::cos(rollAngle);
    f32 sinTheta = sead::Mathf::sin(rollAngle);
    sead::Vector3f kCrossV = axis.cross(_240);
    f32 kDotV = axis.dot(_240);
    sead::Vector3f rotated = _240 * cosTheta + kCrossV * sinTheta +
                             axis * (kDotV * (1.0f - cosTheta));
    _240 = rotated;
    al::normalize(&_240);
    sead::Quatf quat = sead::Quatf::unit;
    sead::Vector3f negDir = -_240;
    al::makeQuatFrontUp(&quat, negDir, _24c);
    al::updatePoseQuat(this, quat);
    f32 speed = al::calcSpeed(this);
    al::setVelocity(this, _240 * speed);
    if (_30c) {
        al::tryDeleteEffect(this, "FlyRollingGrounding");
    }
}

bool HackCap::tryCollideWallReactionRollingGround() {
    if (tryCollideWallLockOn())
        return true;
    bool isHit = false;
    bool isAttack = trySendAttackCollideAndReaction(&isHit);
    if (isHit && isAttack)
        return true;
    sead::Vector3f crossDir = _24c.cross(_240);
    al::normalize(&crossDir);
    sead::Vector3f collPos(0.0f, 0.0f, 0.0f);
    sead::Vector3f collNormal(0.0f, 0.0f, 0.0f);
    if (rs::isCollidedWall(mPlayerColliderHackCap)) {
        const sead::Vector3f& wallNormal = rs::getCollidedWallNormal(mPlayerColliderHackCap);
        if (!al::isFloorPolygon(wallNormal, -crossDir)) {
            collNormal = wallNormal;
            collPos = rs::getCollidedWallPos(mPlayerColliderHackCap);
        }
    }
    if (_268.y > 0.0f) {
        if (al::isNearZero(collNormal, 0.1f) && rs::isCollidedGround(mPlayerColliderHackCap)) {
            const sead::Vector3f& groundNormal =
                rs::getCollidedGroundNormal(mPlayerColliderHackCap);
            if (!al::isFloorPolygon(groundNormal, -crossDir)) {
                collNormal = groundNormal;
                collPos = rs::getCollidedGroundPos(mPlayerColliderHackCap);
            }
        }
    } else {
        if (al::isNearZero(collNormal, 0.1f) && rs::isCollidedCeiling(mPlayerColliderHackCap)) {
            const sead::Vector3f& ceilingNormal =
                rs::getCollidedCeilingNormal(mPlayerColliderHackCap);
            if (!al::isFloorPolygon(ceilingNormal, -crossDir)) {
                collNormal = ceilingNormal;
                collPos = rs::getCollidedCeilingPos(mPlayerColliderHackCap);
            }
        }
    }
    if (al::isNearZero(collNormal, 0.1f))
        return false;
    *al::getTransPtr(this) -= collNormal;
    sead::Vector3f vertCross;
    al::verticalizeVec(&vertCross, crossDir, collNormal);
    if (!al::tryNormalizeOrZero(&vertCross)) {
        al::setNerve(this, &NrvHackCap.Return);
        return true;
    }
    if (sead::Mathf::abs(vertCross.dot(_240)) >= sead::Mathf::sin(1.0471975f)) {
        al::setNerve(this, &NrvHackCap.Return);
        return true;
    }
    sead::Vector3f vertVel;
    al::verticalizeVec(&vertVel, _240, collNormal);
    if (!al::tryNormalizeOrZero(&vertVel)) {
        al::setNerve(this, &NrvHackCap.Return);
        return true;
    }
    sead::Vector3f alongVec;
    al::alongVectorNormalH(&alongVec, _24c, _240, vertVel);
    if (!al::tryNormalizeOrZero(&alongVec)) {
        al::setNerve(this, &NrvHackCap.Return);
        return true;
    }
    if (sead::Mathf::abs(_240.dot(collNormal)) > 0.08715574f) {
        al::startHitReactionHitEffect(this, "コリジョン衝突方向転換", collPos);
        al::startSeWithParam(this, sead::SafeString("HitCol"),
                             sead::Mathf::abs(_240.dot(collNormal)), "");
    }
    __builtin_memcpy(&_240, &vertVel, sizeof(sead::Vector3f));
    __builtin_memcpy(&_24c, &alongVec, sizeof(sead::Vector3f));
    sead::Quatf quat = sead::Quatf::unit;
    sead::Vector3f negVel = -_240;
    al::makeQuatFrontUp(&quat, negVel, _24c);
    al::updatePoseQuat(this, quat);
    f32 speed = al::calcSpeed(this);
    al::setVelocity(this, vertVel * speed);
    return false;
}

bool HackCap::tryCollideWallReaction() {
    if (tryCollideWallLockOn())
        return true;
    bool isAttackCollide = false;
    if (trySendAttackCollideAndReaction(&isAttackCollide))
        return isAttackCollide;
    if (!rs::isCollidedWall(mPlayerColliderHackCap))
        return false;
    sead::Vector3f wallNormal;
    sead::Vector3f wallPos;
    __builtin_memcpy(&wallNormal, &rs::getCollidedWallNormal(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    __builtin_memcpy(&wallPos, &rs::getCollidedWallPos(mPlayerColliderHackCap),
                     sizeof(sead::Vector3f));
    al::verticalizeVec(&wallNormal, al::getGravity(this), wallNormal);
    if (!al::tryNormalizeOrZero(&wallNormal))
        return false;
    if (rs::sendMsgCapTouchWall(rs::tryGetCollidedWallSensor(mPlayerColliderHackCap), mAttackSensor,
                                wallPos, wallNormal)) {
        if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
            calcThrowReflect(_240, _24c, this, wallNormal);
        al::setNerve(this, &NrvHackCap.Rebound);
        return true;
    }
    if (!rs::isCollidedWallFace(mPlayerColliderHackCap))
        return false;
    bool isEnableEffect = true;
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        if (!al::isNearZero(_514, 0.001f) && _514.dot(wallNormal) > 0.9961947f)
            isEnableEffect = false;
    }
    if (tryReflectThrow(this, _240, _24c, wallNormal, wallPos, isEnableEffect)) {
        _560[0] = (void*)rs::getCollidedWallCollisionParts(mPlayerColliderHackCap);
        __builtin_memcpy((char*)_560 + 8, &wallPos, sizeof(sead::Vector3f));
        __builtin_memcpy((char*)_560 + 20, &wallNormal, sizeof(sead::Vector3f));
        if (isEnableEffect)
            _514 = wallNormal;
        _270 = 0.0f;
        _274 = 0.0f;
        return false;
    }
    if (_560[0]) {
        *al::getTransPtr(this) -= *(sead::Vector3f*)((char*)_560 + 20);
        _560[0] = nullptr;
        _560[1] = nullptr;
        _560[2] = nullptr;
        _560[3] = nullptr;
    }
    if (_24c.dot(wallNormal) < 0.5f)
        return stayWallHit();
    if (wallNormal.dot(al::getGravity(this)) >= -0.08715574f)
        return stayWallHit();
    return false;
}

void HackCap::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEye(self)) {
        if (al::isSensorPlayerAll(other))
            return;
        if (al::isSensorEnemyAttack(other))
            return;
        if (al::getSensorHost(other) == this)
            return;
        if (!mPlayerEyeSensorHitHolder->isEnableRecord(other, self))
            return;
        mPlayerEyeSensorHitHolder->tryRecordHomingAttack(other, self);
        return;
    }
    if (al::isSensorPlayer(other) && al::isSensorName(self, "Push") &&
        al::isNerve(this, &NrvHackCap.ThrowStay) && _294 >= 1 &&
        rs::isPlayerCollidedGround(this)) {
        rs::sendMsgPushToPlayer(other, self);
        return;
    }
    if (!al::isSensorPlayerDecoration(self))
        return;
    if (_5e8)
        return;
    if (al::isSensorPlayer(other) && mStateThrowStay->isEnableSendHipDropMsg() &&
        mStateThrowStay->sendHipDropObjMsg(mHackCapTrigger, other, self))
        return;
    if (!al::isSensorEnemyBody(other) && !al::isSensorMapObj(other) &&
        !al::isSensorNpc(other) && !al::isSensorSimple(other) && !al::isSensorRide(other))
        return;
    const sead::Vector3f& gravity = al::getGravity(this);
    f32 radius = mPlayerColliderHackCap->getColliderRadius();
    if (!rs::sendMsgCapIgnoreCollisionCheck(other, self) &&
        al::calcStrikeArrowCollideWallAndCeilingBetweenAttackSensor(this, self, other, gravity,
                                                                     radius) != 0)
        return;
    if (rs::sendMsgCapItemGet(other, self))
        return;
    if (mStateThrowStay->isEnableSendHipDropMsg() &&
        mStateThrowStay->sendHipDropObjMsg(mHackCapTrigger, other, self))
        return;
    if (al::isSensorPlayer(other))
        return;
    if (!isThrow() && !al::isNerve(this, &NrvHackCap.Return))
        return;
    if (_1d0) {
        al::LiveActor* host = al::getSensorHost(other);
        if (al::getSensorHost(_1d0) == host)
            return;
        if (mCapTargetInfo1 && mCapTargetInfo1->mActor == host)
            return;
    }
    if (al::isNerve(this, &NrvHackCap.Return))
        goto checkHitCount;
    if (!_4d0 && rs::sendMsgCapAttackStayRolling(other, self)) {
        stayRollingOrReflect();
        return;
    }
checkHitCount:
    if (_290 > 0)
        return;
    if (_1b0 != 0) {
        al::LiveActor* host = al::getSensorHost(other);
        for (s32 i = 0; i < _1b0; i++) {
            s32 index = _1ac + i;
            if (index >= _1a8)
                index -= _1a8;
            if (static_cast<al::LiveActor**>(_1a0_arr)[index] == host)
                return;
        }
    }
    if (rs::sendMsgCapReflect(other, self)) {
        bool isSeparateThrow = false;
        if (!_299) {
            u32 flags = mPlayerSeparateCapFlag->_flagsWord;
            isSeparateThrow = (flags & 0xff0000) == 0 && (flags & 0xff) != 0;
        }
        if (isSeparateThrow || _4d0) {
            sead::Vector3f dir;
            al::calcVecBetweenSensors(&dir, other, self);
            al::verticalizeVec(&dir, _24c, dir);
            if (!al::tryNormalizeOrZero(&dir))
                dir = -_240;
            calcThrowReflect(_240, _24c, this, dir);
        }
        if (!isSeparateThrow) {
            al::LiveActor* host = al::getSensorHost(other);
            if (_1b0 < _1a8) {
                s32 index = _1ac + _1b0;
                if (index >= _1a8)
                    index -= _1a8;
                _1b0 = _1b0 + 1;
                static_cast<al::LiveActor**>(_1a0_arr)[index] = host;
            } else {
                s32 newCount = _1b0 - 1;
                if (newCount >= 0) {
                    s32 nextHead = _1ac + 1;
                    _1ac = nextHead >= _1a8 ? 0 : nextHead;
                    _1b0 = newCount;
                }
                if (_1b0 < _1a8) {
                    s32 index = _1ac + _1b0;
                    if (index >= _1a8)
                        index -= _1a8;
                    _1b0 = _1b0 + 1;
                    static_cast<al::LiveActor**>(_1a0_arr)[index] = host;
                }
            }
        }
        al::setNerve(this, &NrvHackCap.Return);
        return;
    }
    if (al::isSensorName(self, "Attack") && rs::sendMsgCapStartLockOn(other, self)) {
        prepareLockOn(other);
        return;
    }
    if (!rs::sendMsgCapAttack(other, self))
        return;
    _2a0[1] = false;
    if (mStateThrowStay->isEnableSendHipDropMsg())
        return;
    if (al::isNerve(this, &NrvHackCap.Return)) {
        al::LiveActor* host = al::getSensorHost(other);
        if (_1b0 < _1a8) {
            s32 index = _1ac + _1b0;
            if (index >= _1a8)
                index -= _1a8;
            _1b0 = _1b0 + 1;
            static_cast<al::LiveActor**>(_1a0_arr)[index] = host;
        } else {
            s32 newCount = _1b0 - 1;
            if (newCount >= 0) {
                s32 nextHead = _1ac + 1;
                _1ac = nextHead >= _1a8 ? 1 : _1ac;
                _1b0 = newCount;
            }
            if (_1b0 < _1a8) {
                s32 index = _1ac + _1b0;
                if (index >= _1a8)
                    index -= _1a8;
                _1b0 = _1b0 + 1;
                static_cast<al::LiveActor**>(_1a0_arr)[index] = host;
            }
        }
        al::setNerve(this, &NrvHackCap.Rebound);
        return;
    }
    if (_4d0) {
        sead::Vector3f dir;
        al::calcVecBetweenSensors(&dir, other, self);
        al::verticalizeVec(&dir, _24c, dir);
        if (!al::tryNormalizeOrZero(&dir))
            dir = -_240;
        calcThrowReflect(_240, _24c, this, dir);
    } else if (!_299) {
        u32 flags = mPlayerSeparateCapFlag->_flagsWord;
        if ((flags & 0xff0000) == 0 && (flags & 0xff) != 0) {
            sead::Vector3f dir;
            al::calcVecBetweenSensors(&dir, other, self);
            al::verticalizeVec(&dir, _24c, dir);
            if (!al::tryNormalizeOrZero(&dir))
                dir = -_240;
            calcThrowReflect(_240, _24c, this, dir);
        }
    }
    al::setNerve(this, &NrvHackCap.Return);
}

bool HackCap::attackSpin(al::HitSensor* self, al::HitSensor* other, f32 arg) {
    if (!al::isSensorEnemyBody(other) && !al::isSensorMapObj(other) && !al::isSensorNpc(other) &&
        !al::isSensorSimple(other) && !al::isSensorRide(other))
        return false;
    if (al::isNerve(this, &NrvHackCap.Hide) || al::isNerve(this, &NrvHackCap.Hack) ||
        al::isNerve(this, &NrvHackCap.Rescue))
        return false;
    if (!_2a0[0])
        return false;
    if (_1d0) {
        const al::HitSensor* targetSensor = _1d0;
        const CapTargetInfo* targetInfo = mCapTargetInfo1;
        al::LiveActor* host = al::getSensorHost(other);
        if (al::getSensorHost(targetSensor) == host)
            return false;
        if (targetInfo && targetInfo->mActor == host)
            return false;
    }
    const sead::Vector3f& gravity = al::getGravity(this);
    if (!rs::sendMsgCapIgnoreCollisionCheck(other, self) &&
        al::calcStrikeArrowCollideWallAndCeilingBetweenAttackSensor(this, self, other, gravity,
                                                                    arg))
        return false;
    if (_1b8_current != 0) {
        al::LiveActor* host = al::getSensorHost(other);
        for (s32 i = 0; i < _1b8_current; i++) {
            s32 index = _1c4 + i;
            if (index >= _1c0)
                index -= _1c0;
            if (static_cast<al::LiveActor**>(_1b8_arr)[index] == host)
                return false;
        }
    }
    bool isLockOn = al::isNerve(this, &NrvHackCap.LockOn);
    if (!isLockOn && _2a0[1] && !isThrow() && !al::isNerve(this, &NrvHackCap.Return) &&
        !al::isNerve(this, &NrvHackCap.Catch)) {
        const al::LiveActor* playerActor = mPlayerActor;
        PlayerEyeSensorHitHolder* eyeHolder = mPlayerEyeSensorHitHolder;
        if (eyeHolder->_40 || eyeHolder->_38) {
            if (isTargetBehindPlayer(playerActor, self, other))
                goto spinAttack;
        }
        if (rs::sendMsgCapStartLockOn(other, mAttackSensor)) {
            prepareLockOn(other);
            _2a0[1] = false;
            return true;
        }
    }
spinAttack:
    if (al::sendMsgPlayerSpinAttack(other, mAttackSensor, nullptr) ||
        rs::sendMsgCapReflect(other, mAttackSensor)) {
        if (isLockOn)
            return true;
    } else {
        bool isAttack = rs::sendMsgCapAttack(other, mAttackSensor);
        if (!isLockOn && !isAttack)
            return false;
    }
    if (al::isNerve(this, &NrvHackCap.Return) || al::isNerve(this, &NrvHackCap.Rebound) ||
        al::isNerve(this, &NrvHackCap.Catch))
        return true;
    al::LiveActor* host = al::getSensorHost(other);
    if (_1b8_current < _1c0) {
        s32 index = _1c4 + _1b8_current;
        if (index >= _1c0)
            index -= _1c0;
        _1b8_current = _1b8_current + 1;
        static_cast<al::LiveActor**>(_1b8_arr)[index] = host;
    } else {
        s32 newCount = _1b8_current - 1;
        if (newCount >= 0) {
            s32 nextHead = _1c4 + 1;
            _1c4 = nextHead >= _1c0 ? 1 : _1c4;
            _1b8_current = newCount;
        }
        if (_1b8_current < _1c0) {
            s32 index = _1c4 + _1b8_current;
            if (index >= _1c0)
                index -= _1c0;
            _1b8_current = _1b8_current + 1;
            static_cast<al::LiveActor**>(_1b8_arr)[index] = host;
        }
    }
    if (isTargetBehindPlayer(mPlayerActor, self, other))
        return true;
    _2a0[1] = false;
    return true;
}

void HackCap::startThrowCapEyeThrowAction() {
    if (_298)
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
    else
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
}

void HackCap::startPuppetCheckpointWarp() {
    _5f9 = true;
    al::hideModelIfShow(this);
    al::hideModelIfShow(mEquipmentHat);
    al::hideModelIfShow(mEquipmentHatDepthShadow);
    al::tryKillEmitterAndParticleAll(this);
}

void HackCap::requestForceFollowSeparateHide() {
    if (al::isNerve(this, &NrvHackCap.Hide))
        mStateHide->mIsForceFollowSeparateHide = true;
}

void HackCap::recordHack() {
    if (mCapTargetInfo1->mIsSetHackNameToCamera) {
        const HackObjInfo* hackObjInfo = CapFunction::getHackObjInfo(this);
        PlayerCameraFunction::setCameraHackName(this, hackObjInfo);
    }
}

bool HackCap::isSeparateHide() const {
    if (al::isNerve(this, &NrvHackCap.Hide))
        return mStateHide->isSeparateMode();
    return false;
}

void HackCap::startHack() {
    mIsHide = false;
    mIsHackDamageVisible = false;
    _5ba = false;
    _5bb = true;
    _5bc = 0;
    al::invalidateHitSensor(this, "Trample");
    al::setNerve(this, &NrvHackCap.Hack);
}

void HackCap::endHack() {
    if (mCapTargetInfo1->mIsSetHackNameToCamera)
        PlayerCameraFunction::resetCameraHackName(this);
    hide(true);
    al::tryKillEmitterAndParticleAll(this);
    _2a0[2] = true;
    _2a0[3] = true;
}

bool HackCap::isLockOnEnableHackTarget() const {
    if (!al::isNerve(this, &NrvHackCap.LockOn))
        return false;
    if (!mCapTargetInfo1)
        return false;
    return !mCapTargetInfo1->mIsLockOnOnly;
}

void HackCap::prepareTransferLockOn(al::HitSensor*) {
    if (mIsSeparateFlying) {
        al::setNerve(this, &NrvHackCap.LockOn);
        rs::sendMsgTransferHack(mPlayerBodySensor, const_cast<al::HitSensor*>(_1d0));
        mIsSeparateFlying = false;
        mPlayerBodySensor = nullptr;
    }
}

f32 HackCap::calcSeparateHideSpeedH(const sead::Vector3f& dir) const {
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        return 0.0f;
    if (!al::isNerve(this, &NrvHackCap.Hide))
        return 0.0f;
    return al::calcSpeedExceptDir(this, dir);
}

void HackCap::noticeHackMarioEnter() {
    _5bb = false;
    al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
    if (mCapTargetInfo1->mIsInvalidCapEye) {
        if (al::isAlive(lockOnCapEyes))
            lockOnCapEyes->kill();
    }
    else if (al::isDead(lockOnCapEyes)) {
        lockOnCapEyes->appear();
    }
    al::startAction(mLockOnCapEyes, "Appear");
}

void HackCap::startRescuePlayer() {
    if (al::isNerve(this, &NrvHackCap.Return))
        al::startHitReaction(this, "プレイヤー救出消滅");
    hide(true);
    al::hideModelIfShow(this);
    al::offCollide(this);
    al::tryStartActionIfNotPlaying(this, "WaitSeparate");
    al::setNerve(this, &NrvHackCap.Rescue);
}

bool HackCap::isEnableThrowSeparate() const {
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        return false;
    return al::isNerve(this, &NrvHackCap.Hide) || al::isNerve(this, &NrvHackCap.Catch);
}

void HackCap::setupStartLockOn() {
    al::setVelocityZero(this);
    al::offCollide(this);
    al::invalidateHitSensors(this);
    al::validateHitSensor(this, "Trample");
    _29e = false;
    _274 = 0;
    _264 = 0.0f;
    _268.x = 0.0f;
    _268.y = 0.0f;
    _270 = 0.0f;
    if (_298)
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
    else
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
}

bool HackCap::isWaitHackLockOn() const {
    if (!mCapTargetInfo1)
        return false;
    bool isRequestable = _2a5[1];
    if (!al::isNerve(this, &NrvHackCap.LockOn))
        return false;
    bool isNotLessStep = !al::isLessEqualStep(this, 5);
    bool isEnable = isRequestable && isNotLessStep;
    if (!isEnable)
        return false;
    return !mCapTargetInfo1->mIsLockOnOnly;
}

void HackCap::movement() {
    mCapEyeSensorHitHolder->clear();
    if (rs::isCollidedGround(mPlayerColliderHackCap)) {
        al::setMaterialCode(this, rs::getMaterialCodeGround(mPlayerColliderHackCap));
        al::updateMaterialCodePuddle(this);
    }
    al::LiveActor::movement();
    mHackCapTrigger->clearAfterMovemetTrigger();
    if (al::isNerve(this, &NrvHackCap.Hide))
        mCapExternalVelocity->reset();
    else
        mCapExternalVelocity->update();
    updateShadowMaskOffset();
    mSelfPlayerWetControl->update();
}

void HackCap::updateCollider() {
    sead::Vector3f velocity(0.0f, 0.0f, 0.0f);
    mCapPushReceiver->calcPushedVelocity(
        &velocity, al::getVelocity(this) + mCapExternalVelocity->mTotalVelocity);
    rs::updateCollider(this, mPlayerColliderHackCap, velocity);
    mCapPushReceiver->clear();
    updateFrameOutLayout();
    if (!_5e8 && mStateThrowStay->isEnableSendHipDropMsg())
        mStateThrowStay->sendHipDropCollideMsg(mAttackSensor);
    if (isThrow() || al::isNerve(this, &NrvHackCap.Return) || al::isNerve(this, &NrvHackCap.Rebound))
        mHackCapAboveGroundChecker->update(-al::getGravity(this));
}

void HackCap::recordCapJump(PlayerWallActionHistory*) {}

void HackCap::endHackShineGetDemo() {}

void HackCap::showPuppetCapSilhouette() {
    mIsHidePuppetCapSilhouette = false;
}

void HackCap::hidePuppetCapSilhouette() {
    mIsHidePuppetCapSilhouette = true;
}

void HackCap::syncHackDamageVisibility(bool isVisible) {
    mIsHackDamageVisible = isVisible;
}

bool HackCap::isEnableRescuePlayer() const {
    return false;
}

void HackCap::endHackThrow() {
    mIsSeparateFlying = false;
    mPlayerBodySensor = nullptr;
}

bool HackCap::requestReturn(bool* isRequest) {
    return tryReturn(false, isRequest);
}

bool HackCap::tryReturn(bool isHide, bool* isRequest) {
    if (isRequest)
        *isRequest = false;
    if (mIsSeparateFlying) {
        if (isRequest)
            *isRequest = true;
        return false;
    }
    if (al::isNerve(this, &NrvHackCap.ThrowStay)) {
        al::startHitReaction(this, "帽子戻り");
    } else {
        bool isTarget78 = mCapTargetInfo1->_78;
        if (!mIsSeparateFlying && !al::isNerve(this, &NrvHackCap.Hack)) {
            if (!rs::sendMsgCapIgnoreCancelLockOn(const_cast<al::HitSensor*>(_1d0),
                                                  mAttackSensor)) {
                rs::sendMsgCapCancelLockOn(const_cast<al::HitSensor*>(_1d0), mAttackSensor);
            } else if (!isHide) {
                if (isRequest)
                    *isRequest = !rs::sendMsgCapIgnoreCancelMissReaction(
                        const_cast<al::HitSensor*>(_1d0), mAttackSensor);
                return false;
            } else {
                rs::sendMsgCapCancelLockOn(const_cast<al::HitSensor*>(_1d0), mAttackSensor);
            }
        }
        if (_1d0) {
            al::LiveActor* host = al::getSensorHost(_1d0);
            if (_1b0 < _1a8) {
                s32 index = _1ac + _1b0;
                if (index >= _1a8)
                    index -= _1a8;
                _1b0 = _1b0 + 1;
                static_cast<al::LiveActor**>(_1a0_arr)[index] = host;
            } else {
                s32 newCount = _1b0 - 1;
                if (newCount >= 0) {
                    s32 nextHead = _1ac + 1;
                    _1ac = nextHead >= _1a8 ? 0 : nextHead;
                    _1b0 = newCount;
                }
                if (_1b0 < _1a8) {
                    s32 index = _1ac + _1b0;
                    if (index >= _1a8)
                        index -= _1a8;
                    _1b0 = _1b0 + 1;
                    static_cast<al::LiveActor**>(_1a0_arr)[index] = host;
                }
            }
        }
        if (isTarget78) {
            _290 = 0x708;
            al::offCollide(this);
            al::invalidateHitSensors(this);
            al::validateHitSensor(this, "Catch");
        } else {
            _290 = 3;
            al::offCollide(this);
            al::validateHitSensors(this);
            al::invalidateHitSensor(this, "SpiralTail1");
            al::invalidateHitSensor(this, "SpiralTail2");
            al::invalidateHitSensor(this, "SpiralTail3");
            al::invalidateHitSensor(this, "SpiralTail4");
            al::invalidateHitSensor(this, "SpiralTail5");
        }
        mCapTargetInfo1 = nullptr;
        _1d0 = nullptr;
        al::startHitReaction(this, "帽子戻り");
        if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
            _510 = false;
        }
    }
    al::setNerve(this, &NrvHackCap.Return);
    return true;
}

void HackCap::startHackShineGetDemo() {
    if (mIsSeparateFlying)
        endHackThrowAndReturnHack();
}

void HackCap::followTarget() {
    if (mLockOnCounter == 4) {
        sead::Matrix34f lockOnMtx;
        mCapTargetInfo1->makeLockOnMtx(&lockOnMtx);
        al::updatePoseMtx(this, &lockOnMtx);
        sead::Vector3f scale(1.0f, 1.0f, 1.0f);
        mCapTargetInfo1->calcLockOnFollowTargetScale(&scale);
        al::setScale(this, scale);
    } else {
        mLockOnCounter = al::converge(mLockOnCounter, 4, 1);
        f32 rate = mLockOnCounter * 0.25f;
        sead::Matrix34f lockOnMtx;
        mCapTargetInfo1->makeLockOnMtx(&lockOnMtx);
        sead::Matrix34f blendMtx = sead::Matrix34f::ident;
        al::blendMtx(&blendMtx, _588, lockOnMtx, rate);
        al::updatePoseTrans(
            this, sead::Vector3f(blendMtx.m[0][3], blendMtx.m[1][3], blendMtx.m[2][3]));
        sead::Quatf quat = sead::Quatf::unit;
        sead::Matrix34CalcCommon<f32>::toQuat(quat, _588);
        al::turnQuatYDirRate(
            &quat, quat,
            sead::Vector3f(blendMtx.m[0][1], blendMtx.m[1][1], blendMtx.m[2][1]), 1.0f);
        al::updatePoseQuat(this, quat);
    }
}

// Syncs the equipment hat and the lock-on cap eyes to Cappy's current
// pose and visibility state: while locked on, hacking, or trample-locked
// on, the hat visibility follows the hide/damage flags and the eyes are
// killed or appeared from the target info; otherwise the model alpha is
// synced (Hide nerve) or taken from the dither animator. The equipment
// hat then copies Cappy's scale and pose. (Original at 0x3fd52c in
// HackCap.o; the eye kill/appear pattern is the same as in
// noticeHackDemoPuppetableEnd.)
void HackCap::updateCapPose() {
    mActorDitherAnimator->update();
    if (al::isNerve(this, &NrvHackCap.LockOn) || al::isNerve(this, &NrvHackCap.Hack) ||
        al::isNerve(this, &NrvHackCap.TrampleLockOn)) {
        if (!_5f9) {
            if (al::isNerve(this, &NrvHackCap.Hack)) {
                bool isHide = mIsHide || mIsHackDamageVisible;
                updateEquipmentHatVisibility(isHide, mEquipmentHat, mEquipmentHatDepthShadow,
                                             mCapTargetInfo1, true);
                if (isHide || _5ba || _5bb || mCapTargetInfo1->mIsInvalidCapEye) {
                    if (al::isAlive(mLockOnCapEyes))
                        mLockOnCapEyes->kill();
                } else if (al::isDead(mLockOnCapEyes)) {
                    mLockOnCapEyes->appear();
                }
            } else if (al::isNerve(this, &NrvHackCap.LockOn)) {
                bool isHide = mIsSeparateFlying || mCapTargetInfo1->_72;
                updateEquipmentHatVisibility(isHide, mEquipmentHat, mEquipmentHatDepthShadow,
                                             mCapTargetInfo1, false);
                if (isHide || !mCapTargetInfo1->mIsLockOnOnly ||
                    mCapTargetInfo1->mIsInvalidCapEye) {
                    if (al::isAlive(mLockOnCapEyes))
                        mLockOnCapEyes->kill();
                } else if (al::isDead(mLockOnCapEyes)) {
                    mLockOnCapEyes->appear();
                }
            } else {
                updateEquipmentHatVisibility(mIsSeparateFlying, mEquipmentHat,
                                             mEquipmentHatDepthShadow, mCapTargetInfo1, false);
                if (mIsSeparateFlying || mCapTargetInfo1->mIsInvalidCapEye) {
                    if (al::isAlive(mLockOnCapEyes))
                        mLockOnCapEyes->kill();
                } else if (al::isDead(mLockOnCapEyes)) {
                    mLockOnCapEyes->appear();
                }
            }
        }
        followTarget();
        HackCapFunction::resetPositionAndCollision(this, mPlayerColliderHackCap);
        al::setScale(mEquipmentHat, al::getScale(this));
        al::copyPose(mEquipmentHat, this);
        al::resetPosition(mEquipmentHat);
        al::resetPosition(mEquipmentHatDepthShadow);
        al::resetPosition(mLockOnCapEyes);
        if (mCapTargetInfo1->mIsExistModel) {
            f32 alpha = al::getModelAlphaMask(mCapTargetInfo1->mActor);
            al::setModelAlphaMask(this, alpha);
            al::setModelAlphaMask(mEquipmentHat, alpha);
        }
    } else if (al::isNerve(this, &NrvHackCap.Hide)) {
        rs::syncPlayerModelAlpha(this);
        rs::syncPlayerModelAlpha(mEquipmentHat);
    } else {
        f32 alpha = mActorDitherAnimator->getDitherAlpha();
        al::setModelAlphaMask(this, alpha);
        al::setModelAlphaMask(mEquipmentHat, alpha);
    }
    if (mIsPuppet)
        syncPuppetSilhouette();
}

void HackCap::endHackThrowAndReturnHack() {
    mIsSeparateFlying = false;
    mPlayerBodySensor = nullptr;
    HackCapJointControlKeeper* jointControlKeeper = mHackCapJointControlKeeper;
    jointControlKeeper->mRotateX = 0.0f;
    jointControlKeeper->mRotateY = 0.0f;
    jointControlKeeper->mRotateZ = 0.0f;
    jointControlKeeper->_c = 0;
    al::startAction(this, "Capture");
    al::LiveActor* equipmentHat = mEquipmentHat;
    al::LiveActor* equipmentHatDepthShadow = mEquipmentHatDepthShadow;
    al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
    al::startAction(equipmentHat, "Capture");
    al::startAction(equipmentHatDepthShadow, "Capture");
    if (al::isAlive(lockOnCapEyes) && !al::tryStartAction(lockOnCapEyes, "Capture"))
        al::tryStartActionIfNotPlaying(lockOnCapEyes, "Wait");
    al::setActionFrame(this, al::getActionFrameMax(this, "Capture"));
    al::setVelocityZero(this);
    followTarget();
    HackCapFunction::resetPositionAndCollision(this, mPlayerColliderHackCap);
    updateEquipmentHatVisibility(mIsHide, mEquipmentHat, mEquipmentHatDepthShadow, mCapTargetInfo1,
                                 false);
    {
        al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
        if (al::isAlive(lockOnCapEyes))
            lockOnCapEyes->kill();
    }
    al::hideModelIfShow(this);
    al::invalidateHitSensors(this);
    al::setNerve(this, &NrvHackCap.Hack);
}

void HackCap::endHackThrowAndReturnHackOrHide() {
    if (mIsSeparateFlying)
        endHackThrowAndReturnHack();
    else
        hide(false);
}

void HackCap::exeHide() {
    al::updateNerveState(this);
}

void HackCap::makeFollowMtx(sead::Matrix34f* mtx) const {
    mCapTargetInfo1->makeLockOnMtx(mtx);
}

s32 HackCap::getPadRumblePort() const {
    return mPadRumbleKeeper->getPort();
}

f32 HackCap::getThrowHeight() const {
    return mHackCapThrowParam->mHackThrowHeight->value;
}

void HackCap::startPuppet() {
    mIsPuppet = true;
    _5f9 = false;
    mIsHidePuppetCapSilhouette = false;
}

void HackCap::endPuppet() {
    mIsPuppet = false;
    _5f9 = false;
    mIsHidePuppetCapSilhouette = false;
    syncPuppetSilhouette();
}

void HackCap::hidePuppetCap() {
    _5f9 = true;
    if (al::isAlive(mEquipmentHat)) {
        al::hideModelIfShow(mEquipmentHat);
        return;
    }
    al::hideModelIfShow(this);
}

void HackCap::showPuppetCap() {
    _5f9 = true;
    if (al::isAlive(mEquipmentHat)) {
        al::showModelIfHide(mEquipmentHat);
        return;
    }
    al::showModelIfHide(this);
}

void HackCap::exeRescue() {
    if (!rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvHackCap.Hide);
}

void HackCap::endThrowSpiral() {
    al::invalidateHitSensor(this, "SpiralTail1");
    al::invalidateHitSensor(this, "SpiralTail2");
    al::invalidateHitSensor(this, "SpiralTail3");
    al::invalidateHitSensor(this, "SpiralTail4");
    al::invalidateHitSensor(this, "SpiralTail5");
}

bool HackCap::isEnableThrow() const {
    return al::isNerve(this, &NrvHackCap.SpinAttack);
}

bool HackCap::isSpinAttack() const {
    return al::isNerve(this, &NrvHackCap.SpinAttack);
}

bool HackCap::isRescuePlayer() const {
    return al::isNerve(this, &NrvHackCap.Rescue);
}

bool HackCap::isHide() const {
    return al::isNerve(this, &NrvHackCap.Hide);
}

bool HackCap::isPutOn() const {
    if (al::isNerve(this, &NrvHackCap.Hide))
        return _2a0[2] != 0;
    return false;
}

bool HackCap::isCatched() const {
    if (al::isNerve(this, &NrvHackCap.Catch) || al::isNerve(this, &NrvHackCap.Hide))
        return _2a0[3] != 0;
    return false;
}

bool HackCap::isNoPutOnHide() const {
    if (al::isNerve(this, &NrvHackCap.Hide))
        return _2a0[2] == 0;
    return false;
}

bool HackCap::isThrow() const {
    if (al::isNerve(this, &NrvHackCap.Throw))
        return true;
    if (al::isNerve(this, &NrvHackCap.Rethrow))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowBrake))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowRolling))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowRollingBrake))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowAppend))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowStay))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowSpiral))
        return true;
    if (al::isNerve(this, &NrvHackCap.ThrowTornado))
        return true;
    return al::isNerve(this, &NrvHackCap.Blow);
}

f32 HackCap::getFlyingSpeedMax() const {
    if (isThrow() || al::isNerve(this, &NrvHackCap.Return) ||
        al::isNerve(this, &NrvHackCap.Rebound)) {
        if (_29f)
            return 60.0f;
        if (al::isNearZero(_268.x, 0.001f)) {
            if (!al::isNearZero(_268.y, 0.001f))
                return mHackCapThrowParam->mRollSpeed->value;
        }
        if (_29a)
            return mHackCapThrowParam->mWaterMaxSpeed->value;
        if (_29e || _4d0)
            return mHackCapThrowParam->mContinuousThrowSpeed->value;
        return mHackCapThrowParam->mMaxVel->value;
    }
    return mHackCapThrowParam->mMaxVel->value;
}

f32 HackCap::getThrowSpeed() const {
    if (_29f)
        return 60.0f;
    if (al::isNearZero(_268.x, 0.001f)) {
        if (!al::isNearZero(_268.y, 0.001f))
            return mHackCapThrowParam->mRollSpeed->value;
    }
    if (_29a)
        return mHackCapThrowParam->mWaterMaxSpeed->value;
    if (_29e || _4d0)
        return mHackCapThrowParam->mContinuousThrowSpeed->value;
    return mHackCapThrowParam->mMaxVel->value;
}

f32 HackCap::getThrowSpeedAppend() const {
    if (_29f)
        return 60.0f;
    if (_29a || (!_29e && !_4d0))
        return mHackCapThrowParam->mMaxVel->value;
    return mHackCapThrowParam->mContinuousThrowSpeed->value;
}

f32 HackCap::getThrowBackSpeed() const {
    return (_29a ? mHackCapThrowParam->mWaterMaxRetSpeed : mHackCapThrowParam->mMaxRetSpeed)
        ->value;
}

s32 HackCap::getThrowStayTime() const {
    if (_29f)
        return 3;
    return mHackCapThrowParam->mEndpointStopTime->value;
}

s32 HackCap::getThrowStayTimeMax() const {
    if (_299)
        return mHackCapThrowParam->mMaxEndpointStopTime->value;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        return 120;
    return mHackCapThrowParam->mMaxEndpointStopTime->value;
}

s32 HackCap::getThrowBrakeTime() const {
    if (_29f)
        return 2;
    if (al::isNearZero(_268.x, 0.001f)) {
        if (!al::isNearZero(_268.y, 0.001f))
            return (_268.y > 0.0f ? mHackCapThrowParam->mRollBrakeTimeTop
                                  : mHackCapThrowParam->mRollBrakeTimeBottom)
                ->value;
    }
    return (_29a ? mHackCapThrowParam->mWaterBreakTime : mHackCapThrowParam->mBreakTime)->value;
}

__attribute__((always_inline)) f32 HackCap::getThrowRange() const {
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        return sThrowRange[_29a];
    if (_29f)
        return 1200.0f;
    if (_4d0)
        return mHackCapThrowParam->mTornadoDist->value;
    if (al::isNearZero(_268.x, 0.001f)) {
        if (!al::isNearZero(_268.y, 0.001f))
            return (_268.y > 0.0f ? mHackCapThrowParam->mRollDistTop
                                  : mHackCapThrowParam->mRollDistBottom)
                ->value;
    }
    return (_29a ? mHackCapThrowParam->mWaterDist : mHackCapThrowParam->mMaxDist)->value;
}

f32 HackCap::getThrowRangeAppend() const {
    return sThrowRangeAppend[_2a5[0]];
}

bool HackCap::isHoldInputKeepLockOn() const {
    if (_299)
        return mInput->isHoldAction();
    return mInput->isHoldCapAction();
}

bool HackCap::isHoldSpinCapStay() const {
    if (_299)
        return mInput->isHoldAction();
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        return _510;
    return mInput->isHoldSpinCap();
}

bool HackCap::isThrowTypeSpiral() const {
    return !al::isNearZero(_268.x, 0.001f);
}

bool HackCap::isThrowTypeRolling() const {
    return al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f);
}

bool HackCap::isHackInvalidSeparatePlay() const {
    return mCapTargetInfo1 && mCapTargetInfo1->_7e;
}

bool HackCap::isSeparateThrowFlying() const {
    if (isThrow() || al::isNerve(this, &NrvHackCap.Return) ||
        al::isNerve(this, &NrvHackCap.Rebound)) {
        if (_299)
            return false;
        return (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
               (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
    }
    return false;
}

void HackCap::exeSpinAttack() {
    updateSpinAttackPose(this, mPlayerActor, mPlayerJointControlKeeper);
}

void HackCap::exeHack() {
    if (al::isFirstStep(this)) {
        updateEquipmentHatVisibility(mIsHide, mEquipmentHat, mEquipmentHatDepthShadow, mCapTargetInfo1,
                                     false);
        al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
        bool isKillCapEyes = mIsHide || _5ba || mCapTargetInfo1->mIsInvalidCapEye;
        if (isKillCapEyes) {
            if (al::isAlive(lockOnCapEyes))
                lockOnCapEyes->kill();
        } else if (al::isDead(lockOnCapEyes)) {
            lockOnCapEyes->appear();
        }
        al::hideModelIfShow(this);
        _5bc = 0;
    }
    if (al::isAlive(mLockOnCapEyes)) {
        if (al::isActionPlaying(mLockOnCapEyes, "Disappear")) {
            if (al::isActionEnd(mLockOnCapEyes)) {
                _5ba = true;
                al::startAction(mLockOnCapEyes, "Wait");
            }
        } else if (al::isActionPlaying(mLockOnCapEyes, "Appear")) {
            if (al::isActionEnd(mLockOnCapEyes))
                al::startAction(mLockOnCapEyes, "Wait");
        }
    }
}

void HackCap::addHackStartDemo() {
    rs::addDemoActor(this, true);
    rs::addDemoActor(mEquipmentHat, true);
}

void HackCap::addLockOnKeepDemo() {
    rs::addDemoActor(this, true);
    rs::addDemoActor(mEquipmentHat, true);
}

void HackCap::prepareCooperateThrow() {
    hide(true);
    al::hideModelIfShow(this);
    al::offCollide(this);
    al::tryStartActionIfNotPlaying(this, "WaitSeparate");
}

void HackCap::endMove() {
    al::offCollide(this);
    al::invalidateShadow(this);
    al::invalidateHitSensors(this);
    mCapTargetInfo1 = nullptr;
    _1d0 = nullptr;
    _2a8 = false;
    al::setVelocityZero(this);
}

void HackCap::resetLockOnParam() {
    mCapExternalVelocity->reset();
    mCapTargetInfo1 = nullptr;
    _580 = 0;
    mLockOnCounter = 0;
    _2a5[1] = false;
    al::makeMtxRT(&_588, this);
}

bool HackCap::requestLockOnHitReaction(const CapTargetInfo* capTargetInfo,
                                       const char* hitReactionName) {
    if (mCapTargetInfo1 && mCapTargetInfo1 == capTargetInfo) {
        al::startHitReaction(mEquipmentHat, hitReactionName);
        return true;
    }
    return false;
}

bool HackCap::isEnablePreInput() const {
    if (_2a8)
        return false;
    if (_299)
        return true;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        return true;
    if (isThrow() || al::isNerve(this, &NrvHackCap.Return) ||
        al::isNerve(this, &NrvHackCap.Rebound))
        return al::isNerve(this, &NrvHackCap.Return);
    return true;
}

bool HackCap::isForceCapTouchJump() const {
    if (_299)
        return false;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
        return mStateThrowStay->isHomingPlayerJump();
    return false;
}

void HackCap::setupThrowStart() {
    _290 = 0;
    _294 = 0;
    al::validateHitSensors(this);
    al::invalidateHitSensor(this, "SpiralTail1");
    al::invalidateHitSensor(this, "SpiralTail2");
    al::invalidateHitSensor(this, "SpiralTail3");
    al::invalidateHitSensor(this, "SpiralTail4");
    al::invalidateHitSensor(this, "SpiralTail5");
    al::validateShadow(this);
    al::onCollide(this);
    const char* actionNameL = _29e ? "FlyingWaitCapCatchL" : "FlyingWaitL";
    const char* actionNameR = _29e ? "FlyingWaitCapCatchR" : "FlyingWaitR";
    al::startAction(this, _264 > 0.0f ? actionNameL : actionNameR);
}

void HackCap::clearThrowType() {
    _29e = false;
    _274 = 0;
    _264 = 0.0f;
    _268.x = 0.0f;
    _268.y = 0.0f;
    _270 = 0.0f;
    if (_298)
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
    else
        al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
}

void HackCap::updateSeparateMode(const PlayerSeparateCapFlag* flag) {
    s32 port;
    if ((flag->_flagsWord & 0xff0000) == 0 && (flag->_flagsWord & 0xff) != 0)
        port = 1;
    else
        port = 0;
    mPadRumbleKeeper->setPort(al::getPlayerControllerPort(port));
    if (flag->_01 && !flag->_02)
        return;
    if (al::isNerve(this, &NrvHackCap.Hide) && mStateHide->isSeparateMode())
        _2a0[2] = false;
}

bool HackCap::changeThrowParamInWater(s32 step, bool isThrow) {
    if (_29a) {
        updateWaterArea();
        return false;
    }

    f32 speed;
    if (_29f) {
        speed = 60.0f;
    } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        speed = mHackCapThrowParam->mRollSpeed->value;
    } else if (_29a) {
        speed = mHackCapThrowParam->mWaterMaxSpeed->value;
    } else if (_29e || _4d0) {
        speed = mHackCapThrowParam->mContinuousThrowSpeed->value;
    } else {
        speed = mHackCapThrowParam->mMaxVel->value;
    }

    s32 breakTime;
    if (_29f) {
        breakTime = 2;
    } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        breakTime = _268.y > 0.0f ? mHackCapThrowParam->mRollBrakeTimeTop->value
                                  : mHackCapThrowParam->mRollBrakeTimeBottom->value;
    } else {
        breakTime = _29a ? mHackCapThrowParam->mWaterBreakTime->value
                         : mHackCapThrowParam->mBreakTime->value;
    }

    updateWaterArea();

    if (!_29a) {
        if (mWaterSurfaceFinder->isFoundSurface()) {
            f32 offset = sead::Mathf::max(mWaterSurfaceFinder->getDistance() + 50.0f, 0.0f) * 0.25f;
            sead::Vector3f newPos = al::getTrans(this) - al::getGravity(this) * offset;
            al::setTrans(this, newPos);
        }
        return false;
    }

    f32 dist = speed * (f32)(_27c + breakTime) + (f32)breakTime * (f32)breakTime * (_280 * 0.5f);
    if (isThrow) {
        dist = (f32)step * (f32)step * (_280 * 0.5f) + dist - speed * (f32)(_27c + step);
    } else {
        dist = dist - (f32)step * speed;
    }

    bool isSeparate = !_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                      (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0;
    if (isThrow) {
        if (isSeparate) {
            dist = sead::Mathf::min(dist, 550.0f);
        } else if (_29f) {
            dist = sead::Mathf::min(dist, 1200.0f);
        } else {
            f32 maxDist;
            if (_4d0) {
                maxDist = mHackCapThrowParam->mTornadoDist->value;
            } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
                maxDist = _268.y > 0.0f ? mHackCapThrowParam->mRollDistTop->value
                                        : mHackCapThrowParam->mRollDistBottom->value;
            } else {
                maxDist = _29a ? mHackCapThrowParam->mWaterDist->value
                               : mHackCapThrowParam->mMaxDist->value;
            }
            dist = sead::Mathf::min(dist, maxDist);
        }

        f32 maxSpeed;
        s32 newBreakTime;
        if (_29f) {
            maxSpeed = 60.0f;
            newBreakTime = 2;
        } else {
            if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
                maxSpeed = mHackCapThrowParam->mRollSpeed->value;
            } else if (_29a) {
                maxSpeed = mHackCapThrowParam->mWaterMaxSpeed->value;
            } else if (_29e || _4d0) {
                maxSpeed = mHackCapThrowParam->mContinuousThrowSpeed->value;
            } else {
                maxSpeed = mHackCapThrowParam->mMaxVel->value;
            }
            if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
                newBreakTime = _268.y > 0.0f ? mHackCapThrowParam->mRollBrakeTimeTop->value
                                             : mHackCapThrowParam->mRollBrakeTimeBottom->value;
            } else {
                newBreakTime = _29a ? mHackCapThrowParam->mWaterBreakTime->value
                                    : mHackCapThrowParam->mBreakTime->value;
            }
        }

        f32 rate = maxSpeed / (f32)newBreakTime;
        _280 = -rate;
        _27c = (s32)((dist - maxSpeed * (f32)newBreakTime * 0.5f) / maxSpeed);
        if (_27c - 1 < 0) {
            al::parallelizeVec(al::getVelocityPtr(this), _240, *al::getVelocityPtr(this));
            f32 limit;
            if (_29f) {
                limit = 60.0f;
            } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
                limit = mHackCapThrowParam->mRollSpeed->value;
            } else if (_29a) {
                limit = mHackCapThrowParam->mWaterMaxSpeed->value;
            } else if (_29e || _4d0) {
                limit = mHackCapThrowParam->mContinuousThrowSpeed->value;
            } else {
                limit = mHackCapThrowParam->mMaxVel->value;
            }
            al::limitVelocity(this, limit);
            return false;
        }
        _27c = _27c - 1;
        f32 limit;
        if (_29f) {
            limit = 60.0f;
        } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
            limit = mHackCapThrowParam->mRollSpeed->value;
        } else if (_29a) {
            limit = mHackCapThrowParam->mWaterMaxSpeed->value;
        } else if (_29e || _4d0) {
            limit = mHackCapThrowParam->mContinuousThrowSpeed->value;
        } else {
            limit = mHackCapThrowParam->mMaxVel->value;
        }
        al::limitVelocity(this, limit);
        return true;
    }

    if (isSeparate) {
        dist = sead::Mathf::min(dist, 550.0f);
    } else if (_29f) {
        dist = sead::Mathf::min(dist, 1200.0f);
    } else {
        f32 maxDist;
        if (_4d0) {
            maxDist = mHackCapThrowParam->mTornadoDist->value;
        } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
            maxDist = _268.y > 0.0f ? mHackCapThrowParam->mRollDistTop->value
                                    : mHackCapThrowParam->mRollDistBottom->value;
        } else {
            maxDist = _29a ? mHackCapThrowParam->mWaterDist->value
                           : mHackCapThrowParam->mMaxDist->value;
        }
        dist = sead::Mathf::min(dist, maxDist);
    }

    f32 maxSpeed;
    s32 newBreakTime;
    if (_29f) {
        maxSpeed = 60.0f;
        newBreakTime = 2;
    } else {
        if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
            maxSpeed = mHackCapThrowParam->mRollSpeed->value;
        } else if (_29a) {
            maxSpeed = mHackCapThrowParam->mWaterMaxSpeed->value;
        } else if (_29e || _4d0) {
            maxSpeed = mHackCapThrowParam->mContinuousThrowSpeed->value;
        } else {
            maxSpeed = mHackCapThrowParam->mMaxVel->value;
        }
        if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
            newBreakTime = _268.y > 0.0f ? mHackCapThrowParam->mRollBrakeTimeTop->value
                                         : mHackCapThrowParam->mRollBrakeTimeBottom->value;
        } else {
            newBreakTime = _29a ? mHackCapThrowParam->mWaterBreakTime->value
                                : mHackCapThrowParam->mBreakTime->value;
        }
    }

    f32 rate = maxSpeed / (f32)newBreakTime;
    _280 = -rate;
    _27c = (s32)((dist - maxSpeed * (f32)newBreakTime * 0.5f) / maxSpeed) + step;
    al::parallelizeVec(al::getVelocityPtr(this), _240, *al::getVelocityPtr(this));
    f32 limit;
    if (_29f) {
        limit = 60.0f;
    } else if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        limit = mHackCapThrowParam->mRollSpeed->value;
    } else if (_29a) {
        limit = mHackCapThrowParam->mWaterMaxSpeed->value;
    } else if (_29e || _4d0) {
        limit = mHackCapThrowParam->mContinuousThrowSpeed->value;
    } else {
        limit = mHackCapThrowParam->mMaxVel->value;
    }
    al::limitVelocity(this, limit);
    return false;
}

bool HackCap::tryChangeSeparateThrow() {
    if (!_299)
        return false;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        if (mInput->isNoInputSeparateCap())
            return false;
        _299 = false;
        _1b0 = 0;
        _1ac = 0;
        _510 = true;
        rs::resetJudge(mCapJudgePreInputSeparateThrow);
        return true;
    }
    return false;
}

bool HackCap::sendMsgStartHack(al::HitSensor* sensor) {
    if (!mCapTargetInfo1)
        return false;
    bool isRequestable = _2a5[1];
    if (!al::isNerve(this, &NrvHackCap.LockOn))
        return false;
    bool isNotLessStep = !al::isLessEqualStep(this, 5);
    bool isEnable = isRequestable && isNotLessStep;
    if (isEnable)
        return rs::sendMsgStartHack(const_cast<al::HitSensor*>(_1d0), sensor);
    return false;
}

void HackCap::updateModelAlphaForSnapShot() {
    if (al::isNerve(this, &NrvHackCap.Hide)) {
        rs::syncPlayerModelAlpha(this);
        rs::syncPlayerModelAlpha(mEquipmentHat);
    }
    else {
        mActorDitherAnimator->update();
        f32 alpha = mActorDitherAnimator->getDitherAlpha();
        al::setModelAlphaMask(this, alpha);
        al::setModelAlphaMask(mEquipmentHat, alpha);
    }
}

void HackCap::noticeHackDemoPuppetableEnd() {
    _5bb = false;
    al::LiveActor* lockOnCapEyes = mLockOnCapEyes;
    if (mCapTargetInfo1->mIsInvalidCapEye) {
        if (al::isAlive(lockOnCapEyes))
            lockOnCapEyes->kill();
    }
    else if (al::isDead(lockOnCapEyes)) {
        lockOnCapEyes->appear();
    }
    al::tryStartActionIfNotPlaying(mLockOnCapEyes, "Appear");
    al::setActionFrame(mLockOnCapEyes, al::getActionFrameMax(mLockOnCapEyes));
}

void HackCap::calcReturnTargetPos(sead::Vector3f* out) const {
    if (mIsSeparateFlying) {
        sead::Matrix34f mtx = sead::Matrix34f::ident;
        mCapTargetInfo1->makeLockOnMtx(&mtx);
        out->x = mtx.m[0][3];
        out->y = mtx.m[1][3];
        out->z = mtx.m[2][3];
    }
    else {
        *out = rs::getPlayerHeadPos(this);
    }
}

void HackCap::emitHackStartEffect() {
    al::startHitReaction(mEquipmentHat, "キャプチャー開始デモ演出エフェクト");
    f32 rate = al::calcRate01(mCapTargetInfo1->mLockOnScale, 1.3f, 1.5f);
    f32 scale = al::lerpValue(1.0f, 1.5f, rate);
    al::setEffectAllScale(mEquipmentHat, "MarioHackExpressionShot", sead::Vector3f(scale, scale, scale));
}

void HackCap::calcHackFollowTrans(sead::Vector3f* out, bool isUseLocalOffset) const {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    mCapTargetInfo1->makeLockOnMtx(&mtx);
    if (isUseLocalOffset && mCapTargetInfo1->mIsEscapeLocalOffset) {
        al::calcTransLocalOffsetByMtx(out, mtx, mCapTargetInfo1->mEscapeLocalOffset);
    } else {
        out->x = mtx.m[0][3];
        out->y = mtx.m[1][3];
        out->z = mtx.m[2][3];
    }
}

bool HackCap::tryAppendAttack() {
    if (!mStateThrowStay->isEnableAppendAttack())
        return false;
    if (!mInput->isTriggerAppendCapAttack(_299))
        return false;
    _2a5[0] = 1;
    _238 = mInput->isTriggerSwingRightHand() ? SwingHandType::Val2 : SwingHandType::Val1;

    sead::Vector3f sideDir = _24c;
    if (al::isNearZero(_268.x, 0.001f) && !al::isNearZero(_268.y, 0.001f)) {
        sideDir = _240.cross(_24c);
        if (sideDir.dot(al::getGravity(this)) >= 0.0f)
            sideDir = -sideDir;
        al::normalize(&sideDir);
    }

    sead::Vector3f upDir = _240.cross(sideDir);
    al::normalize(&upDir);

    sead::Vector3f viewInputX(0.0f, 0.0f, 0.0f);
    al::calcVecViewInput(&viewInputX, sead::Vector2f::ex, sideDir,
                         &PlayerFunction::getPlayerViewMtx(mPlayerActor));
    f32 dotX = upDir.dot(viewInputX);
    sead::Vector3f viewInputY(0.0f, 0.0f, 0.0f);
    al::calcVecViewInput(&viewInputY, sead::Vector2f::ey, sideDir,
                         &PlayerFunction::getPlayerViewMtx(mPlayerActor));
    if (dotX < 0.0f) {
        if (sead::Mathf::abs(upDir.dot(viewInputY)) < 0.9659258f)
            upDir = -upDir;
    }

    const sead::Vector2f& throwDir = mInput->getCapThrowDir();
    sead::Vector3f sideScaled = sideDir * throwDir.y;
    sead::Vector3f dir(0.0f, 0.0f, 0.0f);
    if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
        (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        sead::Vector3f negGravity = -al::getGravity(this);
        mInput->calcCapSeparateMoveInput(&dir, negGravity);
    } else {
        sead::Vector3f upScaled = upDir * throwDir.x;
        dir = sideScaled + upScaled + _240;
        dir += upScaled;
    }
    if (!al::tryNormalizeOrZero(&dir))
        dir = _240;

    sead::Vector3f vert(0.0f, 0.0f, 0.0f);
    sead::Vector3f outPos(0.0f, 0.0f, 0.0f);
    al::verticalizeVec(&vert, dir, sideDir);
    if (al::tryNormalizeOrZero(&vert)) {
        al::HitSensor* sensor = mCapEyeSensorHitHolder->findNearestSensor(
            &outPos, al::getTrans(this), dir, vert, 250.0f, 90.0f, 80.0f);
        if (!sensor)
            sensor = mCapEyeSensorHitHolder->findNearestSensor(
                &outPos, al::getTrans(this), dir, vert, 250.0f, 180.0f, 180.0f);
        if (sensor) {
            _2e8 = outPos;
            _2a5[0] = 0;
        } else {
            f32 absThrowX = sead::Mathf::abs(throwDir.x);
            f32 absThrowY = sead::Mathf::abs(throwDir.y);
            if (!_299 && (mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
                (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
                _2e8 = dir;
            } else {
                if (absThrowX > absThrowY)
                    _2e8 = upDir * throwDir.x;
                else
                    _2e8 = sideScaled;
                if (mInput->isMove())
                    _2e8 += _240;
            }
            if (!al::tryNormalizeOrZero(&_2e8))
                _2e8 = _240;
        }
    }

    if (al::isParallelDirection(_240, _2e8, 0.01f)) {
        _240 = _2e8;
    } else {
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatRotationRate(&quat, _240, _2e8, 1.0f);
        _24c.rotate(quat);
        al::normalize(&_24c);
        _240.rotate(quat);
        al::normalize(&_240);
    }

    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, -_240, _24c);
    al::updatePoseQuat(this, quat);
    return true;
}

bool HackCap::tryCollideWallLockOn() {
    al::HitSensor* sensor = rs::tryGetCollidedWallSensor(mPlayerColliderHackCap);
    if (!sensor)
        return false;
    if (_1b0 != 0) {
        al::LiveActor* host = al::getSensorHost(sensor);
        for (s32 i = 0; i < _1b0; i++) {
            s32 idx = _1ac + i;
            if (idx >= _1a8)
                idx -= _1a8;
            if (static_cast<al::LiveActor**>(_1a0_arr)[idx] == host)
                return false;
        }
    }
    if (rs::sendMsgCapStartLockOn(sensor, mAttackSensor)) {
        prepareLockOn(sensor);
        return true;
    }
    return false;
}

bool HackCap::isEnableSpinAttack() const {
    if (al::isNerve(this, &NrvHackCap.Hide))
        return true;
    if (al::isNerve(this, &NrvHackCap.Catch))
        return true;
    if (_299)
        return false;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0)
        return false;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        return false;
    if (!al::isNerve(this, &NrvHackCap.ThrowStay))
        return false;
    if (mIsSeparateFlying)
        return false;
    return al::calcDistance(this, mPlayerActor) <= 200.0f;
}

bool HackCap::isEnableCapTouchJumpInput() const {
    if (_299) {
        if (!mInput->isHoldAction())
            return false;
    } else if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
               (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0) {
        if (!_510)
            return false;
    } else {
        if (!mInput->isHoldSpinCap())
            return false;
    }
    if (_299)
        return true;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) != 0)
        return true;
    if ((mPlayerSeparateCapFlag->_flagsWord & 0xff) == 0)
        return true;
    if (mInput->isTriggerCapSeparateJump())
        return true;
    return mStateThrowStay->isHomingPlayerJump();
}

bool HackCap::isEnableHackThrowAutoCatch() const {
    if (!mIsSeparateFlying)
        return false;
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    mCapTargetInfo1->makeLockOnMtx(&mtx);
    sead::Vector3f targetPos(mtx.m[0][3], mtx.m[1][3], mtx.m[2][3]);
    const sead::Vector3f& selfPos = al::getTrans(this);
    return (targetPos - selfPos).length() < 100.0f;
}

void HackCap::updateFrameOutLayout() {
    u32 flags = mPlayerSeparateCapFlag->_flagsWord;
    if ((flags & 0xff0000) == 0 && (flags & 0xff) == 0)
        goto hide;
    if (al::isNerve(this, &NrvHackCap.Hack) || al::isNerve(this, &NrvHackCap.Hide) ||
        al::isNerve(this, &NrvHackCap.Rescue))
        goto hide;
    {
        sead::Vector3f layoutPos;
        al::calcLayoutPosFromWorldPosWithClampOutRange(&layoutPos, this, al::getTrans(this), 50.0f,
                                                       0);
        if (layoutPos.z > 0.0f) {
            layoutPos.x = -layoutPos.x;
            layoutPos.y = -layoutPos.y;
        }
        f32 radius = al::calcLayoutRadiusFromWorldRadius(al::getTrans(this), this, 120.0f);
        radius = sead::Mathf::abs(radius);
        f32 halfW = al::getLayoutDisplayWidth() * 0.5f;
        f32 halfH = al::getLayoutDisplayHeight() * 0.5f;
        if (sead::Mathf::abs(layoutPos.x) <= radius + halfW &&
            sead::Mathf::abs(layoutPos.y) <= radius + halfH) {
            if (_5f0->isAppearOrWait())
                _5f0->end();
            _5e8 = layoutPos.z > 0.0f;
            return;
        }
        f32 boundX = halfW - 85.0f;
        f32 boundY = halfH - 65.0f;
        sead::Vector2f clampedPos;
        clampedPos.x = sead::Mathf::clamp(layoutPos.x, -boundX, boundX);
        clampedPos.y = sead::Mathf::clamp(layoutPos.y, -boundY, boundY);
        f32 scaleX = 1.0f;
        if (!al::isNearZero(layoutPos.x, 0.1f))
            scaleX = clampedPos.x / layoutPos.x;
        f32 scaleY = 1.0f;
        if (!al::isNearZero(layoutPos.y, 0.1f))
            scaleY = clampedPos.y / layoutPos.y;
        f32 scale = sead::Mathf::min(scaleX, scaleY);
        scale = sead::Mathf::clamp(scale, 0.0f, 1.0f);
        sead::Vector2f outPos(scale * layoutPos.x, scale * layoutPos.y);
        sead::Vector2f dir(layoutPos.x - outPos.x, layoutPos.y - outPos.y);
        f32 angle = 0.0f;
        if (al::tryNormalizeOrZero(&dir)) {
            f32 negDot = dir.x * (-sead::Vector2f::ey.x) - dir.y * sead::Vector2f::ey.y;
            f32 clampedDot = sead::Mathf::clamp(negDot, -1.0f, 1.0f);
            angle = sead::Mathf::acos(clampedDot) * 57.29578f;
            f32 cross = dir.y * (-sead::Vector2f::ey.x) - dir.x * (-sead::Vector2f::ey.y);
            if (cross < 0.0f)
                angle = -angle;
        }
        _5e8 = true;
        if (al::isDead(_5f0))
            _5f0->appear();
        al::setLocalTrans(_5f0, outPos);
        sead::Vector3f rot(0.0f, 0.0f, angle);
        al::setPaneLocalRotate(_5f0, "PicBalloon", rot);
        return;
    }
hide:
    _5e8 = false;
    if (_5f0->isAppearOrWait())
        _5f0->end();
}

void HackCap::updateTargetLayout() {
    if (rs::isModeDiverOrJungleGymRom())
        return;

    GameDataHolderAccessor accessor(this);
    if (!GameDataFunction::isEnableCap(accessor))
        goto hideTarget;
    if (al::isPlayingEntranceCamera(this, 0))
        goto hideTarget;

    if (al::isNerve(this, &NrvHackCap.Hide) || al::isNerve(this, &NrvHackCap.SpinAttack)) {
    } else if (al::isNerve(this, &NrvHackCap.Rethrow)) {
        goto hideTarget;
    } else if (al::isNerve(this, &NrvHackCap.Throw) || al::isNerve(this, &NrvHackCap.Rethrow) ||
               al::isNerve(this, &NrvHackCap.ThrowBrake) ||
               al::isNerve(this, &NrvHackCap.ThrowRolling) ||
               al::isNerve(this, &NrvHackCap.ThrowRollingBrake) ||
               al::isNerve(this, &NrvHackCap.ThrowAppend)) {
    } else {
        goto hideTarget;
    }

    if (!rs::isPlayer3D(this))
        goto hideTarget;

    if (al::isNerve(this, &NrvHackCap.Hide) || al::isNerve(this, &NrvHackCap.SpinAttack)) {
        if (al::isActive(mTargetLayout)) {
            sead::Vector3f frontDir;
            sead::Vector3f upDir;
            sead::Vector3f targetPos;
            rs::calcPlayerFrontDir(&frontDir, this);
            rs::calcPlayerUpDir(&upDir, this);
            al::HitSensor* sensor = mPlayerEyeSensorHitHolder->findTargetMarkerSensor(
                &targetPos, rs::getPlayerBodyPos(this), frontDir, upDir, 20.0f, 0.0f, 200.0f);
            if (rs::isPlayerEnableTalkGround(this)) {
                mTargetLayoutCounter = 0;
                mTargetSensor = sensor;
                goto storeTarget;
            }
            mTargetLayoutCounter = mTargetLayoutCounter + 1 > 30 ? 30 : mTargetLayoutCounter + 1;
            if (mTargetLayoutCounter < 30) {
                if (sensor == mTargetSensor)
                    goto storeTarget;
                mTargetSensor = nullptr;
                goto hideTarget;
            }
            mTargetSensor = nullptr;
            goto hideTarget;
        }
        if (rs::isPlayerEnableTalkGround(this)) {
            if (mTargetLayoutCounter < 30) {
                mTargetLayoutCounter++;
                if (mTargetLayoutCounter - 1 != 29) {
                    mTargetSensor = nullptr;
                    goto hideTarget;
                }
            } else {
                mTargetLayoutCounter = 30;
            }
            sead::Vector3f frontDir;
            sead::Vector3f upDir;
            sead::Vector3f targetPos;
            rs::calcPlayerFrontDir(&frontDir, this);
            rs::calcPlayerUpDir(&upDir, this);
            mTargetSensor = mPlayerEyeSensorHitHolder->findTargetMarkerSensor(
                &targetPos, rs::getPlayerBodyPos(this), frontDir, upDir, 20.0f, 0.0f, 200.0f);
            goto storeTarget;
        }
        mTargetLayoutCounter = 0;
        mTargetSensor = nullptr;
        goto hideTarget;
    }

storeTarget:
    if (!mTargetSensor)
        goto hideTarget;
    mTargetSensorForLayout = mTargetSensor;
    {
        sead::Vector3f worldPos;
        if (!rs::sendMsgTargetMarkerPosition(mTargetSensorForLayout, mAttackSensor, &worldPos)) {
            const sead::Vector3f& sensorPos = al::getSensorPos(mTargetSensorForLayout);
            const sead::Vector3f& gravity = al::getGravity(this);
            f32 radius = al::getSensorRadius(mTargetSensorForLayout);
            worldPos = sensorPos - gravity * radius;
        }
        sead::Vector2f layoutPos;
        al::calcLayoutPosFromWorldPos(&layoutPos, this, worldPos);
        al::setLocalTrans(mTargetLayout, layoutPos);
        const sead::Matrix34f& viewMtx = al::getViewMtx(this, 0);
        sead::Vector3f viewPos;
        viewPos.x = viewMtx.m[0][0] * worldPos.x + viewMtx.m[0][1] * worldPos.y +
                    viewMtx.m[0][2] * worldPos.z + viewMtx.m[0][3];
        viewPos.y = viewMtx.m[1][0] * worldPos.x + viewMtx.m[1][1] * worldPos.y +
                    viewMtx.m[1][2] * worldPos.z + viewMtx.m[1][3];
        viewPos.z = viewMtx.m[2][0] * worldPos.x + viewMtx.m[2][1] * worldPos.y +
                    viewMtx.m[2][2] * worldPos.z + viewMtx.m[2][3];
        const sead::Matrix44f& projMtx = al::getProjectionMtx(this, 0);
        sead::Vector3f clipPos;
        clipPos.x = projMtx.m[0][0] * viewPos.x + projMtx.m[0][1] * viewPos.y +
                    projMtx.m[0][2] * viewPos.z + projMtx.m[0][3];
        clipPos.y = projMtx.m[1][0] * viewPos.x + projMtx.m[1][1] * viewPos.y +
                    projMtx.m[1][2] * viewPos.z + projMtx.m[1][3];
        clipPos.z = projMtx.m[2][0] * viewPos.x + projMtx.m[2][1] * viewPos.y +
                    projMtx.m[2][2] * viewPos.z + projMtx.m[2][3];
        sead::Vector2f screenPos;
        al::calcScreenPosFromWorldPos(&screenPos, this, worldPos);
        bool isOccluded = al::isOcclusionCulling(mOcclusionCullingJudge, clipPos, screenPos,
                                                  48.0f, 24.0f, 50.0f, 3);
        bool isSubjective = rs::isPlayerCameraSubjective(this);
        sead::Vector3f layoutPos3D;
        al::calcLayoutPosFromWorldPos(&layoutPos3D, this, worldPos);
        if (!isOccluded && !isSubjective && layoutPos3D.z <= 0.0f) {
            al::setLocalTrans(mTargetLayout, layoutPos);
            if (!al::isDead(mTargetLayout))
                return;
            mTargetLayout->end();
            mTargetLayoutCounter = 0;
            return;
        }
        if (!al::isActive(mTargetLayout))
            return;
        if (!mTargetLayout->isAppearOrWait())
            return;
        mTargetLayout->end();
        mTargetLayoutCounter = 0;
        return;
    }

hideTarget:
    mTargetSensor = nullptr;
    mTargetLayoutCounter = 0;
    if (mTargetSensorForLayout) {
        sead::Vector3f worldPos;
        if (!rs::sendMsgTargetMarkerPosition(mTargetSensorForLayout, mAttackSensor, &worldPos)) {
            const sead::Vector3f& sensorPos = al::getSensorPos(mTargetSensorForLayout);
            const sead::Vector3f& gravity = al::getGravity(this);
            f32 radius = al::getSensorRadius(mTargetSensorForLayout);
            worldPos = sensorPos - gravity * radius;
        }
        sead::Vector2f layoutPos;
        al::calcLayoutPosFromWorldPos(&layoutPos, this, worldPos);
        al::setLocalTrans(mTargetLayout, layoutPos);
        sead::Vector3f layoutPos3D;
        al::calcLayoutPosFromWorldPos(&layoutPos3D, this, worldPos);
        if (layoutPos3D.z > 0.0f)
            mTargetLayout->appear();
    }
    if (!al::isActive(mTargetLayout)) {
        if (mTargetSensorForLayout)
            mTargetSensorForLayout = nullptr;
        return;
    }
    if (!al::isSensorValid(mTargetSensorForLayout) ||
        !al::isAlive(al::getSensorHost(mTargetSensorForLayout)) ||
        rs::sendMsgPlayerDisregardTargetMarker(mTargetSensorForLayout, mTargetSensorForLayout)) {
        mTargetLayout->appear();
        return;
    }
    if (mTargetLayout->isAppearOrWait()) {
        mTargetLayout->end();
        mTargetLayoutCounter = 0;
    }
    return;
}

void HackCap::syncPuppetSilhouette() {
    if (mIsHidePuppetCapSilhouette) {
        if (al::isAlive(this) && !al::isHideModel(this))
            al::hideSilhouetteModelIfShow(this);
        if (al::isAlive(mEquipmentHat) && !al::isHideModel(mEquipmentHat))
            al::hideSilhouetteModelIfShow(mEquipmentHat);
    } else {
        if (al::isAlive(this) && !mIsSeparateFlying && !al::isNerve(this, &NrvHackCap.Hack) &&
            !al::isHideModel(this))
            al::showSilhouetteModelIfHide(this);
        if (al::isAlive(mEquipmentHat) && !al::isHideModel(mEquipmentHat))
            al::showSilhouetteModelIfHide(mEquipmentHat);
    }
}

void HackCap::startCatch(const char* actionName, bool isStartHitReaction,
                         const sead::Vector3f& hitReactionPos) {
    HackCapJointControlKeeper* jointControlKeeper = mHackCapJointControlKeeper;
    jointControlKeeper->mRotateX = 0.0f;
    jointControlKeeper->mRotateY = 0.0f;
    jointControlKeeper->mRotateZ = 0.0f;
    jointControlKeeper->_c = 0;
    al::copyPose(this, mPlayerActor);
    al::startAction(this, actionName);
    if (isStartHitReaction)
        al::startHitReactionHitEffect(this, "帽子キャッチ", hitReactionPos);
    if (al::isNerve(this, &NrvHackCap.Hide)) {
        if (!_2a0[2])
            _2a0[2] = true;
    }
    mEquipmentHat->kill();
    mLockOnCapEyes->kill();
    al::setNerve(this, &NrvHackCap.Catch);
}

void HackCap::hide(bool isHide) {
    if (al::isNerve(this, &NrvHackCap.Hide)) {
        if (!isHide)
            return;
        if ((mPlayerSeparateCapFlag->_flagsWord & 0xff0000) == 0 &&
            (mPlayerSeparateCapFlag->_flagsWord & 0xff) != 0)
            return;
        if (!mStateHide->isSeparateMode())
            return;
        mStateHide->cancelSeparateMode();
    }
    if (mIsSeparateFlying)
        endHackThrowAndReturnHack();
    if (_1d0)
        tryReturn(isHide, nullptr);
    al::offCollide(this);
    al::invalidateShadow(this);
    al::invalidateHitSensors(this);
    mCapTargetInfo1 = nullptr;
    _1d0 = nullptr;
    _2a8 = false;
    al::setVelocityZero(this);
    al::hideModelIfShow(this);
    mEquipmentHat->kill();
    mLockOnCapEyes->kill();
    al::updateMaterialCodeWater(this, false);
    al::setNerve(this, &NrvHackCap.Hide);
}

void HackCap::activateInvincibleEffect() {
    if (al::isDead(mEquipmentHat))
        return;
    al::tryDeleteEffect(mEquipmentHat, "InvincibleStart");
    al::tryDeleteEffect(mEquipmentHat, "Invincible");
    if (mCapTargetInfo1) {
        f32 rate = al::calcRate01(mCapTargetInfo1->mLockOnScale, 1.0f, 2.25f);
        f32 scale = al::lerpValue(1.0f, 2.25f, rate);
        al::setEffectAllScale(mEquipmentHat, "InvincibleStart", sead::Vector3f(scale, scale, scale));
        al::setEffectAllScale(mEquipmentHat, "Invincible", sead::Vector3f(scale, scale, scale));
        if (mCapTargetInfo1->_7d)
            return;
    }
    al::emitEffect(mEquipmentHat, "InvincibleStart", nullptr);
    al::emitEffect(mEquipmentHat, "Invincible", nullptr);
}

void HackCap::syncInvincibleEffect(bool isEnable) {
    if (al::isDead(mEquipmentHat))
        return;
    if (mCapTargetInfo1) {
        f32 rate = al::calcRate01(mCapTargetInfo1->mLockOnScale, 1.0f, 2.25f);
        f32 scale = al::lerpValue(1.0f, 2.25f, rate);
        sead::Vector3f scaleVec(scale, scale, scale);
        al::setEffectAllScale(mEquipmentHat, "Invincible", scaleVec);
        if (mCapTargetInfo1->_7d) {
            al::tryDeleteEffect(mEquipmentHat, "Invincible");
        } else if (isEnable) {
            al::tryEmitEffect(mEquipmentHat, "Invincible", nullptr);
        } else {
            al::tryDeleteEffect(mEquipmentHat, "Invincible");
        }
    } else if (isEnable) {
        al::tryEmitEffect(mEquipmentHat, "Invincible", nullptr);
    } else {
        al::tryDeleteEffect(mEquipmentHat, "Invincible");
    }
}

bool HackCap::receiveRequestTransferHack(al::HitSensor* sensor, al::HitSensor* other) {
    if (mIsSeparateFlying)
        return false;
    if (!al::isNerve(this, &NrvHackCap.Hack))
        return false;
    if (rs::isActiveDemo(this))
        return false;
    if (!rs::sendMsgCapStartLockOn(sensor, mAttackSensor))
        return false;
    if (al::isNerve(this, &NrvHackCap.Hack)) {
        mPlayerBodySensor = other;
        mIsSeparateFlying = true;
        mEquipmentHat->kill();
        mLockOnCapEyes->kill();
        startThrowSeparatePlay(sead::Vector3f::ez, sead::Vector3f::ey, 0.0f, false);
    }
    prepareLockOn(sensor);
    return true;
}

void HackCap::updateCapEyeShowHide(bool isShow, s32 step) {
    if (_5ba) {
        s32 value;
        if (isShow)
            value = al::converge(_5bc, step, 1);
        else
            value = 0;
        _5bc = value;
        _5ba = value < step;
        if (value >= step)
            al::startAction(mLockOnCapEyes, "Appear");
    } else {
        if (isShow)
            return;
        if (al::isActionPlaying(mLockOnCapEyes, "Disappear"))
            return;
        al::startAction(mLockOnCapEyes, "Disappear");
        _5bc = 0;
    }
}

void HackCap::exeCatch() {
    updateSpinAttackPose(this, mPlayerActor, mPlayerJointControlKeeper);
    if (al::isFirstStep(this)) {
        _2a0[0] = false;
        al::showModelIfHide(this);
        mEquipmentHat->kill();
        mLockOnCapEyes->kill();
    }
    if (al::isGreaterStep(this, al::getActionFrame(this)))
        hide(false);
}

void HackCap::updateLavaSurfaceMove() {
    bool isOnLava = _29b;
    _29b = false;
    const sead::Vector3f& trans = al::getTrans(this);
    sead::Vector3f negGravity = -al::getGravity(this);
    sead::Vector3f surfacePos(0.0f, 0.0f, 0.0f);
    sead::Vector3f surfaceNormal(0.0f, 0.0f, 0.0f);
    if (al::calcFindFireSurface(&surfacePos, &surfaceNormal, this, trans, negGravity, 100.0f)) {
        sead::Vector3f diff = surfacePos - trans;
        f32 dot = diff.dot(negGravity);
        if (dot > 0.0f) {
            sead::Vector3f* vel = al::getVelocityPtr(this);
            al::limitVectorOppositeDir(vel, negGravity, *vel, vel->length());
        }
        f32 moveDist = (dot + 25.0f) * 0.25f;
        f32 move;
        if (isOnLava) {
            f32 velDot = negGravity.dot(al::getVelocity(this));
            if (velDot > 0.0f) {
                move = sead::Mathf::max(0.0f, moveDist);
                _29b = true;
            } else {
                move = moveDist;
                _29b = true;
            }
        } else {
            move = sead::Mathf::max(0.0f, moveDist);
            _29b = dot > 0.0f;
        }
        al::setTrans(this, trans + negGravity * move);
    }
}

bool HackCap::stayWallHit() {
    bool wasWallHit = _530;
    if (rs::isCollidedWall(mPlayerColliderHackCap) && !_530) {
        _530 = true;
        _534 = 10;
        __builtin_memcpy(&mWallPos, &rs::getCollidedWallPos(mPlayerColliderHackCap),
                         sizeof(sead::Vector3f));
        __builtin_memcpy(&mWallNormal, &rs::getCollidedWallNormal(mPlayerColliderHackCap),
                         sizeof(sead::Vector3f));
        __builtin_memcpy(&mWallVelocity, &al::getVelocity(this), sizeof(sead::Vector3f));
    }
    if (_530 && (!wasWallHit || !al::isNerve(this, &NrvHackCap.ThrowStay))) {
        al::startHitReactionHitEffect(this, "コリジョン衝突停止", mWallPos);
        al::startSeWithParam(this, "HitWall", mWallVelocity.length(), "");
    }
    const al::Nerve* nextNerve;
    if (al::isNerve(this, &NrvHackCap.ThrowAppend)) {
        _29e = false;
        _274 = 0.0f;
        _264 = 0.0f;
        _268.set(0.0f, 0.0f);
        _270 = 0.0f;
        if (_298)
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        else
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        nextNerve = &NrvHackCap.Return;
    } else {
        if (al::isNerve(this, &NrvHackCap.ThrowStay))
            return false;
        _29e = false;
        _274 = 0.0f;
        _264 = 0.0f;
        _268.set(0.0f, 0.0f);
        _270 = 0.0f;
        if (_298)
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinL");
        else
            al::tryStartActionIfNotPlaying(mThrowingHatEyes, "SpinR");
        al::setVelocityZero(this);
        nextNerve = &NrvHackCap.ThrowStay;
    }
    al::setNerve(this, nextNerve);
    return true;
}
