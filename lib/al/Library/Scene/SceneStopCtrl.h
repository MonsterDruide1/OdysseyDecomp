#pragma once

#include <basis/seadTypes.h>

namespace al {
class SceneStopCtrl {
public:
    SceneStopCtrl();
    void reqeustStopScene(s32 stopForFrames, s32 updateForFrames);
    void update();

private:
    s32 mStopFrames = -1;
    s32 mUpdateFrames = 0;
};
}  // namespace al
