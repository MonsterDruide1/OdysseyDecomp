#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class Shine;

class CageShineWatcher : public al::LiveActor {
public:
    CageShineWatcher(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initBySwitch();
    void initAfterPlacement() override;
    void syncShineGetCamera();
    void startBreakDemo();
    bool isSameShine(const Shine* shine) const;
    void exeShineWatch();
    void exeWaitCameraStart();
    void exeCameraInterpole();
    void exeBreak();
    void endBreakDemo();
    void exeWaitEntranceCameraStart();
    void exeLinkSwitchWatch();
    bool isSwitchOn() const;

private:
    char filler_108[0x78];
};

static_assert(sizeof(CageShineWatcher) == 0x180);
