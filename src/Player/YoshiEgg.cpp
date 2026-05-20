#include "Player/YoshiEgg.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "MapObj/FukankunZoomCapMessage.h"
#include "MapObj/FukankunZoomTargetFunction.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(YoshiEgg, Wait);
NERVE_IMPL(YoshiEgg, Appear);
NERVE_IMPL(YoshiEgg, Break);

NERVES_MAKE_NOSTRUCT(YoshiEgg, Appear);

struct {
    YoshiEggNrvWait Wait;
    YoshiEggNrvBreak Break;
} NrvYoshiEgg;
}  // namespace

YoshiEgg::YoshiEgg(const al::LiveActor* hostActor, const IUsePlayerCollision* playerCollision)
    : al::LiveActor("卵"), mHostActor(hostActor), mPlayerCollision(playerCollision) {}

void YoshiEgg::init(const al::ActorInitInfo& info) {
    al::initChildActorWithArchiveNameNoPlacementInfo(this, info, "YoshiEgg", nullptr);
    al::initNerve(this, &NrvYoshiEgg.Wait, 0);

    if (al::isPlaced(info)) {
        bool isFukankunZoomCapMessage = false;
        if (al::tryGetArg(&isFukankunZoomCapMessage, info, "IsFukankunZoomCapMessage") &&
            isFukankunZoomCapMessage) {
            mFukankunZoomCapMessage = new FukankunZoomCapMessage(this);
            mFukankunZoomCapMessage->init(info, "CapMessage", "FukankunZoomYoshiEgg");

            FukankunZoomCapMessage* capMessage = mFukankunZoomCapMessage;
            capMessage->setWatchCount(FukankunZoomTargetFunction::getFukankunWatchCountDefault());
            capMessage->resetZoomTypes();
        }
    }

    makeActorDead();
}

void YoshiEgg::initAfterPlacement() {
    if (mFukankunZoomCapMessage)
        mFukankunZoomCapMessage->initAfterPlacement();
}

void YoshiEgg::initPlacementEgg() {
    al::copyPose(this, mHostActor);
    appear();
    al::setNerve(this, &NrvYoshiEgg.Wait);
}

void YoshiEgg::appearEgg() {
    al::copyPose(this, mHostActor);
    appear();
    al::invalidateClipping(this);
    al::setNerve(this, &Appear);
}

bool YoshiEgg::isEndAppear() const {
    return !al::isNerve(this, &Appear);
}

bool YoshiEgg::isBreak() const {
    return al::isNerve(this, &NrvYoshiEgg.Break);
}

void YoshiEgg::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");

    al::copyPose(this, mHostActor);

    if (al::isActionEnd(this)) {
        al::validateClipping(this);
        al::setNerve(this, &NrvYoshiEgg.Wait);
    }
}

void YoshiEgg::exeWait() {
    al::tryStartActionIfNotPlaying(this, "Wait");
    al::copyPose(this, mHostActor);

    if (mFukankunZoomCapMessage)
        mFukankunZoomCapMessage->update();
}

void YoshiEgg::exeBreak() {
    if (al::isFirstStep(this))
        al::startAction(this, "Break");

    if (al::isActionEnd(this))
        kill();
}

void YoshiEgg::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "Push") && !al::isNerve(this, &NrvYoshiEgg.Break))
        rs::sendMsgPushToPlayer(other, self);
}

bool YoshiEgg::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isSensorName(self, "Push"))
        return al::isMsgPlayerDisregard(message);

    if ((al::isMsgPlayerTrample(message) || rs::isMsgPlayerAndCapObjHipDropReflectAll(message) ||
         rs::isMsgCapAttack(message) || rs::isMsgThrowObjHitReflect(message) ||
         rs::isMsgTankBullet(message) || rs::isMsgMotorcycleAttack(message)) &&
        al::isNerve(this, &NrvYoshiEgg.Wait)) {
        al::setNerve(this, &NrvYoshiEgg.Break);
        return true;
    }

    return false;
}
