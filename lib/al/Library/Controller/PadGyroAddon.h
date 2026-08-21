#pragma once

#include <basis/seadTypes.h>
#include <controller/seadControllerAddon.h>
#include <math/seadVector.h>

namespace al {
class PadGyroAddon : public sead::ControllerAddon {
    SEAD_RTTI_OVERRIDE(PadGyroAddon, ControllerAddon)
public:
    PadGyroAddon(sead::Controller* controller, s32 index);

    bool calc() override;

    bool tryUpdateGyroStatus();
    void getPose(sead::Vector3f* outSide, sead::Vector3f* outUp, sead::Vector3f* outFront) const;

private:
    bool mGyroStatus = false;
    sead::Vector3f mSide = sead::Vector3f::ex;
    sead::Vector3f mUp = sead::Vector3f::ey;
    sead::Vector3f mFront = sead::Vector3f::ez;
    sead::Vector3f mAngularVelocity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mAngle = {0.0f, 0.0f, 0.0f};
    s64 mPrevSamplingNumber = 0;
    s32 mSampleCount = 0;
    s32 mIndex;
};

static_assert(sizeof(PadGyroAddon) == 0x78);
}  // namespace al
