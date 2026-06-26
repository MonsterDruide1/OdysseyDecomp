#include "Player/HackCapStateThrowStay.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/HackCap.h"

namespace {
NERVE_IMPL(HackCapStateThrowStay, Wait);
NERVE_IMPL(HackCapStateThrowStay, Stay);
NERVE_IMPL(HackCapStateThrowStay, End);

NERVES_MAKE_NOSTRUCT(HackCapStateThrowStay, Wait, Stay, End);
}  // namespace

HackCapStateThrowStay::HackCapStateThrowStay(HackCap* hackCap)
    : al::ActorStateBase("キャップ停滞", hackCap), mHackCap(hackCap) {
    initNerve(&Wait, 0);
}

void HackCapStateThrowStay::appear() {
    al::ActorStateBase::appear();
    al::setNerve(this, &Wait);
}

bool HackCapStateThrowStay::isHomingPlayerJump() const {
    return true;
}

void HackCapStateThrowStay::exeWait() {
    // NON_MATCHING
}

void HackCapStateThrowStay::exeStay() {
    // NON_MATCHING
}

void HackCapStateThrowStay::exeEnd() {
    // NON_MATCHING
}
