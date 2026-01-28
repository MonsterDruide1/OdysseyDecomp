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

private:
    const char* _20;
    const char* _28;
    const char* _30;
    const char* _38;
    bool _40;
};

static_assert(sizeof(LoopAnimState) == 0x48);
