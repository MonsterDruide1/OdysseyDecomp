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
    void* _0;  // object of size 0x14
    void* _8;  // object of size 0x10
    bool mIsValidRotateH;
    s32 _14;
    f32 _18;
    f32 _1c;
    f32 mMinAngleH;
    f32 mMaxAngleH;
    f32 _28;
    f32 mAngleV;
    f32 _30;
    f32 mMinAngleV;
    f32 mMaxAngleV;
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
