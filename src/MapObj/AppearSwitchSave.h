#pragma once

#include "Library/LiveActor/LiveActor.h"

class AppearSwitchSave {
public:
    AppearSwitchSave(al::LiveActor* actor, const al::ActorInitInfo& info);

    void onSwitch();
    void onSwitchDemo();
    bool isOn() const;

private:
    void* filler[10];
};
