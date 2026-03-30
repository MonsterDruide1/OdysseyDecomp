#include "Boss/Koopa/KoopaCappyEyes2D.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(KoopaCappyEyes2D, Appear)
NERVE_IMPL(KoopaCappyEyes2D, Disappear)
NERVE_IMPL(KoopaCappyEyes2D, Wait)

NERVES_MAKE_NOSTRUCT(KoopaCappyEyes2D, Appear, Disappear, Wait)
}  // namespace

KoopaCappyEyes2D::KoopaCappyEyes2D(const char* name) : al::PartsModel(name) {}

void KoopaCappyEyes2D::init(const al::ActorInitInfo& info) {
    al::initNerve(this, &Appear, 0);
    makeActorDead();
}

void KoopaCappyEyes2D::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Appear);
}

void KoopaCappyEyes2D::disappear() {
    if (!al::isNerve(this, &Disappear))
        al::setNerve(this, &Disappear);
}

bool KoopaCappyEyes2D::isPlayingAppearAnim() const {
    return al::isNerve(this, &Appear);
}

void KoopaCappyEyes2D::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");
    al::setNerveAtActionEnd(this, &Wait);
}

void KoopaCappyEyes2D::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void KoopaCappyEyes2D::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Disappear");
    if (al::isActionEnd(this))
        kill();
}
