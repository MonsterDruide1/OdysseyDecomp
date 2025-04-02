#include "MapObj/WeightSwitch.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Item/Shine.h"
#include "Util/DemoUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(WeightSwitch, OffWait);
NERVE_IMPL(WeightSwitch, OnWait);
NERVE_IMPL(WeightSwitch, OnDemoWaitStart);
NERVE_IMPL(WeightSwitch, OnDemoWaitCameraInterpoling);
NERVE_IMPL(WeightSwitch, OnDemo);
NERVE_IMPL(WeightSwitch, Off);
NERVE_IMPL(WeightSwitch, On);

NERVES_MAKE_NOSTRUCT(WeightSwitch, OnDemoWaitCameraInterpoling, OnDemo, Off);
NERVES_MAKE_STRUCT(WeightSwitch, OffWait, OnWait, OnDemoWaitStart, On);
}  // namespace

WeightSwitch::WeightSwitch(const char* actorName) : al::LiveActor(actorName) {}

void WeightSwitch::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "TrampleSwitch", nullptr);
    al::initNerve(this, &NrvWeightSwitch.OffWait, 0);

    mMtxConnector = al::createMtxConnector(this);

    bool isValidObjectCamera = false;
    al::tryGetArg(&isValidObjectCamera, info, "IsValidObjectCamera");
    if (isValidObjectCamera)
        mDemoCamera = al::initDemoObjectCamera(this, info, nullptr, "固定");

    mShine = rs::tryInitLinkShine(info, "ShineActor", 0);
    mCollisionBody = al::createCollisionObj(this, info, "TrampleSwitch_Body",
                                            al::getHitSensor(this, "PPanel"), nullptr, nullptr);
    mCollisionBody->makeActorAlive();
    al::validateCollisionParts(mCollisionBody);

    makeActorAlive();
}

void WeightSwitch::initAfterPlacement() {
    al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void WeightSwitch::control() {
    al::connectPoseQT(this, mMtxConnector);
}

void WeightSwitch::exeOffWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::validateHitSensors(this);
        al::startAction(this, "OffWait");
    }
}

void WeightSwitch::exeOn() {
    if (al::isFirstStep(this))
        al::startAction(this, "On");

    if (al::isActionEnd(this)) {
        if (mDemoCamera) {
            al::setNerve(this, &NrvWeightSwitch.OnDemoWaitStart);
            return;
        }
        al::tryOnStageSwitch(this, "SwitchTrampleOn");

        if (mShine)
            al::sendMsgRestart(mShine);
        al::setNerve(this, &NrvWeightSwitch.OnWait);
    }
}

void WeightSwitch::exeOnDemoWaitStart() {
    if (rs::requestStartDemoNormal(this, false)) {
        al::startCamera(this, mDemoCamera, -1);
        al::tryOnStageSwitch(this, "SwitchTrampleOn");
        al::setNerve(this, &OnDemoWaitCameraInterpoling);
    }
}

void WeightSwitch::exeOnDemoWaitCameraInterpoling() {
    if (!al::isActiveCameraInterpole(this, 0))
        al::setNerve(this, &OnDemo);
}

void WeightSwitch::exeOnDemo() {
    if (al::isFirstStep(this) && mShine) {
        al::sendMsgRestart(mShine);
        rs::addDemoActor(mShine, false);
    }
    if (al::isActionPlaying(this, "On") && al::isActionEnd(this))
        al::startAction(this, "OnWait");

    if (al::isGreaterEqualStep(this, 150)) {
        al::endCamera(this, mDemoCamera, -1, 0);
        rs::requestEndDemoNormal(this);
        al::setNerve(this, &NrvWeightSwitch.OnWait);
    }
}

void WeightSwitch::exeOnWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "OnWait");

    mWeight--;

    if (mWeight <= 0) {
        al::tryOffStageSwitch(this, "SwitchTrampleOn");
        if (mShine)
            mShine->kill();
        al::setNerve(this, &Off);
    }
}

void WeightSwitch::exeOff() {
    if (al::isFirstStep(this))
        al::startAction(this, "Off");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvWeightSwitch.OffWait);
}

bool WeightSwitch::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (al::isSensorName(self, "PlayerRegard"))
        return rs::isMsgPlayerDisregardTargetMarker(message);
    if (!al::isNerve(this, &NrvWeightSwitch.OffWait) || al::isSensorName(self, "PPanel")) {
        if (al::isMsgPlayerTouch(message))
            mWeight = 7;
        else if (al::isMsgEnemyFloorTouch(message))
            mWeight = 20;
        else if (rs::isMsgCapHipDrop(message))
            mWeight = 28;
        if (mWeight < 1)
            return false;
        if (al::isNerve(this, &NrvWeightSwitch.OffWait)) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvWeightSwitch.On);
            return true;
        }
    }
    return false;
}
