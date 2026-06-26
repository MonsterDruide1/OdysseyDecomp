#include "Player/HackCapStateHide.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/HackCap.h"

namespace {
NERVE_IMPL(HackCapStateHide, Hide);

NERVES_MAKE_NOSTRUCT(HackCapStateHide, Hide);
}  // namespace

HackCapStateHide::HackCapStateHide(HackCap* hackCap)
    : al::ActorStateBase("キャップ非表示", hackCap), mHackCap(hackCap) {
    initNerve(&Hide, 0);
}

void HackCapStateHide::appear() {
    al::ActorStateBase::appear();
    al::setNerve(this, &Hide);
}

void HackCapStateHide::exeHide() {
    // NON_MATCHING
}
