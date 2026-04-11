#pragma once

#include "Library/LiveActor/LiveActor.h"

class DoorCity : public al::LiveActor {
public:
    DoorCity(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void onStageSwitch();
    void listenSwitchOff();
    void exeWaitOpen();
    void exeOpen();
    void exeWaitClose();
};

static_assert(sizeof(DoorCity) == 0x108);
