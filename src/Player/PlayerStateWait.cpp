#include "Player/PlayerStateWait.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(PlayerStateWait, Wait);
NERVE_IMPL(PlayerStateWait, LandStiffen);
NERVE_IMPL(PlayerStateWait, Land);
NERVE_IMPL(PlayerStateWait, WaitConnect2D);
NERVE_IMPL(PlayerStateWait, WaitSnoozeStart);
NERVE_IMPL(PlayerStateWait, WaitSnooze);
NERVE_IMPL(PlayerStateWait, WaitSleepStart);
NERVE_IMPL(PlayerStateWait, WaitSleep);
NERVE_IMPL(PlayerStateWait, AreaAnimWait);
NERVE_IMPL(PlayerStateWait, WaitRelaxStart);
NERVE_IMPL(PlayerStateWait, WaitRelax);
NERVE_IMPL(PlayerStateWait, AreaAnimTurn);
NERVE_IMPL(PlayerStateWait, RequestAnimWait);

// __attribute__((used)): the internal-linkage nerve classes and their
// execute()s would otherwise be discarded if nothing referenced the Nrv
// struct.
struct {
    PlayerStateWaitNrvWait Wait;
    PlayerStateWaitNrvLandStiffen LandStiffen;
    PlayerStateWaitNrvLand Land;
    PlayerStateWaitNrvWaitConnect2D WaitConnect2D;
    PlayerStateWaitNrvWaitSnoozeStart WaitSnoozeStart;
    PlayerStateWaitNrvWaitSnooze WaitSnooze;
    PlayerStateWaitNrvWaitSleepStart WaitSleepStart;
    PlayerStateWaitNrvWaitSleep WaitSleep;
    PlayerStateWaitNrvAreaAnimWait AreaAnimWait;
    PlayerStateWaitNrvWaitRelaxStart WaitRelaxStart;
    PlayerStateWaitNrvWaitRelax WaitRelax;
    PlayerStateWaitNrvAreaAnimTurn AreaAnimTurn;
    PlayerStateWaitNrvRequestAnimWait RequestAnimWait;
} NrvPlayerStateWait __attribute__((used));
}  // namespace

bool PlayerStateWait::requestAnimName(const char* name) {
    if (mRequestAnimName)
        return false;
    mRequestAnimName = name;
    return true;
}
