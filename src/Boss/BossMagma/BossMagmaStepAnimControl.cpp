#include "Boss/BossMagma/BossMagmaStepAnimControl.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BossMagmaStepAnimControl, Wait)
NERVE_IMPL(BossMagmaStepAnimControl, Up)
NERVE_IMPL(BossMagmaStepAnimControl, DownSign)
NERVE_IMPL(BossMagmaStepAnimControl, Down)

NERVES_MAKE_NOSTRUCT(BossMagmaStepAnimControl, Wait, Up, DownSign, Down)
}  // namespace

BossMagmaStepAnimControl::BossMagmaStepAnimControl(al::LiveActor* actor)
    : al::NerveExecutor("溶岩ボス足場のアニメ制御"), mActor(actor) {
    initNerve(&Wait, 0);
}

void BossMagmaStepAnimControl::requestUp() {
    if (al::isNerve(this, &Wait) || al::isNerve(this, &Up))
        return;

    al::setNerve(this, &Up);
}

void BossMagmaStepAnimControl::requestDown() {
    if (al::isNerve(this, &DownSign) || al::isNerve(this, &Down))
        return;

    al::setNerve(this, &DownSign);
}

void BossMagmaStepAnimControl::requestDownImm() {
    if (al::isNerve(this, &Down))
        return;

    al::setNerve(this, &Down);
}

void BossMagmaStepAnimControl::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "Wait");
}

void BossMagmaStepAnimControl::exeUp() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "Up");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &Wait);
}

void BossMagmaStepAnimControl::exeDownSign() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "DownSign");

    if (al::isGreaterEqualStep(this, 180))
        al::setNerve(this, &Down);
}

void BossMagmaStepAnimControl::exeDown() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "Down");
}
