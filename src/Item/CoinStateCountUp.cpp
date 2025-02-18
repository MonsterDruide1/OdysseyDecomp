#include "Item/CoinStateCountUp.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(CoinStateCountUp, CountUp);
NERVE_IMPL(CoinStateCountUp, Got);

NERVES_MAKE_NOSTRUCT(CoinStateCountUp, CountUp, Got);
}  // namespace

CoinStateCountUp::CoinStateCountUp(al::LiveActor* actor)
    : ActorStateBase("カウントアップ状態", actor) {
    initNerve(&CountUp, 0);
}

void CoinStateCountUp::appear() {
    setDead(false);
    al::setNerve(this, &CountUp);
}

void CoinStateCountUp::exeCountUp() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        mTransY = al::getTrans(actor).y;
        al::setVelocity(actor, sead::Vector3f::ey * 65.0f);
    }

    al::rotateQuatYDirDegree(actor, 9.5f);
    al::scaleVelocity(actor, 0.9f);
    al::addVelocityToGravity(actor, 1.5f);

    if (mTransY > al::getTrans(actor).y - 300.0f && al::getVelocity(actor).y < 0.0f) {
        al::startHitReaction(actor, "釣り取得");
        al::setNerve(this, &Got);
    }
}

void CoinStateCountUp::exeGot() {
    kill();
}
