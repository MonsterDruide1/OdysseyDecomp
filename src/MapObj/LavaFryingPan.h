#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class ClockMovement;
class RippleCtrl;
}  // namespace al

class LavaFryingPan : public al::LiveActor {
public:
    LavaFryingPan(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void exeDelay();
    void exeRotateSign();
    void exeRotate();
    void exeWaitUp();
    void exeWaitDown();

private:
    al::ClockMovement* mClockMovement = nullptr;
    al::RippleCtrl* mRippleCtrl = nullptr;
};

static_assert(sizeof(LavaFryingPan) == 0x118);
