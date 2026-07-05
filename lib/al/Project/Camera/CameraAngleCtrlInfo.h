#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class ByamlIter;
struct CameraObjectRequestInfo;

class CameraAngleCtrlInfo {
public:
    CameraAngleCtrlInfo();

    static CameraAngleCtrlInfo* createWithRelativeH();

    void start(f32);
    void load(const ByamlIter& iter);
    void update(const sead::Vector2f& cameraStick, f32 sensitivityScale, bool isTriggerReset);

    s32 getMaxResetStep() const;
    bool isFixByRangeHV() const;
    bool isResetStartTiming() const;

    bool receiveRequestFromObject(const CameraObjectRequestInfo& info);
    void setAngleV(f32 angleV);

    void setDefaultAngleV(f32 min, f32 max) {
        mDefaultMinAngleV = min;
        mDefaultMaxAngleV = max;
    }

    void setStartAngleV(f32 angle) { mStartAngleV = angle; }

    f32 getAngleH() const { return mAngleH; }

    f32 getAngleV() const { return mAngleV; }

private:
    void* _0;  // object of size 0x14
    void* _8;  // object of size 0x10
    bool mIsValidRotateH;
    s32 _14;
    f32 mAngleH;
    f32 _1c;
    f32 mMinAngleH;
    f32 mMaxAngleH;
    f32 mAngleV;
    f32 mStartAngleV;
    f32 _30;
    f32 mDefaultMinAngleV;
    f32 mDefaultMaxAngleV;
    f32 _3c;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4c;
    bool mIsKeepPreAngleV;
    bool mIsSetResetAngleV;
    f32 mResetAngleV;
    bool mIsInvalidReceiveRequest;
};

static_assert(sizeof(CameraAngleCtrlInfo) == 0x60);

}  // namespace al
