#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {

class JointAimInfo {
public:
    JointAimInfo();
    void makeTurnQuat(sead::Quatf* quat, const sead::Vector3f& targetDir) const;
    void makeTurnQuatCircle(sead::Quatf* quat, const sead::Vector3f& targetDir) const;
    void makeTurnQuatOval(sead::Quatf* quat, const sead::Vector3f& targetDir) const;
    void makeTurnQuatRect(sead::Quatf* quat, const sead::Vector3f& targetDir) const;
    void setBaseAimLocalDir(const sead::Vector3f& dir);
    void setBaseUpLocalDir(const sead::Vector3f& dir);
    void setBaseSideLocalDir(const sead::Vector3f& dir);
    void setBaseOffsetLocal(const sead::Vector3f& offset);
    void setBaseMtxPtr(const sead::Matrix34f* mtxPtr);
    void setTargetPos(const sead::Vector3f& pos);
    void setPowerRate(f32 rate);
    void setLimitDegreeCircle(f32 degree);
    void setLimitDegreeOval(f32 a, f32 b, f32 c, f32 d);
    void setLimitDegreeRect(f32 a, f32 b, f32 c, f32 d);
    void setEnableBackAim(bool enable);
    void addPowerRate(f32 rate);
    void subPowerRate(f32 rate);
    void setInterpoleRate(f32 rate);

private:
    sead::Vector3f mTargetPos = sead::Vector3f::zero;
    sead::Vector3f mBaseAimLocalDir = sead::Vector3f::ez;
    sead::Vector3f mBaseSideLocalDir = sead::Vector3f::ex;
    sead::Vector3f mBaseUpLocalDir = sead::Vector3f::ey;
    sead::Vector3f mBaseOffsetLocal = sead::Vector3f::zero;
    const sead::Matrix34f* mBaseMtxPtr = nullptr;
    f32 mPowerRate = 1.0f;
    f32 mInterpoleRate = 0.1f;
    f32 _50 = 30.0f;
    f32 _54 = 30.0f;
    f32 _58 = 30.0f;
    f32 _5c = 30.0f;
    s32 _60 = 0;
    bool mIsEnableBackAim = false;
};

static_assert(sizeof(JointAimInfo) == 0x68);

}  // namespace al
