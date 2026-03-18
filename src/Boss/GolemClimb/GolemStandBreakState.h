#pragma once

#include "Library/Nerve/NerveStateBase.h"

class GolemClimbWeakPoint;
class GolemShoutState;
class IUseGolemState;

class GolemStandBreakState : public al::HostStateBase<IUseGolemState> {
public:
    GolemStandBreakState(const char* name, IUseGolemState* golemState, GolemShoutState* shoutState);
    void appear() override;
    void kill() override;
    void exeDamage();
    void exeShout();
    void exeRecover();
    void startBreak(GolemClimbWeakPoint* weakPoint);

private:
    GolemShoutState* mShoutState = nullptr;
    GolemClimbWeakPoint* mWeakPoint = nullptr;
};

static_assert(sizeof(GolemStandBreakState) == 0x30);
