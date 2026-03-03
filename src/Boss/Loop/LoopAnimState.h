#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class LoopAnimState : public al::ActorStateBase {
public:
    LoopAnimState(al::LiveActor*, const char*, bool);
    void appear() override;
    void end();
    void exeStart();
    void startAction(const char*);
    void exeLoop();
    void exeEnd();
    ~LoopAnimState() override;

private:
    const char* mName = nullptr;
    const char* mStartSuffix = nullptr;
    const char* mLoopSuffix = nullptr;
    const char* mEndSuffix = nullptr;
    bool mIsSyncSubActor = false;
};

static_assert(sizeof(LoopAnimState) == 0x48);
