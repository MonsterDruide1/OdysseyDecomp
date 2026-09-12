#pragma once

#include <basis/seadTypes.h>
#include <container/seadObjArray.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"

#include "Camera/PlayerColliderCameraTarget.h"
#include "Player/IUsePlayerCollision.h"

namespace al {
class CollisionPartsConnector;
class CameraTargetBase;
class TransCameraSubTarget;
class CameraSubTargetTurnParam;

}  // namespace al
class IUsePlayerPuppet;
class PlayerCollider;
class MotorcyclePlayerAnimator;
class BindKeepDemoInfo;
class MotorcycleColliderCameraTarget;

struct MotorcycleParams {
    bool isInBack(const sead::Vector3f pos) {
        for (s32 i = 0; i < backContactPoints.size(); i++) {
            const sead::Vector3f& contact = *backContactPoints[i];
            const f32 value = -pos.y * contact.y - pos.x * contact.x - pos.z * contact.z;
            if (value < 0.0f)
                return true;
        }
        return false;
    }

    bool isInFront(const sead::Vector3f pos) {
        for (s32 i = 0; i < frontContactPoints.size(); i++)
            if (pos.dot(*frontContactPoints[i]) < 0.0f)
                return true;
        return false;
    }

    bool isOnGround = false;
    char isOnJump = false;
    bool isTouchingGround = false;
    bool isTouchingWall = false;
    bool isTouchingFront = false;
    bool isTouchingBack = false;
    sead::Vector3f groundNormalAvg = {0.0f, 0.0f, 0.0f};
    sead::FixedObjArray<sead::Vector3f, 64> frontContactPoints;
    sead::FixedObjArray<sead::Vector3f, 64> backContactPoints;
    s32 framesInAir = 0;
    sead::Vector3f lastGroundPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f groundNormal = {0.0f, 1.0f, 0.0f};
};

static_assert(sizeof(MotorcycleParams) == 0xc78);

struct MotorcyclePose {
    f32 steerAngle = 0.0f;
    f32 handleAngle = 0.0f;
    f32 leanAngle = 0.0f;
    f32 jumpAngle = 0.0f;
};

static_assert(sizeof(MotorcyclePose) == 0x10);

struct ParkingParams {
    al::LiveActor* actor = nullptr;
    MotorcyclePose pose;
    sead::Quatf quatA = sead::Quatf::unit;
    sead::Quatf quatB = sead::Quatf::unit;
    sead::Vector3f mCameraSubTargetPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vectorB = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(ParkingParams) == 0x50);

struct AccelerationState {
    void reset() {
        isAccelerating = false;
        accelRate = 0.0f;
    }

    bool isAccelerating = false;
    f32 accelRate = 0.0f;
};

static_assert(sizeof(AccelerationState) == 0x8);

struct SeRumbleState {
    void reset() {
        volume = 0.0f;
        rumble = 0;
    }

    f32 volume = 0.0f;
    s32 rumble = 0;
};

static_assert(sizeof(SeRumbleState) == 0x8);

class Motorcycle : public al::LiveActor, public IUsePlayerCollision {
public:
    Motorcycle(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void movement() override;
    void kill() override;
    void updateCollider() override;
    void calcAnim() override;

    PlayerCollider* getPlayerCollider() const override { return mPlayerCollider; }

    void exeWait();
    void exeCreep();
    void exeFall();
    void exeJump();
    void endJump();
    void exeReaction();
    void exeReset();
    void exeRideStart();
    void exeRideStartOn();
    void exeRideWait();
    void exeRideWaitJump();
    void endRideWaitJump();
    void exeRideWaitLand();
    void exeRideRunStart();
    void exeRideRun();
    void exeRideRunCollide();
    void exeRideRunFall();
    void exeRideRunWheelie();
    void endRideRunWheelie();
    void exeRideRunLand();
    void exeRideRunJump();
    void endRideRunJump();
    void exeRideRunBoundStart();
    void exeRideRunBound();
    void exeRideRunClash();
    void exeRideParkingSnap();
    void exeRideParkingStart();
    void exeRideParking();
    void exeRideParkingAfter();

    MotorcycleParams* getParams() const { return mParams; }

    bool isStickWorldPose() const { return mIsStickWorldPose; }

    BindKeepDemoInfo* getBindKeepDemoInfo() const { return mBindKeepDemoInfo; }

    s32* getOffDelay() { return &mGetOffDelay; }

    IUsePlayerPuppet** getPuppy() { return &mPlayerPuppet; }

private:
    IUsePlayerPuppet* mPlayerPuppet = nullptr;
    PlayerCollider* mPlayerCollider = nullptr;
    MotorcycleParams* mParams = nullptr;
    MotorcyclePlayerAnimator* mPlayerAnimator = nullptr;
    AccelerationState* mAccelerationState = nullptr;
    MotorcyclePose mPose;
    SeRumbleState* mSeRumbleState = nullptr;
    ParkingParams* mParkingParams = nullptr;
    BindKeepDemoInfo* mBindKeepDemoInfo = nullptr;
    al::CollisionPartsConnector* mCollisionPartsConnector = nullptr;
    MotorcycleColliderCameraTarget* mColliderCameraTarget = nullptr;

    al::TransCameraSubTarget* mTransCameraSubTarget = nullptr;
    al::CameraSubTargetTurnParam* mCameraSubTargetTurnParam = nullptr;
    sead::Vector3f mCameraSubTargetPos = {0.0f, 0.0f, 0.0f};
    f32 mCameraSubTargetAngle = 0.0f;

    sead::Vector3f* mCoursePoints = nullptr;
    s32 mCoursePointSize = 0;
    s32 mCoursePointFollowTimer = 0;

    sead::Quatf mBaseQuat = sead::Quatf::unit;
    sead::Vector3f mBaseTrans = {0.0f, 0.0f, 0.0f};
    sead::Quatf mPuppetQuat = sead::Quatf::unit;
    sead::Vector3f mPuppetTrans = {0.0f, 0.0f, 0.0f};

    sead::Vector3f mExternalPushVelocity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mSteerShiftVelocity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mPrevTransDelta = {0.0f, 0.0f, 0.0f};

    f32 mPuppetRotZLeft = 0;
    f32 mPuppetRotZRight = 0;
    sead::Matrix34f mWaterSurfaceMtx = sead::Matrix34f::ident;
    s32 mHighSpeedCameraTimer = 0;
    s32 mAutoResetTimer = -1;
    s32 mGetOffDelay = -1;
    s32 mItemSpawnCount = 3;

    bool mIsOnLight = false;
    bool mIsAccelerating = false;
    bool mIsStickWorldPose = false;
    bool mIsOnJump = false;
    bool mIsInWater = false;
    bool mIsExecutingBindDemo = false;
};

static_assert(sizeof(Motorcycle) == 0x250);

const sead::Vector3f sColliderTrans = {0.0f, 0.0f, -85.0f};

class MotorcycleColliderCameraTarget : public PlayerColliderCameraTarget {
public:
    MotorcycleColliderCameraTarget(const Motorcycle* actor)
        : PlayerColliderCameraTarget(actor, actor) {}

    void calcTrans(sead::Vector3f* offset) const override {
        al::calcTransLocalOffset(offset, getActor(), sColliderTrans);
    }
};
