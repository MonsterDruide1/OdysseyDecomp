#pragma once

#include <basis/seadTypes.h>

#include "Library/Bgm/DspLinearValueController.h"

namespace al {
class DspSinValueController {
public:
    DspSinValueController(f32, f32);
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
