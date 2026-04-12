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

private:
    void* _0;
    void* _8[1];
    bool mIsValidRotateH;
    void* _18[1];
    f32 mMinAngleH;
    f32 mMaxAngleH;
    f32 _28;
    f32 mAngleV;
    f32 _30;
    f32 mMinAngleV;
    f32 mMaxAngleV;
    f32 _3c[5];
    bool mIsKeepPreAngleV;
    bool mIsSetResetAngleV;
    f32 mResetAngleV;
    bool mIsInvalidReceiveRequest;
};
}  // namespace al
