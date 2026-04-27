#include "Enemy/ElectricBall.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(ElectricBall, Wait)

NERVES_MAKE_NOSTRUCT(ElectricBall, Wait)
}  // namespace

ElectricBall::ElectricBall(const char* name, al::LiveActor* parentActor)
    : LiveActor(name), mParentActor(parentActor) {}

void ElectricBall::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ElectricBall", nullptr);
    al::initNerve(this, &Wait, 0);
    al::setTrans(this, al::getTrans(mParentActor));
    al::startAction(this, "Wait");
    makeActorDead();
}

void ElectricBall::control() {
    return al::setTrans(this, al::getTrans(mParentActor));
}

void ElectricBall::setChargeLevel(s32 chargeLevel) {
    if (chargeLevel <= 0) {
        al::hideModelIfShow(this);

        if (!al::isDead(this))
            return kill();

        return;
    }

    if (al::isDead(this))
        appear();

    al::setTrans(this, al::getTrans(mParentActor));
    al::showModelIfHide(this);

    if (!al::isActionPlaying(this, "Level"))
        al::startAction(this, "Level");

    mChargeLevel = chargeLevel;
}

void ElectricBall::exeWait() {
    if (mChargeLevel < 1)
        return;

    if (!al::isActionPlaying(this, "Level"))
        al::startAction(this, "Level");

    return al::setVisAnimFrameForAction(this, mChargeLevel - 1);
}
