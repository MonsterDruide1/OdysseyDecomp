#include "Enemy/KakkuGlideAnimCtrl.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Enemy/KakkuStateGlide.h"

namespace {
NERVE_IMPL(KakkuGlideAnimCtrl, GlideNormal);
NERVE_IMPL(KakkuGlideAnimCtrl, GlideLeft);
NERVE_IMPL(KakkuGlideAnimCtrl, GlideRight);
NERVE_IMPL(KakkuGlideAnimCtrl, GlideLeftStart);
NERVE_IMPL(KakkuGlideAnimCtrl, GlideRightStart);

NERVES_MAKE_STRUCT(KakkuGlideAnimCtrl, GlideNormal, GlideLeft, GlideRight, GlideLeftStart,
                   GlideRightStart);
}  // namespace

KakkuGlideAnimCtrl::KakkuGlideAnimCtrl(const char* name, al::LiveActor* actor,
                                       KakkuStateGlide* stateGlide)
    : al::ActorStateBase(name, actor), mStateGlide(stateGlide) {
    initNerve(&NrvKakkuGlideAnimCtrl.GlideNormal, 0);
}

void KakkuGlideAnimCtrl::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideNormal);
}

bool KakkuGlideAnimCtrl::isNormal() const {
    return al::isActionPlaying(mActor, "Glide");
}

void KakkuGlideAnimCtrl::exeGlideNormal() {
    tryChangeAnim();
}

void KakkuGlideAnimCtrl::tryChangeAnim() {
    sead::Vector2f stick = sead::Vector2f::zero;
    mStateGlide->getStick(&stick);

    if (stick.y <= -0.8f) {
        al::tryStartActionIfNotPlaying(mActor, "GlideRise");
        al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideNormal);
        return;
    }

    if (stick.y >= 0.8f) {
        al::tryStartActionIfNotPlaying(mActor, "GlideDrop");
        al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideNormal);
        return;
    }

    if (stick.x <= -0.8f) {
        if (!al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideLeftStart) &&
            !al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideLeft))
            al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideLeftStart);
        return;
    }

    if (stick.x >= 0.8f) {
        if (!al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideRightStart) &&
            !al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideRight))
            al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideRightStart);
        return;
    }

    al::tryStartActionIfNotPlaying(mActor, "Glide");
    al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideNormal);
}

void KakkuGlideAnimCtrl::exeGlideLeftStart() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GlideLStart");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideLeft);
    else
        tryChangeAnim();
}

void KakkuGlideAnimCtrl::exeGlideLeft() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GlideL");

    tryChangeAnim();
}

void KakkuGlideAnimCtrl::exeGlideRightStart() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GlideRStart");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvKakkuGlideAnimCtrl.GlideRight);
    else
        tryChangeAnim();
}

void KakkuGlideAnimCtrl::exeGlideRight() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GlideR");

    tryChangeAnim();
}

bool KakkuGlideAnimCtrl::isCurrentLeftAnim() const {
    return al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideLeftStart) ||
           al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideLeft);
}

bool KakkuGlideAnimCtrl::isCurrentRightAnim() const {
    return al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideRightStart) ||
           al::isNerve(this, &NrvKakkuGlideAnimCtrl.GlideRight);
}
