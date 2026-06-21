#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SePlayObj : public LiveActor {
public:
    SePlayObj(const char* name);
    void init(const ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void makeActorAlive() override;
    void kill() override;
    void movement() override;
    void waitSwitch();
    void startPlaying();
    bool isPlaying();
    void exeWaitSwitch();
    void exePlay();
    bool tryPlayStart();
    void exePlayRepeat();

private:
    char filler[0x98];
};

static_assert(sizeof(SePlayObj) == 0x1a0);
}  // namespace al
