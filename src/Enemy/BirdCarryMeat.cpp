#include "Enemy/BirdCarryMeat.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Rail/RailUtil.h"

#include "Enemy/CarryMeat.h"
#include "Layout/BalloonIcon.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(BirdCarryMeat, WaitOnRail);
NERVE_IMPL(BirdCarryMeat, Drop);
NERVE_IMPL(BirdCarryMeat, DemoCarryMeat);
NERVE_IMPL(BirdCarryMeat, MoveMeat);
NERVE_IMPL(BirdCarryMeat, ReactionCarry);
NERVE_IMPL(BirdCarryMeat, FlyAway);

NERVES_MAKE_STRUCT(BirdCarryMeat, WaitOnRail, Drop, DemoCarryMeat, MoveMeat, ReactionCarry,
                   FlyAway);
}  // namespace

BirdCarryMeat::BirdCarryMeat(const char* name) : al::LiveActor(name) {}

// TODO: rename this
const sead::Vector3f cVec = {300.0f, 0.0f, 0.0f};

void BirdCarryMeat::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BirdCarryMeat", "CarryMeat");
    al::initNerve(this, &NrvBirdCarryMeat.WaitOnRail, 0);
    al::calcLinkChildNum(info, "CarryMeat");
    mCarryMeat = new CarryMeat(al::getLinksActorDisplayName(info, "CarryMeat", 0));
    mCarryMeat->setParent(this);

    al::initLinksActor(mCarryMeat, info, "CarryMeat", 0);
    mRailKeeper = al::tryCreateRailKeeper(al::getPlacementInfo(info), "Rail");
    if (!mRailKeeper)
        kill();
    mWaitRailKeeper = al::tryCreateRailKeeper(al::getPlacementInfo(info), "WaitRail");

    mInitTrans.set(al::getTrans(this));
    mInitQuat.set(al::getQuat(this));

    mBalloonIcon = rs::createMeatBalloon(al::getLayoutInitInfo(info), &mJointMtx, cVec);
    alActorFunction::invalidateFarClipping(this);
    al::registActorToDemoInfo(this, info);
    makeActorAlive();
}

bool BirdCarryMeat::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    return false;
}

void BirdCarryMeat::control() {
    sead::Vector3f jointPos;
    sead::Quatf jointQuat;

    al::calcJointPos(&jointPos, this, "Cap01");
    al::calcJointQuat(&jointQuat, this, "Cap01");
    al::makeMtxQuatPos(&mJointMtx, jointQuat, jointPos);
}

sead::Matrix34f* BirdCarryMeat::getBindMtx() const {
    return al::getJointMtxPtr(this, "Meat");
}

void BirdCarryMeat::skipDemo() {
    al::setRailPosToEnd(this);
    al::resetPosition(this, al::getRailPos(this));
    al::tryStartActionIfNotPlaying(this, "FallMeat");
    al::clearSklAnimInterpole(this);
    al::faceToDirection(this, al::getRailDir(this));
    al::setNerve(this, &NrvBirdCarryMeat.WaitOnRail);
}

void BirdCarryMeat::exeWaitOnRail() {
    if (al::isFirstStep(this)) {
        mBalloonIcon->startUpdateDraw();
        al::startAction(this, "Wait");
    }

    al::moveSyncRailLoop(this, 8.0f);
    al::turnToRailDir(this, 3.0f);

    if (mCarryMeat->isBindable())
        al::setNerve(this, &NrvBirdCarryMeat.WaitOnRail);
}

void BirdCarryMeat::exeDemoCarryMeat() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        mBalloonIcon->hideAndStopUpdate();
        mCarryMeat->startDemo();
        rs::addDemoActor(this, false);
        mCarryMeat->addDemoActor();
        al::startAction(this, "DemoCarryMeat");
    }

    if (al::isActionEnd(this)) {
        mCarryMeat->endDemoAndBind(getBindMtx());
        al::setRailPosToStart(this);
        al::setNerve(this, &NrvBirdCarryMeat.Drop);
    }
}

void BirdCarryMeat::exeMoveMeat() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "MoveMeat")) {
        al::updatePoseMtx(this, al::getJointMtxPtr(this, "AllRoot"));
        sead::Vector3f position;
        al::getJointMtxPtr(this, "AllRoot")->getTranslation(position);
        al::resetPosition(this, position);
        al::startAction(this, "MoveMeat");
        al::clearSklAnimInterpole(this);
    }

    al::moveSyncRail(
        this, al::lerpValue(25.0f, 35.0f, al::getRailCoord(this) / al::getRailTotalLength(this)));
    al::turnToDirection(this, al::getRailDir(this), 1.0f);

    if (al::isRailReachedEnd(this)) {
        al::setNerve(this, &NrvBirdCarryMeat.WaitOnRail);
        return;
    }
    if (mCarryMeat->isCarryReaction())
        al::setNerve(this, &NrvBirdCarryMeat.DemoCarryMeat);
}

void BirdCarryMeat::exeReactionCarry() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionCarry");

    al::moveSyncRail(
        this, al::lerpValue(25.0f, 35.0f, al::getRailCoord(this) / al::getRailTotalLength(this)));
    al::turnToDirection(this, al::getRailDir(this), 3.0f);

    if (al::isRailReachedEnd(this)) {
        al::setNerve(this, &NrvBirdCarryMeat.WaitOnRail);
        return;
    }

    if (al::isActionEnd(this)) {
        al::startAction(this, "MoveMeat");
        al::setNerve(this, &NrvBirdCarryMeat.Drop);
    }
}

void BirdCarryMeat::exeDrop() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "FallMeat");
        al::setVelocityZero(this);
    }
    if (al::isGreaterEqualStep(this, 60)) {
        mCarryMeat->drop();
        al::setNerve(this, &NrvBirdCarryMeat.MoveMeat);
    }
}

void BirdCarryMeat::exeFlyAway() {
    if (al::isFirstStep(this)) {
        al::setVelocityToFront(this, 30.0f);
        al::addVelocityY(this, 10.0f);
    }
    if (al::isGreaterEqualStep(this, 180)) {
        alActorFunction::invalidateFarClipping(this);
        al::setVelocityZero(this);
        al::hideModelIfShow(this);
        if (mCarryMeat->isWaiting()) {
            al::showModelIfHide(this);
            al::setTrans(this, mInitTrans);
            al::setQuat(this, mInitQuat);
            al::setNerve(this, &NrvBirdCarryMeat.WaitOnRail);
        }
    }
}

bool BirdCarryMeat::isDropNerve() const {
    return al::isNerve(this, &NrvBirdCarryMeat.WaitOnRail) ||
           al::isNerve(this, &NrvBirdCarryMeat.MoveMeat);
}

al::RailRider* BirdCarryMeat::getRailRider() const {
    if (al::isNerve(this, &NrvBirdCarryMeat.WaitOnRail) && mWaitRailKeeper)
        return mWaitRailKeeper->getRailRider();

    return mRailKeeper->getRailRider();
}
