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
    char* _20;
    char* _28;
    char* _30;
    char* _38;
    bool _40;
};

static_assert(sizeof(LoopAnimState) == 0x48);
