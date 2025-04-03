#include "Player/CapTargetInfo.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"

CapTargetInfo::CapTargetInfo() {}

void CapTargetInfo::init(const al::LiveActor* actor, const char* name) {
    mActor = actor;
    mIsExistModel = al::isExistModel(actor);
    mHackName = nullptr;
    mPlayerCollision = nullptr;
    mPoseMatrix = nullptr;
    mJointMtx = nullptr;
    mLocalTrans = {0.0f, 0.0f, 0.0f};
    mLocalRotate = {0.0f, 0.0f, 0.0f};
    mLockOnScale = 1.0f;
    mIsUseLockOnFollowMtxScale = false;
    mLockOnAnimName = "Capture";
    mIsEscapeLocalOffset = false;
    mEscapeLocalOffset = {0.0f, 0.0f, 0.0f};
    mName = name;
    _72 = false;
    mIsUseDepthShadow = false;
    _74 = false;
    mIsLockOn = true;
    mIsLockOnStart = false;
    mIsSetHackNameToCamera = false;
    _78 = false;
    _79 = false;
    mIsInvalidHackThrow = false;
    mIsInvalidCapEye = false;
    _7c = false;
    _7d = false;
    _7e = false;
}

void CapTargetInfo::setFollowLockOnMtx(const char* jointName, const sead::Vector3f& localTrans,
                                       const sead::Vector3f& localRotate) {
    if (jointName)
        mJointMtx = al::getJointMtxPtr(mActor, jointName);
    else
        mJointMtx = mActor->getBaseMtx();

    mLocalTrans = localTrans;
    mLocalRotate = localRotate;
}

void CapTargetInfo::setLockOnStartAnimName(const char* animName) {
    mLockOnStartAnimName = animName;
    mIsLockOnStart = !al::isEqualString(animName, "Capture");
    mIsLockOn =
        al::isEqualString(animName, "Capture") && al::isEqualString(mLockOnAnimName, "Capture");
}

void CapTargetInfo::setLockOnAnimName(const char* animName) {
    mLockOnAnimName = animName;
    mIsLockOn = al::isEqualString(mLockOnAnimName, "Capture") &&
                al::isEqualString(mLockOnStartAnimName, "Capture");
}

void CapTargetInfo::setHackName(const char* hackName) {
    mHackName = hackName;
}

void CapTargetInfo::makeLockOnMtx(sead::Matrix34f* outMtx) const {
    if (mPoseMatrix) {
        *outMtx = *mPoseMatrix;
        return;
    }

    if (!mJointMtx) {
        al::makeMtxRT(outMtx, mActor);
        return;
    }

    sead::Matrix34f rotationMatrix;
    sead::Vector3f rotate(sead::Mathf::deg2rad(mLocalRotate.x),
                          sead::Mathf::deg2rad(mLocalRotate.y),
                          sead::Mathf::deg2rad(mLocalRotate.z));
    rotationMatrix.makeR(rotate);

    sead::Matrix34f translationMatrix;
    translationMatrix.makeRT({0.0f, 0.0f, 0.0f}, mLocalTrans * al::getScaleY(mActor));

    sead::Matrix34f poseMatrix = rotationMatrix * translationMatrix;

    if (mIsUseFollowScaleLocalOffset) {
        sead::Matrix34f baseMtx = sead::Matrix34f::ident;
        baseMtx = *mJointMtx * rotationMatrix;

        sead::Vector3f mtxScale = {0.0f, 0.0f, 0.0f};
        al::calcMtxScale(&mtxScale, baseMtx);
        poseMatrix.m[0][3] *= mtxScale.x;
        poseMatrix.m[1][3] *= mtxScale.y;
        poseMatrix.m[2][3] *= mtxScale.z;
    }

    sead::Matrix34f baseMtx = *mJointMtx;
    al::normalize(&baseMtx);
    *outMtx = baseMtx * poseMatrix;
}

void CapTargetInfo::calcLockOnFollowTargetScale(sead::Vector3f* targetScale) const {
    if (!mIsUseLockOnFollowMtxScale) {
        *targetScale = {mLockOnScale, mLockOnScale, mLockOnScale};
        return;
    }

    if (mPoseMatrix) {
        al::calcMtxScale(targetScale, *mPoseMatrix);
        *targetScale *= mLockOnScale;
        return;
    }

    sead::Matrix34f baseMtx = sead::Matrix34f::ident;
    if (mJointMtx)
        baseMtx = *mJointMtx;
    else
        al::makeMtxRT(&baseMtx, mActor);

    sead::Matrix34f rotationMatrix;
    sead::Vector3f rotate(sead::Mathf::deg2rad(mLocalRotate.x),
                          sead::Mathf::deg2rad(mLocalRotate.y),
                          sead::Mathf::deg2rad(mLocalRotate.z));
    rotationMatrix.makeR(rotate);

    al::calcMtxScale(targetScale, baseMtx * rotationMatrix);
    *targetScale *= mLockOnScale;
}
