#pragma once

#include "Library/Nerve/NerveStateBase.h"

class GolemClimbWeakPoint;
class GolemShoutState;
class IUseGolemState;

class GolemDownBreakState : public al::HostStateBase<IUseGolemState> {
public:
    GolemDownBreakState(const char* name, IUseGolemState* golemState, GolemShoutState* shoutState);
    void appear() override;
    void kill() override;
    void exeDamage();
    void exeShout();
    void exeRecover();
    void startBreak(GolemClimbWeakPoint* weakPoint, const char* damageActionName,
                    const char* recoverActionName);

private:
    GolemShoutState* mShoutState = nullptr;
    GolemClimbWeakPoint* mWeakPoint = nullptr;
    const char* mDamageActionName = nullptr;
    const char* mRecoverActionName = nullptr;
};

static_assert(sizeof(GolemDownBreakState) == 0x40);
