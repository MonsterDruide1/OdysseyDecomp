#pragma once

namespace al {
class SceneStopCtrl {
public:
    SceneStopCtrl();

    void update();
    void reqeustStopScene(s32 stopFrames, s32 delayFrames);

    bool isStop() { return mDelayFrames == 0 && mStopFrames > 0; }

private:
    s32 mStopFrames = -1;
    s32 mDelayFrames = 0;
};

static_assert(sizeof(SceneStopCtrl) == 0x8);
}  // namespace al
