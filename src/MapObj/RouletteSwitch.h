#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

class HipDropSwitch;
class TrampleSwitch;

class RouletteSwitch : public al::LiveActor {
public:
    RouletteSwitch(const char* name);
    void init(const al::ActorInitInfo& actorInitInfo) override;
    void control() override;
    void exeWait();

private:
    sead::FixedPtrArray<TrampleSwitch, 3> mButtons;
    HipDropSwitch* mResetButton = nullptr;
    bool mIsPressReset = false;
};

static_assert(sizeof(RouletteSwitch) == 0x140);
