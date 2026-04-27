#include "Boss/FireBlower/FireBlowerCap.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL_(FireBlowerCap, CapWait, Wait)
NERVE_IMPL_(FireBlowerCap, CapDisappear, Disappear)
NERVE_IMPL_(FireBlowerCap, CapAppear, Appear)
NERVE_IMPL_(FireBlowerCap, CapResetAttackStart, ResetAttackStart)

NERVES_MAKE_NOSTRUCT(FireBlowerCap, CapAppear, CapResetAttackStart, CapWait, CapDisappear)

static al::EnemyStateBlowDownParam sFireBlowerCapBlowDownParam =
    al::EnemyStateBlowDownParam("Disappear", 18.0f, 35.0f, 1.0f, 0.9f, 120, true);
}  // namespace

FireBlowerCap::FireBlowerCap() : al::PartsModel("FireBlowerCap") {}

void FireBlowerCap::initCap(al::LiveActor* actor, const al::ActorInitInfo& initInfo) {
    initPartsFixFile(actor, initInfo, "FireBlowerCap", nullptr, "Cap");
    al::initNerve(this, &CapWait, 1);

    mStateBlowDown = new al::EnemyStateBlowDown(this, &sFireBlowerCapBlowDownParam, "吹き飛び状態");
    al::initNerveState(this, mStateBlowDown, &CapDisappear, "帽子吹き飛び挙動");

    makeActorAlive();
}

void FireBlowerCap::appear() {
    setPoseUpdate(true);
    al::LiveActor::appear();
    al::setNerve(this, &CapAppear);
}

void FireBlowerCap::disappear(const al::HitSensor* sourceSensor,
                              const al::HitSensor* targetSensor) {
    setPoseUpdate(false);
    mStateBlowDown->start(sourceSensor, targetSensor);
    al::setNerve(this, &CapDisappear);
}

void FireBlowerCap::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");

    if (al::isActionEnd(this))
        al::setNerve(this, &CapWait);
}

void FireBlowerCap::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void FireBlowerCap::appearResetAttackStart() {
    setPoseUpdate(true);
    al::LiveActor::appear();
    al::setNerve(this, &CapResetAttackStart);
}

void FireBlowerCap::exeResetAttackStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "ResetAttackStart");
}

void FireBlowerCap::exeDisappear() {
    if (al::updateNerveState(this))
        kill();
}

bool FireBlowerCap::isDisappear() const {
    return al::isNerve(this, &CapDisappear);
}
