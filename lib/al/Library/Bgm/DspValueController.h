#pragma once

#include <basis/seadTypes.h>

namespace al {
class DspLinearValueController {
public:
    DspLinearValueController(f32 value);
    void init(f32 value);
    void update();
    void changeTarget(f32 target, s32 stepCount);

    bool isReachedTarget() const { return mTarget == mCurrent; }

    f32 getCurrent() const { return mCurrent; }

private:
    f32 mCurrent = 0.0f;
    f32 mTarget = 0.0f;
    f32 mStep = 0.0f;
};

static_assert(sizeof(DspLinearValueController) == 0xc);

class DspSinValueController {
public:
    DspSinValueController(f32 shift, f32 amplitude);
    void init(f32);
    void update();
    void changeTarget(f32, s32);
    void changeFreq(f32);

    bool isReachedTarget() const { return mLinearValue->isReachedTarget(); }

    f32 getAmplitude() const { return mAmplitude; }

private:
    f32 mShift;
    f32 mFreq;
    f32 mDelta;
    f32 mAngle;
    f32 mAmplitude;
    DspLinearValueController* mLinearValue;
};

static_assert(sizeof(DspSinValueController) == 0x20);

}  // namespace al
