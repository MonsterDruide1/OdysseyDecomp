#include "Library/Scene/SceneStopCtrl.h"

namespace al {
SceneStopCtrl::SceneStopCtrl() {}

void SceneStopCtrl::reqeustStopScene(s32 stopForFrames, s32 updateForFrames) {
    if (mStopFrames <= 0 && mUpdateFrames <= 0)
        mUpdateFrames = updateForFrames;
    if (mStopFrames < stopForFrames)
        mStopFrames = stopForFrames;
}

void SceneStopCtrl::update() {
    if (mUpdateFrames >= 1)
        mUpdateFrames--;
    else if (mStopFrames >= 1)
        mStopFrames--;
}
}  // namespace al
