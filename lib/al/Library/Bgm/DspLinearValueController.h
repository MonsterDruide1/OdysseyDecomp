#pragma once

#include <basis/seadTypes.h>

namespace al {
class DspLinearValueController {
public:
    DspLinearValueController(f32);
    void init(f32);
    void update();
    void changeTarget(f32, s32);

    bool isReachedTarget() const { return mTarget == mCurrent; }

    f32 getCurrent() const { return mCurrent; }

private:
    f32 mCurrent;
    f32 mTarget;
    f32 mStep;
};

static_assert(sizeof(DspLinearValueController) == 0xc);

}  // namespace al
