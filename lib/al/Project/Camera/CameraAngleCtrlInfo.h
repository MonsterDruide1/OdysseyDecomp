#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class ByamlIter;
struct CameraObjectRequestInfo;

class CameraAngleCtrlInfo {
public:
    CameraAngleCtrlInfo();

    void start(f32);
    void load(const ByamlIter& iter);
    void update(const sead::Vector2f, f32, bool);

    f32 getMaxResetStep();
    bool isFixByRangeHV();
    bool isResetStartTiming();

    bool recieveRequestFromObject(const CameraObjectRequestInfo& info);
    void createWithRelativeH();
    void setAngleV(f32);
};
}  // namespace al
