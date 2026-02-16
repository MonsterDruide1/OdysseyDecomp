#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerCollision;
class CapTargetInfoFunction;

class CapTargetInfo {
public:
    CapTargetInfo();

    void init(const al::LiveActor* actor, const char* name);
    void setFollowLockOnMtx(const char* jointName, const sead::Vector3f& localTrans,
                            const sead::Vector3f& localRotate);
    void setLockOnStartAnimName(const char* animName);
    void setLockOnAnimName(const char* animName);
    void setHackName(const char* hackName);
    void makeLockOnMtx(sead::Matrix34f* outMtx) const;
    void calcLockOnFollowTargetScale(sead::Vector3f* targetScale) const;

    void setPoseMatrix(sead::Matrix34f* mtx) { mPoseMatrix = mtx; }

private:
    friend CapTargetInfoFunction;
    const al::LiveActor* mActor = nullptr;
    const char* mHackName = nullptr;
    IUsePlayerCollision* mPlayerCollision = nullptr;
    sead::Matrix34f* mPoseMatrix = nullptr;
    const sead::Matrix34f* mJointMtx = nullptr;
    sead::Vector3f mLocalTrans = sead::Vector3f::zero;
    sead::Vector3f mLocalRotate = sead::Vector3f::zero;
    f32 mLockOnScale = 1.0f;
    bool mIsUseLockOnFollowMtxScale = false;
    bool mIsUseFollowMtxScaleLocalOffset = false;
    const char* mLockOnStartAnimName = "Capture";
    const char* mLockOnAnimName = "Capture";
    bool mIsEscapeLocalOffset = false;
    sead::Vector3f mEscapeLocalOffset = {0.0f, 0.0f, 0.0f};
    const char* mName = nullptr;
    bool mIsExistModel = false;
    bool mIsLockOnOnly = false;
    bool _72 = false;
    bool mIsUseDepthShadow = false;
    bool _74 = false;
    bool mIsLockOn = true;
    bool mIsLockOnStart = false;
    bool mIsSetHackNameToCamera = false;
    bool _78 = false;
    bool _79 = false;
    bool mIsInvalidHackThrow = false;
    bool mIsInvalidCapEye = false;
    bool _7c = false;
    bool _7d = false;
    bool _7e = false;
    bool _7f = false;
};

class CapTargetInfoFunction {
public:
    static void initIterCapTargetInfo(CapTargetInfo* capTargetInfo, IUsePlayerCollision*,
                                      const al::LiveActor* actor, const char* name);
};
