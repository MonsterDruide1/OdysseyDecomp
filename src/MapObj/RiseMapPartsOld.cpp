#include "MapObj/RiseMapPartsOld.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(RiseMapPartsOld, Wait);
NERVE_IMPL(RiseMapPartsOld, Stop);
NERVE_IMPL(RiseMapPartsOld, Move);

NERVES_MAKE_NOSTRUCT(RiseMapPartsOld, Wait, Stop, Move);
}  // namespace

RiseMapPartsOld::RiseMapPartsOld(const char* name) : al::LiveActor(name) {}

void RiseMapPartsOld::init(const al::ActorInitInfo& info) {
    using RiseMapPartsOldFunctor = al::FunctorV0M<RiseMapPartsOld*, void (RiseMapPartsOld::*)()>;

    al::initMapPartsActor(this, info, nullptr);
    al::initActorSeKeeper(this, info, "SuccessSeObj");
    al::initNerve(this, &Wait, 0);
    mKeyPoseKeeper = al::createKeyPoseKeeper(info);
    bool isStageSwitchListening = al::listenStageSwitchOnStart(
        this, RiseMapPartsOldFunctor(this, &RiseMapPartsOld::startRise));
    mPlacementId = al::createPlacementId(info);
    mMoveTime = al::calcKeyMoveMoveTime(mKeyPoseKeeper);
    if (!isStageSwitchListening)
        makeActorDead();

    bool isValidObjectCamera = false;
    al::tryGetArg(&isValidObjectCamera, info, "IsValidObjectCamera");
    if (isValidObjectCamera)
        mObjectCamera = al::initObjectCamera(this, info, nullptr, "固定");

    makeActorAlive();
}

void RiseMapPartsOld::startRise() {
    if (al::isDead(this))
        appear();

    const al::Nerve* stopNerve = &Stop;
    const al::Nerve* moveNerve = &Move;
    if (al::isNerve(this, stopNerve) || al::isNerve(this, moveNerve))
        return;

    if (mObjectCamera) {
        al::startCamera(this, mObjectCamera, -1);
        al::startSe(this, "Riddle");
    }

    al::invalidateClipping(this);
    al::setNerve(this, moveNerve);
}

void RiseMapPartsOld::initAfterPlacement() {
    if (al::isNerve(this, &Stop))
        al::tryOnStageSwitch(this, "SwitchStopOn");
}

void RiseMapPartsOld::exeWait() {}

void RiseMapPartsOld::exeMove() {
    if (al::isGreaterEqualStep(this, mMoveTime)) {
        if (mObjectCamera)
            al::endCamera(this, mObjectCamera, -1, false);

        al::setNerve(this, &Stop);
        return;
    }

    sead::Vector3f trans = sead::Vector3f::zero;
    const al::KeyPoseKeeper* keyPoseKeeper = mKeyPoseKeeper;
    f32 rate = al::calcNerveRate(this, mMoveTime);
    al::calcLerpKeyTrans(&trans, keyPoseKeeper, rate);
    al::setTrans(this, trans);
}

void RiseMapPartsOld::exeStop() {
    if (al::isFirstStep(this)) {
        al::resetPosition(this, al::getNextKeyTrans(mKeyPoseKeeper));
        al::validateClipping(this);
        al::tryOnStageSwitch(this, "SwitchStopOn");
    }
}
