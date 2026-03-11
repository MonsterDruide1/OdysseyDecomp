#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class LoopAnimState : public al::ActorStateBase {
public:
    LoopAnimState(al::LiveActor* actor, const char* name, bool isSyncSubActor);
    void appear() override;
    void end();
    void exeStart();
    void startAction(const char* actionName);
    void exeLoop();
    void exeEnd();

private:
    const char* mName = nullptr;
    const char* mStartSuffix = "Start";
    const char* mLoopSuffix = "Loop";
    const char* mEndSuffix = "End";
    bool mIsSyncSubActor = false;
};

static_assert(sizeof(LoopAnimState) == 0x48);
