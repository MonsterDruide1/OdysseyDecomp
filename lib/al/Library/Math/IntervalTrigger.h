#pragma once

#include <basis/seadTypes.h>

namespace al {
class IntervalTrigger {
public:
    IntervalTrigger(f32 intervalTime);
    void update(f32 deltaTime);

    bool isTriggered() const { return mIsTriggered; }

private:
    f32 mIntervalTime = 0.0f;
    f32 mTime = 0.0f;
    bool mIsTriggered = false;
};
}  // namespace al
