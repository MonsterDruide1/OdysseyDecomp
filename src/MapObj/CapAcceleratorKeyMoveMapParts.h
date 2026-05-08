#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class KeyPoseKeeper;

class CapAcceleratorKeyMoveMapParts : public LiveActor {
public:
    CapAcceleratorKeyMoveMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void control() override;

    void exeWait();
    void exeMove();
    void exeStop();

private:
    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mKeyMoveWaitTime = 30;
    s32 mKeyMoveMoveTime = 0;
};

static_assert(sizeof(CapAcceleratorKeyMoveMapParts) == 0x118);
}  // namespace al
