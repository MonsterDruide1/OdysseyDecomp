#pragma once

namespace al {
class LiveActor;

class FlowMapCtrl {
public:
    FlowMapCtrl(LiveActor* parentActor);
    void update();

    void init(s32 interval, f32 speed) {
        mInterval = interval;
        mSpeed = speed;
    }

private:
    LiveActor* mParent = nullptr;
    s32 mFlowStep = 0;
    s32 mInterval = 60;
    f32 mSpeed = 1.0f;
};
}  // namespace al
