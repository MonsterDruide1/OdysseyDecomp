#include "MapObj/CloudStepKeyMoveComponent.h"

#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "MapObj/CloudStep.h"

namespace {
NERVE_ACTION_IMPL(CloudStepKeyMoveComponent, Wait)
NERVE_ACTION_IMPL(CloudStepKeyMoveComponent, StandBy)
NERVE_ACTION_IMPL(CloudStepKeyMoveComponent, Move)
NERVE_ACTION_IMPL(CloudStepKeyMoveComponent, Stop)

NERVE_ACTIONS_MAKE_STRUCT(CloudStepKeyMoveComponent, Wait, StandBy, Move, Stop)
}  // namespace

CloudStepKeyMoveComponent::CloudStepKeyMoveComponent(CloudStep* cloudStep)
    : al::LiveActor("雲足場キームーブ"), mCloudStep(cloudStep) {}

void CloudStepKeyMoveComponent::init(const al::ActorInitInfo& info) {
    using CloudStepKeyMoveComponentFunctor =
        al::FunctorV0M<CloudStepKeyMoveComponent*, void (CloudStepKeyMoveComponent::*)()>;

    al::initNerveAction(this, "Wait", &NrvCloudStepKeyMoveComponent.collector, 0);
    al::initActorSceneInfo(this, info);
    mKeyPoseKeeper = al::createKeyPoseKeeper(info);
    al::tryGetArg(&mIsFloorTouchStart, info, "IsFloorTouchStart");

    if (al::getKeyPoseCount(mKeyPoseKeeper) < 2 || mIsFloorTouchStart ||
        al::listenStageSwitchOnStart(mCloudStep, CloudStepKeyMoveComponentFunctor(
                                                     this, &CloudStepKeyMoveComponent::start))) {
        al::startNerveAction(this, "StandBy");
    }

    al::listenStageSwitchOn(
        mCloudStep, "SwitchStop",
        CloudStepKeyMoveComponentFunctor(this, &CloudStepKeyMoveComponent::stop));
    makeActorAlive();
}

void CloudStepKeyMoveComponent::start() {
    if (!al::isNerve(this, NrvCloudStepKeyMoveComponent.StandBy.data()))
        return;

    if (al::getKeyPoseCount(mKeyPoseKeeper) < 2)
        return;

    al::startNerveAction(this, "Wait");
}

void CloudStepKeyMoveComponent::stop() {
    if (!al::isNerve(this, NrvCloudStepKeyMoveComponent.Stop.data()))
        al::startNerveAction(this, "Stop");
}

bool CloudStepKeyMoveComponent::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                           al::HitSensor* self) {
    if (!mIsFloorTouchStart || !al::isMsgFloorTouch(message))
        return false;

    if (al::isNerve(this, NrvCloudStepKeyMoveComponent.StandBy.data()) &&
        al::getKeyPoseCount(mKeyPoseKeeper) >= 2)
        al::startNerveAction(this, "Wait");

    return true;
}

void CloudStepKeyMoveComponent::exeWait() {
    if (al::isFirstStep(this)) {
        s32 waitTime = al::calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (waitTime >= 0)
            mWaitTime = waitTime;
    }

    if (al::isGreaterEqualStep(this, mWaitTime)) {
        if (al::isRestart(mKeyPoseKeeper)) {
            al::restartKeyPose(mKeyPoseKeeper, al::getTransPtr(mCloudStep),
                               al::getQuatPtr(mCloudStep));
            al::resetPosition(mCloudStep);
            al::startNerveAction(this, "Wait");
        } else {
            al::startNerveAction(this, "Move");
        }
    }
}

void CloudStepKeyMoveComponent::exeStandBy() {}

void CloudStepKeyMoveComponent::exeMove() {
    if (al::isFirstStep(this))
        mMoveTime = al::calcKeyMoveMoveTime(mKeyPoseKeeper);

    f32 rate = al::calcNerveRate(this, mMoveTime);
    al::calcLerpKeyTrans(al::getTransPtr(mCloudStep), mKeyPoseKeeper, rate);
    al::calcSlerpKeyQuat(al::getQuatPtr(mCloudStep), mKeyPoseKeeper, rate);

    if (al::isGreaterEqualStep(this, mMoveTime)) {
        al::nextKeyPose(mKeyPoseKeeper);

        if (al::isStop(mKeyPoseKeeper)) {
            if (al::isNerve(this, NrvCloudStepKeyMoveComponent.Stop.data()))
                return;

            al::startNerveAction(this, "Stop");
        } else {
            al::startNerveAction(this, "Wait");
        }
    }
}

void CloudStepKeyMoveComponent::exeStop() {
    if (al::isFirstStep(this) && al::isInvalidClipping(mCloudStep))
        al::validateClipping(mCloudStep);
}
