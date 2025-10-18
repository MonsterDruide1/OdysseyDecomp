#include "Library/Scene/SceneStopCtrl.h"

namespace al {
SceneStopCtrl::SceneStopCtrl() = default;

void SceneStopCtrl::reqeustStopScene(s32 stopFrames, s32 delayFrames) {
    if (mStopFrames <= 0 && mDelayFrames <= 0)
        mDelayFrames = delayFrames;
    if (mStopFrames < stopFrames)
        mStopFrames = stopFrames;
}

void SceneStopCtrl::update() {
    if (mDelayFrames >= 1)
        mDelayFrames--;
    else if (mStopFrames >= 1)
        mStopFrames--;
}
}  // namespace al
