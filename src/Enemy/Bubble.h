#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class ActorMatrixCameraTarget;
class AnimScaleController;
class CameraTicket;
class CollisionPartsFilterSpecialPurpose;
class HitSensor;
class JointSpringControllerHolder;
class SensorMsg;
class Triangle;
}  // namespace al

class CapTargetInfo;
class IUsePlayerHack;
class DisregardReceiver;
class PlayerHackStartShaderCtrl;
class BubbleStateInLauncher;

class Bubble : public al::LiveActor {
public:
    Bubble(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void updateCollider() override;
    void appear() override;
    void calcAnim() override;

    void recalcClippingInfo();
    void start();
    void killAll();
    bool isIgnoreTriangle(const al::Triangle& triangle) const;
    void checkEffectPos();
    bool isValidCollisionOrWaveCheck() const;
    bool isHack() const;
    void revive();
    void updateLavaWave();
    void updateScrollAnimRate();
    bool isOnGround() const;
    void setupHack();
    bool isCurrentNerveEnableLockOnAndStartHack() const;
    void offGroupClipping();
    void startHackLocal(al::HitSensor* self, al::HitSensor* other);
    bool isInvalidHackEscape() const;
    void prepareEndHack();
    void endHackByCancel();
    void forceEndHack();
    void updatePosOnWave();
    void updateVelocityIfValidCollision();
    bool isOnDamageFire() const;
    void tryStartHitReactionUp();
    void updateShadowMaskLength();
    void tryStartHitReactionDown();
    void shiftSink();
    void headUp();
    void shiftReadyOrMove();
    bool trySendMsgStartInSaucePan();
    void tryHitReactionThroughFence();
    bool tryShiftLand();
    bool isHoldHackAction() const;
    bool tryBoundMoveWall();
    void updateHackOnGround();
    bool constrainLavaDomain();
    bool tryShiftFall();
    bool isTriggerHackSwing() const;
    void faceToMoveVec();
    bool isTriggerHackJump() const;
    void revertTargetQuatInHackJump(sead::Quatf* quatA, sead::Quatf* quatB);
    void calcHackerMoveVec(sead::Vector3f* moveVec, const sead::Vector3f& inputDir) const;
    void makeDisplayQuatInHackJump(const sead::Quatf& quatA, const sead::Quatf& quatB,
                                   const sead::Quatf& quatC, bool isValue);
    bool isDropAttackCollision() const;
    bool isRiseAttackCollision() const;
    bool isHoldHackJump() const;
    bool tryShiftContinuousJump();
    bool calcHackerMoveDir(sead::Vector3f* moveDir, const sead::Vector3f& inputDir) const;
    bool isWaitingLaunch() const;
    void launch(const sead::Vector3f& dir, f32 force, al::CameraTicket* cameraTicket);
    void launchCancel(const sead::Vector3f& dir);
    void showHackCap();
    void hideHackCap();
    bool isTriggerJump() const;
    bool isTriggerAction() const;
    bool isTriggerHackAction() const;
    bool isTriggerCancel() const;
    void startJumpAnim(f32 force);
    void resetAndAppear(const sead::Vector3f& trans, const sead::Quatf& quat, f32 unused);
    void endHackCommon();
    void calcLaunchPos(sead::Vector3f* pos, const sead::Vector3f& vecA, f32 valA, f32 valB) const;
    void onGroupClipping();
    bool isOnGroundNoVelocity() const;
    void updateCollisionPartsMove();
    void accelStick();
    bool addHackActorAccelStick(sead::Vector3f* stickAccel, f32 scale, const sead::Vector3f& dir);
    bool isGroundOverTheWave(bool checkHeight, const sead::Vector3f& wavePos) const;
    bool isEnableSnapWaveSurface() const;

    void exeStandBy();
    void exeDelay();
    void exeRailMove();
    void exeReady();
    void exeUp();
    void exeTurn();
    void exeDown();
    void exeSink();
    void exeWait();
    void exeDie();
    void exeWaitHack();
    void exeWaitHackStart();
    void endWaitHackStart();
    void exeWaitHackFall();
    void exeHackFall();
    void exeHackMove();
    void endHackMove();
    void exeHackJump();
    void endHackJump();
    void exeHackLand();
    void exeHackInLauncher();
    void endHackInLauncher();
    void exeHackResetPos();
    void exeHackDemo();
    void exeRevive();

private:
    sead::Vector3f mCurrentPosition = {0.0f, 0.0f, 0.0f};
    f32 mJumpHeight = 0.0f;
    s32 mTurnFrameDelay = 0;
    u32 mTurnFrameCount = 0;
    f32 mRailTotalLength = 0.0f;
    s32 mRailMoveFrame = 0;
    s32 mWaitFrameNum = 0;
    s32 mDelayFrameNum = 0;
    sead::Vector3f mClippingPos = {0.0f, 0.0f, 0.0f};
    u32 mWaitDelayCount = 0;
    u32 mWaitDelayTime = 0;
    sead::Vector3f mJumpForce = {0.0f, 0.0f, 0.0f};
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    u32 mShiftFallDelay = 0;
    sead::Quatf mStartingRotation = sead::Quatf::unit;
    sead::Vector3f mResetPosition = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mRevivePosition = {0.0f, 0.0f, 0.0f};
    bool mIsInFire = false;
    sead::Vector3f mLandPos = {0.0f, 0.0f, 0.0f};
    BubbleStateInLauncher* mBubbleStateInLauncher = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    al::ActorMatrixCameraTarget* mActorMatrixCameraTarget = nullptr;
    sead::Matrix34f mCameraMtx = sead::Matrix34f::ident;
    sead::Vector3f mStickForce = {0.0f, 0.0f, 0.0f};
    s32 mJumpFrame = 0;
    sead::Quatf mCurrentRotation = sead::Quatf::unit;
    sead::Vector3f mUpDir = sead::Vector3f::ey;
    f32 mHackTurnAngle = 0.0f;
    u32 mHackTurnFrame = 1;
    u32 mHackTurnDelay = 1;
    sead::Vector3f mFireSurface = sead::Vector3f::ey;
    al::LiveActor* mProveActor = nullptr;
    bool mIsClipped = false;
    s32 mWaitDelay = 0;
    sead::Vector3f mPreviousTrans = {0.0f, 0.0f, 0.0f};
    sead::Matrix34f _24c = sead::Matrix34f::ident;
    bool mIsWaveCheckOn = false;
    sead::Quatf mRailFinalRotation = sead::Quatf::unit;
    sead::Vector3f mShadowMaskOffset = {0.0f, 0.0f, 0.0f};
    f32 mShadowMaskDropLength = 0;
    s32 mCancelLaunchCooldown = 0;
    sead::Vector3f mColliderPos = {0.0f, 0.0f, 0.0f};
    u32 mJumpDelay = 15;
    al::CollisionPartsFilterSpecialPurpose* mCollisionPartsFilter = nullptr;
    bool mIsCheckEffectPosRequired = true;
    sead::Vector3f mSurfaceProvePos[2];
    bool mIsOnHitReactionEffect = true;
    sead::Matrix34f mLavaSurfaceMtx = sead::Matrix34f::ident;
    const char* mMaterialCode = "";
    sead::Vector3f mResetTargetPos = {0.0f, 0.0f, 0.0f};
    bool mIsPlayerCaptured = false;
    bool mIsInBossSequence = false;
    bool mIsHackOnGround = false;
    DisregardReceiver* mDisregardReceiver = nullptr;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    const char* mActionName = "";
    al::AnimScaleController* mAnimScaleController = nullptr;
    bool mIsInSaucePan = false;
    bool mIsShowTutorial = false;
    bool mIsOnGroupClipping = true;
};

static_assert(sizeof(Bubble) == 0x358);
