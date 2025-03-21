#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <container/seadRingBuffer.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class ActorMatrixCameraTarget;
class CameraTicket;
class EventFlowExecutor;
class HitSensor;
class JointLocalAxisRotator;
class MtxConnector;
class SensorMsg;
}  // namespace al

class IUsePlayerHack;
class CapTargetInfo;
class PlayerHackStartShaderCtrl;

class HackFork : public al::LiveActor {
public:
    HackFork(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void initAfterPlacement() override;
    void control() override;
    void calcAnim() override;

    void initBasicPoseInfo();
    bool tryTouch(f32 force, const char* reactionName);
    void resetCapMtx(al::HitSensor* sensor);
    bool isNerveHackable() const;
    bool isHack() const;
    void controlSpring();
    void checkSwing();
    bool trySwingJump();
    bool updateInput(sead::Vector3f* pullDirection, sead::Vector3f frontDir);
    f32 getJumpRange() const;

    void bendAndTwist(const sead::Vector3f& direction, const sead::Vector3f& front);
    void shoot();
    void updateCapMtx();
    void calcHackDir(al::HitSensor* sensor);

    void exeWait();
    void exeHackStartWait();
    void exeDamping();
    void exeHackStart();
    void exeHackWait();
    void exeHackBend();
    void exeHackShoot();

private:
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    const char* mJointName = nullptr;
    sead::Matrix34f mPoseMtx = sead::Matrix34f::ident;
    sead::Matrix34f mInvJointMtx = sead::Matrix34f::ident;
    sead::Matrix34f mStartingPoseMtx = sead::Matrix34f::ident;
    sead::Matrix34f mNextPoseMtx = sead::Matrix34f::ident;
    al::CameraTicket* mCameraTicket = nullptr;
    al::ActorMatrixCameraTarget* mMatrixCameraTarget = nullptr;
    sead::Matrix34f mCameraTargetMtx = sead::Matrix34f::ident;
    sead::Vector3f mCameraOffset = {0.0f, 0.0f, 0.0f};
    bool mHasCameraOffset = false;
    f32 mDampingStrength = 0.0f;
    sead::Vector3f mSideDir = sead::Vector3f::ez;
    f32 mDampingForce = 0.0f;
    sead::PtrArray<al::JointLocalAxisRotator> mJointRotationHolder;
    f32 mTouchForce = 0.0f;
    bool mIsHackSwing = false;
    u32 mTimeSinceSwingStart = 0;
    bool mIsSwingJump = false;
    bool mIsReadyToShoot = false;
    sead::FixedRingBuffer<sead::Vector3f, 10> mInputBuffer;
    bool mIsControlledByPlayer = false;
    sead::Quatf mHackRotation = sead::Quatf::unit;
    sead::Vector3f mPullDirection2 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mPullDirection = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mUpDir = {0.0f, 0.0f, 0.0f};
    sead::Quatf mHackDir = sead::Quatf::unit;
    sead::Quatf _340 = sead::Quatf::unit;
    bool mIsJumpFoward = false;
    bool mIsLimitterFree = false;
    s32 mTouchDelay = 0;
    s32 mAirVel = 0;
    al::MtxConnector* mMtxConnector = nullptr;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    PlayerHackStartShaderCtrl* mHackStartShaderCtrl = nullptr;
    sead::Vector3f mHeadGuidePos = {0.0f, 0.0f, 0.0f};
};
