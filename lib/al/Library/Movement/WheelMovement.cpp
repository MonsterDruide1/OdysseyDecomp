#include "Library/Movement/WheelMovement.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

namespace al {
WheelMovement::WheelMovement(LiveActor* actor, const ActorInitInfo& info)
    : NerveExecutor("車輪動作計算") {
    tryGetArg((s32*)&mRotateAxis, info, "RotateAxis");
    tryGetArg(&mRotateAccel, info, "RotateAccel");
    tryGetArg(&mMoveEndDegree, info, "MoveEndDegree");
    tryGetArg(&mNoRotateWidth, info, "NoRotateWidth");

    getQuat(&mInitialActorQuat, info);
    mActorQuat = mInitialActorQuat;

    sead::Vector3f localRotateAxis = sead::Vector3f::ex;
    calcQuatLocalAxis(&localRotateAxis, mInitialActorQuat, (s32)mRotateAxis);

    mMoveDir.setCross(localRotateAxis, sead::Vector3f::ey);
    if (isNearZero(mMoveDir))
        mMoveDir = sead::Vector3f::ez;

    if (isExistRail(actor)) {
        setSyncRailToNearestPos(actor);
        mIsOnRail = true;
        mIsRailPlusDir = isRailPlusDir(actor, mMoveDir);
        f32 railProgress = getRailCoord(actor) / getRailTotalLength(actor);
        mRailProgress = railProgress;
        mInitialRailProgress = railProgress;

        if (mIsRailPlusDir)
            mWheelAngle = railProgress * mMoveEndDegree;
        else
            mWheelAngle = -(railProgress * mMoveEndDegree);

        rotateQuatLocalDirDegree(&mActorQuat, mInitialActorQuat, (s32)mRotateAxis,
                                 modf(mWheelAngle + 360.0f, 360.0f) + 0.0f);
    }
}

bool WheelMovement::receiveMsg(LiveActor* actor, const SensorMsg* message, HitSensor* other,
                               HitSensor* self) {
    if (isMsgFloorTouch(message)) {
        sead::Vector3f pos;
        if (isMySensor(self, actor))
            pos = getSensorPos(other);
        else
            pos = getActorTrans(self);

        f32 width = normalizeAbs(mMoveDir.dot(pos - getTrans(actor)), mNoRotateWidth,
                                 mNoRotateWidth + 50.0f);
        mRotateWidth += isMsgEnemyFloorTouch(message) ? width * 0.9f : width;
        return true;
    }

    return false;
}

void WheelMovement::update(LiveActor* actor) {
    updateRotate();
    updateActorPoseAndTrans(actor);
}

void WheelMovement::updateRotate() {
    mRotateWidth = sead::Mathf::clamp(mRotateWidth, -1.25f, 1.25f);
    mDeltaAngle = (mRotateWidth * mRotateAccel * 0.001f + mNextDeltaAngle) * 0.97f;
    mWheelAngle = mWheelAngle + mDeltaAngle;

    mNextDeltaAngle = mDeltaAngle;
    mIsInvertDirection = false;
    if (mIsOnRail) {
        if (mIsRailPlusDir) {
            if (mWheelAngle < 0.0f) {
                mWheelAngle = 0.0f;
                if (mNextDeltaAngle < 0.0f) {
                    if (mNextDeltaAngle < mRotateAccel * -0.001f)
                        mIsInvertDirection = true;
                    else
                        mNextDeltaAngle = 0.0f;
                }
            }
            if (mWheelAngle > mMoveEndDegree) {
                mWheelAngle = mMoveEndDegree;
                if (mNextDeltaAngle > 0.0f) {
                    if (mNextDeltaAngle > mRotateAccel * 0.001f)
                        mIsInvertDirection = true;
                    else
                        mNextDeltaAngle = 0.0f;
                }
            }
            mRailProgress = mWheelAngle / mMoveEndDegree;
        } else {
            if (mWheelAngle < -mMoveEndDegree) {
                mWheelAngle = -mMoveEndDegree;
                if (mNextDeltaAngle < 0.0f) {
                    if (mNextDeltaAngle < mRotateAccel * -0.001f)
                        mIsInvertDirection = true;
                    else
                        mNextDeltaAngle = 0.0f;
                }
            }
            if (mWheelAngle > 0.0f) {
                mWheelAngle = 0.0f;
                if (mNextDeltaAngle > 0.0f) {
                    if (mNextDeltaAngle > mRotateAccel * 0.001f)
                        mIsInvertDirection = true;
                    else
                        mNextDeltaAngle = 0.0f;
                }
            }
            mRailProgress = -mWheelAngle / mMoveEndDegree;
        }
        if (mIsInvertDirection)
            mNextDeltaAngle *= -0.2f;
    } else {
        mWheelAngle = modf(mWheelAngle + 360.0f, 360.0f) + 0.0f;
    }
    mRotateWidth = 0.0f;
    rotateQuatLocalDirDegree(&mActorQuat, mInitialActorQuat, (s32)mRotateAxis,
                             modf(mWheelAngle + 360.0f, 360.0f) + 0.0f);
}

void WheelMovement::updateActorPoseAndTrans(LiveActor* actor) {
    setQuat(actor, mActorQuat);
    if (isExistRail(actor))
        setSyncRailToCoord(actor, mRailProgress * getRailTotalLength(actor));
}

void WheelMovement::reset() {
    mWheelAngle = 0.0f;
    mNextDeltaAngle = 0.0f;
    mRotateWidth = 0.0f;
    mIsInvertDirection = false;
    mRailProgress = mInitialRailProgress;
    rotateQuatLocalDirDegree(&mActorQuat, mInitialActorQuat, (s32)mRotateAxis,
                             modf(360.0f, 360.0f) + 0.0f);
}

void WheelMovement::reset(LiveActor* actor) {
    reset();
    updateActorPoseAndTrans(actor);
    resetPosition(actor);
}

}  // namespace al
