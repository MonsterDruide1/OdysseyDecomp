#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerStateWait : public al::ActorStateBase {
public:
    bool isEnableCancelAction() const;
    bool isSleep() const;
    bool requestAnimName(const char* name);

    void exeWait();
    void exeLandStiffen();
    void exeLand();
    void exeWaitConnect2D();
    void exeWaitSnoozeStart();
    void exeWaitSnooze();
    void exeWaitSleepStart();
    void exeWaitSleep();
    void exeAreaAnimWait();
    void exeWaitRelaxStart();
    void exeWaitRelax();
    void exeAreaAnimTurn();
    void exeRequestAnimWait();

private:
    u8 _20[0x98];
    const char* mRequestAnimName = nullptr;
    u8 _c0[0x28];
};
