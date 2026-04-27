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

PlayerCapManHeroEyesControl::PlayerCapManHeroEyesControl(const char* name, al::LiveActor* puppetEye,
                                                         al::LiveActor* normal2DEye)
    : al::NerveExecutor(name), mPuppetEye(puppetEye), mNormal2DEye(normal2DEye) {
    mPuppetEye->kill();
    mNormal2DEye->kill();
    initNerve(&Dead, 0);
}

void PlayerCapManHeroEyesControl::start() {
    if (!mPuppetEye)
        return;

    if (al::isDead(mPuppetEye))
        mPuppetEye->appear();

    al::setNerve(this, &Appear);
}

void PlayerCapManHeroEyesControl::fastStart() {
    if (!mPuppetEye)
        return;

    if (al::isDead(mPuppetEye))
        mPuppetEye->appear();

    al::startAction(mPuppetEye, "Appear");
    al::setActionFrame(mPuppetEye, al::getActionFrameMax(mPuppetEye, "Appear"));
    al::setNerve(this, &Wait);
}

void PlayerCapManHeroEyesControl::end() {
    if (mPuppetEye)
        al::setNerve(this, &Disappear);
}

void PlayerCapManHeroEyesControl::startPuppet() {
    if (mPuppetEye)
        mPuppetEye->kill();

    if (mNormal2DEye)
        mNormal2DEye->kill();

    al::setNerve(this, &Demo);
}

void PlayerCapManHeroEyesControl::endPuppet() {
    if (!mPuppetEye)
        return;

    mPuppetEye->kill();

    if (mPuppetEye) {
        mPuppetEye->kill();
        al::setNerve(this, &Dead);
    }
}

void PlayerCapManHeroEyesControl::kill() {
    if (!mPuppetEye)
        return;

    mPuppetEye->kill();
    al::setNerve(this, &Dead);
}

void PlayerCapManHeroEyesControl::update() {
    updateNerve();
}

al::LiveActor* PlayerCapManHeroEyesControl::getPuppetEye() const {
    return mPuppetEye;
}

bool PlayerCapManHeroEyesControl::isAppear() const {
    return al::isNerve(this, &Appear) || al::isNerve(this, &Wait);
}

bool PlayerCapManHeroEyesControl::isDisappear() const {
    if (al::isNerve(this, &Appear)) {
    } else if (!al::isNerve(this, &Wait)) {
        return true;
    }

    return false;
}

bool PlayerCapManHeroEyesControl::isDemo() const {
    return al::isNerve(this, &Demo);
}

bool PlayerCapManHeroEyesControl::isDeadCap() const {
    return al::isNerve(this, &Dead);
}

void PlayerCapManHeroEyesControl::requestWaitAnimChange(const char* actionName) {
    if (al::isNerve(this, &Wait) && al::isAlive(mPuppetEye))
        al::tryStartActionIfNotPlaying(mPuppetEye, actionName);
}

void PlayerCapManHeroEyesControl::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(mPuppetEye, "Appear");

    if (al::isActionEnd(mPuppetEye))
        al::setNerve(this, &Wait);
}

void PlayerCapManHeroEyesControl::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(mPuppetEye, "Wait");
}

void PlayerCapManHeroEyesControl::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(mPuppetEye, "Disappear");

    if (al::isActionEnd(mPuppetEye))
        al::setNerve(this, &Dead);
}

void PlayerCapManHeroEyesControl::exeDead() {
    if (mPuppetEye)
        mPuppetEye->kill();
}

void PlayerCapManHeroEyesControl::exeDemo() {}
