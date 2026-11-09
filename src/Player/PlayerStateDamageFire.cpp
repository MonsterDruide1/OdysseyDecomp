#include "Player/PlayerStateDamageFire.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(PlayerStateDamageFire, Jump);
NERVE_IMPL(PlayerStateDamageFire, Dead);
NERVE_IMPL(PlayerStateDamageFire, Run);
NERVE_IMPL(PlayerStateDamageFire, Fall);
NERVE_IMPL(PlayerStateDamageFire, Jump2nd);

// __attribute__((used)): the internal-linkage nerve classes and their
// execute()s would otherwise be discarded if nothing referenced the Nrv
// struct.
struct {
    PlayerStateDamageFireNrvJump Jump;
    PlayerStateDamageFireNrvDead Dead;
    PlayerStateDamageFireNrvRun Run;
    PlayerStateDamageFireNrvFall Fall;
    PlayerStateDamageFireNrvJump2nd Jump2nd;
} NrvPlayerStateDamageFire __attribute__((used));
}  // namespace

bool PlayerStateDamageFire::isEndFirstLand() const {
    return !al::isNerve(this, &NrvPlayerStateDamageFire.Jump);
}

bool PlayerStateDamageFire::isEnableJump() const {
    return al::isNerve(this, &NrvPlayerStateDamageFire.Run);
}

s32 PlayerStateDamageFire::getEnableCancelCollisionSnapFrame() const {
    return 20;
}
