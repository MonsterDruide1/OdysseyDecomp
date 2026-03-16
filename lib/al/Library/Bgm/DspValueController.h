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
    DspSinValueController(f32 updateFreq, f32 value);
    void init(f32 value);
    void update();
    void changeTarget(f32 target, s32 stepCount);
    void changeFreq(f32 frequency);

    bool isReachedTarget() const { return mAmplitude->isReachedTarget(); }

    f32 getCurrent() const { return mCurrent; }

private:
    f32 mUpdateFreq = 0.0f;
    f32 mFreq = 0.0f;
    f32 mDelta = 0.0f;
    f32 mAngle = 0.0f;
    f32 mCurrent = 0.0f;
    DspLinearValueController* mAmplitude;
};

static_assert(sizeof(DspSinValueController) == 0x20);

}  // namespace al
