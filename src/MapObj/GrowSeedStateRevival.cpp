#include "MapObj/GrowSeedStateRevival.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/GrowSeedUtil.h"

namespace {
NERVE_IMPL(GrowSeedStateRevival, RevivalWait);
NERVE_IMPL(GrowSeedStateRevival, Revival);

NERVES_MAKE_NOSTRUCT(GrowSeedStateRevival, RevivalWait, Revival);
}  // namespace

GrowSeedStateRevival::GrowSeedStateRevival(const char* name, al::LiveActor* actor,
                                           sead::Quatf* quat)
    : al::ActorStateBase(name, actor), mRevivalQuat(quat) {
    mRevivalTrans.set(al::getTrans(mActor));

    initNerve(&RevivalWait, 0);
}

void GrowSeedStateRevival::appear() {
    al::NerveStateBase::appear();
    al::invalidateClipping(mActor);
    al::invalidateHitSensors(mActor);
    al::setNerve(this, &RevivalWait);
}

void GrowSeedStateRevival::kill() {
    al::NerveStateBase::kill();
    al::validateHitSensors(mActor);
}

void GrowSeedStateRevival::exeRevivalWait() {
    if (al::isFirstStep(mActor)) {
        al::hideModelIfShow(mActor);
        al::setVelocityZero(mActor);
        al::offCollide(mActor);
    }

    if (al::isGreaterEqualStep(mActor, 300)) {
        al::setTrans(mActor, mRevivalTrans);
        mRevivalQuat->set(sead::Quatf::unit);
        GrowSeedUtil::rotateRandom(mRevivalQuat);
        al::setNerve(this, &Revival);
    }
}

void GrowSeedStateRevival::exeRevival() {
    al::showModelIfHide(mActor);
    al::startHitReaction(mActor, "復活");
    al::onCollide(mActor);
    kill();
}
