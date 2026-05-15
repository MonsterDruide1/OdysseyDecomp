#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class WaterfallWorldFallDownBridge : public al::LiveActor {
public:
    WaterfallWorldFallDownBridge(const char*);

    void init(const al::ActorInitInfo&) override;

    void startDemo();
    bool isEndDemo() const;
    void fallDown();
    void exeWait();
    void exeWaitCameraInterpolate();
    void exeDemo();
    void exeDemoAfter();
    void exeFallDown();
};
