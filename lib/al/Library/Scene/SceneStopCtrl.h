#pragma once

#include <basis/seadTypes.h>

namespace al {
class SceneStopCtrl {
public:
    SceneStopCtrl();
    void reqeustStopScene(s32 stopFrames, s32 delayFrames);
    void update();

private:
    s32 mStopFrames = -1;
    s32 mDelayFrames = 0;
};
}  // namespace al
