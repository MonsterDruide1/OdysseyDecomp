#include "Player/PlayerCapManHeroEyesControl.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(PlayerCapManHeroEyesControl, Dead)
NERVE_IMPL(PlayerCapManHeroEyesControl, Appear)
NERVE_IMPL(PlayerCapManHeroEyesControl, Wait)
NERVE_IMPL(PlayerCapManHeroEyesControl, Disappear)
NERVE_IMPL(PlayerCapManHeroEyesControl, Demo)

NERVES_MAKE_NOSTRUCT(PlayerCapManHeroEyesControl, Dead, Disappear, Demo, Appear, Wait)
}  // namespace

PlayerCapManHeroEyesControl::PlayerCapManHeroEyesControl(const char* name, al::LiveActor* capEye,
                                                         al::LiveActor* capEye2D)
    : al::NerveExecutor(name), mCapEye(capEye), mCapEye2D(capEye2D) {
    mCapEye->kill();
    mCapEye2D->kill();
    initNerve(&Dead, 0);
}

void PlayerCapManHeroEyesControl::start() {
    if (!mCapEye)
        return;

    if (al::isDead(mCapEye))
        mCapEye->appear();

    al::setNerve(this, &Appear);
}

void PlayerCapManHeroEyesControl::fastStart() {
    if (!mCapEye)
        return;

    if (al::isDead(mCapEye))
        mCapEye->appear();

    al::startAction(mCapEye, "Appear");
    al::setActionFrame(mCapEye, al::getActionFrameMax(mCapEye, "Appear"));
    al::setNerve(this, &Wait);
}

void PlayerCapManHeroEyesControl::end() {
    if (mCapEye)
        al::setNerve(this, &Disappear);
}

void PlayerCapManHeroEyesControl::startPuppet() {
    if (mCapEye)
        mCapEye->kill();

    if (mCapEye2D)
        mCapEye2D->kill();

    al::setNerve(this, &Demo);
}

void PlayerCapManHeroEyesControl::endPuppet() {
    // BUG: does not kill `mCapEye2D`
    if (mCapEye)
        mCapEye->kill();

    kill();
}

void PlayerCapManHeroEyesControl::kill() {
    if (!mCapEye)
        return;

    mCapEye->kill();
    al::setNerve(this, &Dead);
}

void PlayerCapManHeroEyesControl::update() {
    updateNerve();
}

al::LiveActor* PlayerCapManHeroEyesControl::getPuppetEye() const {
    return mCapEye;
}

bool PlayerCapManHeroEyesControl::isAppear() const {
    return al::isNerve(this, &Appear) || al::isNerve(this, &Wait);
}

bool PlayerCapManHeroEyesControl::isDisappear() const {
    if (isAppear())
        return false;
    return true;
}

bool PlayerCapManHeroEyesControl::isDemo() const {
    return al::isNerve(this, &Demo);
}

bool PlayerCapManHeroEyesControl::isDeadCap() const {
    return al::isNerve(this, &Dead);
}

void PlayerCapManHeroEyesControl::requestWaitAnimChange(const char* actionName) {
    if (al::isNerve(this, &Wait) && al::isAlive(mCapEye))
        al::tryStartActionIfNotPlaying(mCapEye, actionName);
}

void PlayerCapManHeroEyesControl::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(mCapEye, "Appear");

    if (al::isActionEnd(mCapEye))
        al::setNerve(this, &Wait);
}

void PlayerCapManHeroEyesControl::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(mCapEye, "Wait");
}

void PlayerCapManHeroEyesControl::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(mCapEye, "Disappear");

    if (al::isActionEnd(mCapEye))
        al::setNerve(this, &Dead);
}

void PlayerCapManHeroEyesControl::exeDead() {
    if (mCapEye)
        mCapEye->kill();
}

void PlayerCapManHeroEyesControl::exeDemo() {}
