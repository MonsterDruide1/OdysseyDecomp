#include "MapObj/Special2KeyMoveLift.h"

#include <math/seadVector.h>

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"
#include "Project/Joint/KeyPose.h"

namespace {
NERVE_ACTION_IMPL(Special2KeyMoveParts, StandBy)
NERVE_ACTION_IMPL(Special2KeyMoveParts, Delay)
NERVE_ACTION_IMPL(Special2KeyMoveParts, MoveSign)
NERVE_ACTION_IMPL(Special2KeyMoveParts, Move)
NERVE_ACTION_IMPL(Special2KeyMoveParts, Stop)
NERVE_ACTION_IMPL(Special2KeyMoveParts, Reset)

NERVE_ACTIONS_MAKE_STRUCT(Special2KeyMoveParts, StandBy, Delay, MoveSign, Move, Stop, Reset)
}  // namespace

Special2KeyMoveParts::Special2KeyMoveParts(const char* name) : al::LiveActor(name) {}

// NOTE: declares 1 state, but never adds it
void Special2KeyMoveParts::init(const al::ActorInitInfo& info) {
    using Special2KeyMovePartsFunctor =
        al::FunctorV0M<Special2KeyMoveParts*, void (Special2KeyMoveParts::*)()>;

    al::initNerveAction(this, "StandBy", &NrvSpecial2KeyMoveParts.collector, 1);
    al::initActor(this, info);

    al::listenStageSwitchOnStart(this,
                                 Special2KeyMovePartsFunctor(this, &Special2KeyMoveParts::start));
    al::listenStageSwitchOn(this, "SwitchReset",
                            Special2KeyMovePartsFunctor(this, &Special2KeyMoveParts::reset));

    mKeyPoseKeeper = al::createKeyPoseKeeper(info);
    al::getArg(&mDelayTime, info, "DelayTime");

    makeActorAlive();
}

void Special2KeyMoveParts::start() {
    al::invalidateClipping(this);

    if (mDelayTime > 0)
        al::startNerveAction(this, "Delay");
    else
        al::startNerveAction(this, "MoveSign");
}

void Special2KeyMoveParts::reset() {
    al::invalidateClipping(this);
    al::startNerveAction(this, "Reset");
}

void Special2KeyMoveParts::exeStandBy() {
    if (al::isFirstStep(this))
        al::validateClipping(this);
}

void Special2KeyMoveParts::exeDelay() {
    if (al::isGreaterEqualStep(this, mDelayTime))
        al::startNerveAction(this, "MoveSign");
}

void Special2KeyMoveParts::exeMoveSign() {
    if (al::isFirstStep(this))
        al::startAction(this, "MoveKeySign");

    if (al::isActionEnd(this))
        al::startNerveAction(this, "Move");
}

void Special2KeyMoveParts::exeMove() {
    if (al::isFirstStep(this))
        mMoveTime = al::calcKeyMoveMoveTime(mKeyPoseKeeper);

    f32 rate = al::calcNerveRate(this, mMoveTime);

    al::calcLerpKeyTrans(al::getTransPtr(this), mKeyPoseKeeper, rate);

    if (al::isGreaterEqualStep(this, mMoveTime))
        al::startNerveAction(this, "Stop");
}

void Special2KeyMoveParts::exeStop() {}

void Special2KeyMoveParts::exeReset() {
    if (al::isFirstStep(this)) {
    }

    const sead::Vector3f& keyTrans = mKeyPoseKeeper->getKeyPose(0).getTrans();
    const sead::Vector3f& trans = al::getTrans(this);

    sead::Vector3f moveDir = keyTrans - trans;
    al::tryNormalizeOrZero(&moveDir, moveDir);

    al::setTrans(this, trans + moveDir * 10.0f);

    if (moveDir.dot(keyTrans - trans) < 0.0f) {
        al::setTrans(this, keyTrans);
        al::startNerveAction(this, "StandBy");
        mKeyPoseKeeper->reset();
    }
}

Special2KeyMoveLift::Special2KeyMoveLift(const char* name) : al::KeyMoveMapParts(name) {}

void Special2KeyMoveLift::exeStandBy() {
    if (al::isFirstStep(this)) {
        if (mIsFirstStandBy)
            mIsFirstStandBy = false;
        else
            al::startHitReaction(this, "出現");
    }

    al::KeyMoveMapParts::exeStandBy();
}
