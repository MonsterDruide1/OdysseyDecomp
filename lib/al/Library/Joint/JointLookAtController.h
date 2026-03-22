#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Joint/JointControllerBase.h"

namespace al {

class JointLookAtParam {
public:
    JointLookAtParam();
    JointLookAtParam(s32 jointIndex, f32 rate, const sead::Vector2f& limitAngle1,
                     const sead::Vector2f& limitAngle2, const sead::Vector3f& localFront,
                     const sead::Vector3f& localUp);

private:
    s32 mJointIndex;
    f32 mRate;
    sead::Vector2f mLimitAngle1;
    sead::Vector2f mLimitAngle2;
    sead::Vector3f mLocalFront;
    sead::Vector3f mLocalUp;
    sead::Vector3f mLocalRight;
};

static_assert(sizeof(JointLookAtParam) == 0x3c);

class JointLookAtInfo {
private:
    JointLookAtParam* mParam;
    sead::Quatf _08;
    sead::Quatf _18;
    bool _28;
    bool mIsInvalid;
    bool mIsNoJudge;
    bool mIsNoOverLimitYaw;
    const sead::Matrix34f* _30;
    sead::Vector3f _38;
    sead::Vector3f _44;
    sead::Vector3f _50;
    sead::Vector3f _5c;
};

static_assert(sizeof(JointLookAtInfo) == 0x68);

class JointLookAtController : public JointControllerBase {
public:
    JointLookAtController(s32 jointInfoNumMax, const sead::Matrix34f* defaultMtx);

    void calcJointCallback(s32 jointIndex, sead::Matrix34f* mtx) override;
    void appendJoint(JointLookAtInfo* info);
    void requestJointLookAt(const sead::Vector3f& targetTrans);
    bool invalidJoint(s32 jointIndex);
    void validAllJoint();
    const char* getCtrlTypeName() const override;

    void setIsFlipped(bool isFlipped) { mIsFlipped = isFlipped; }

private:
    sead::Vector3f mTargetTrans;
    const sead::Matrix34f* mDefaultMtx;
    s32 mJointInfoNumMax;
    s32 mJointInfoNum;
    JointLookAtInfo** mJointInfos;
    bool mHasRequest;
    bool _51;
    bool mIsFlipped;
    bool mCanRequestLookAt;
    bool _54;
};

static_assert(sizeof(JointLookAtController) == 0x58);

}  // namespace al
