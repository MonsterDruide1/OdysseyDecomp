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
};
}  // namespace al
