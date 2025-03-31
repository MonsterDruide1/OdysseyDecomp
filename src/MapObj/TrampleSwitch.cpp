#include "MapObj/TrampleSwitch.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "MapObj/AppearSwitchSave.h"
#include "Util/DemoUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(TrampleSwitch, OffWait);
NERVE_IMPL(TrampleSwitch, OnWait);
NERVE_IMPL(TrampleSwitch, On);
NERVE_IMPL(TrampleSwitch, Off);
NERVE_IMPL(TrampleSwitch, OnDemoWaitStart);
NERVE_IMPL(TrampleSwitch, OnDemo);

NERVES_MAKE_NOSTRUCT(TrampleSwitch, OnDemo);
NERVES_MAKE_STRUCT(TrampleSwitch, OffWait, OnWait, On, Off, OnDemoWaitStart);
}  // namespace

TrampleSwitch::TrampleSwitch(const char* actorName) : al::LiveActor(actorName) {}

void TrampleSwitch::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "TrampleSwitch", nullptr);
    al::initNerve(this, &NrvTrampleSwitch.OffWait, 0);

    mMtxConnector = al::createMtxConnector(this);
    mAddDemoInfo = al::registDemoRequesterToAddDemoInfo(this, info, 0);
    mIsFacingUp = al::isNearZeroOrLess(al::calcQuatUpY(al::getQuat(this)), 0.001f);

    mCollisionBody = al::createCollisionObj(this, info, "TrampleSwitch_Body",
                                            al::getHitSensor(this, "PPanel"), nullptr, nullptr);
    mCollisionBody->makeActorAlive();
    al::validateCollisionParts(mCollisionBody);

    if (al::isObjectName(info, "TrampleSwitchSave")) {
        mAppearSwitchSave = new AppearSwitchSave(this, info);
        if (mAppearSwitchSave->isOn()) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvTrampleSwitch.OnWait);
        }
        if (mAppearSwitchSave->isOn())
            al::tryOnStageSwitch(this, "SwitchPermanentOn");
        else
            al::tryOffStageSwitch(this, "SwitchPermanentOn");
    } else {
        using TrampleSwitchFunctor = al::FunctorV0M<TrampleSwitch*, void (TrampleSwitch::*)()>;

        al::listenStageSwitchOff(this, "SwitchTrampleOn",
                                 TrampleSwitchFunctor(this, &TrampleSwitch::offSwitch));
        al::listenStageSwitchOn(this, "SwitchReset",
                                TrampleSwitchFunctor(this, &TrampleSwitch::resetSwitch));

        if (al::tryGetBoolArgOrFalse(info, "IsValidObjectCamera"))
            mDemoCamera = al::initDemoObjectCamera(this, info, nullptr, "固定");
    }
    makeActorAlive();
}

void TrampleSwitch::offSwitch() {
    if (al::isNerve(this, &NrvTrampleSwitch.On) || isOn()) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvTrampleSwitch.Off);
    }
}

void TrampleSwitch::resetSwitch() {
    al::setNerve(this, &NrvTrampleSwitch.OffWait);
    al::offStageSwitch(this, "SwitchTrampleOn");
}

void TrampleSwitch::initAfterPlacement() {
    al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void TrampleSwitch::control() {
    al::connectPoseQT(this, mMtxConnector);
}

void TrampleSwitch::exeOffWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::startAction(this, "OffWait");
    }
}

void TrampleSwitch::exeOn() {
    if (al::isFirstStep(this)) {
        al::tryOnStageSwitch(this, "SwitchPermanentOn");
        al::startAction(this, "On");
    }
    if (al::isActionEnd(this)) {
        if (mDemoCamera) {
            al::setNerve(this, &NrvTrampleSwitch.OnDemoWaitStart);
            return;
        }

        if (mAppearSwitchSave)
            mAppearSwitchSave->onSwitch();
        else
            al::tryOnStageSwitch(this, "SwitchTrampleOn");

        al::setNerve(this, &NrvTrampleSwitch.OnWait);
    }
}

void TrampleSwitch::exeOnDemoWaitStart() {
    if (rs::requestStartDemoNormal(this, false)) {
        if (mAppearSwitchSave)
            mAppearSwitchSave->onSwitchDemo();
        else {
            al::tryOnStageSwitch(this, "SwitchTrampleOn");
            al::tryOnStageSwitch(this, "SwitchPermanentOn");
        }
        al::startCamera(this, mDemoCamera, -1);
        al::addDemoActorFromAddDemoInfo(this, mAddDemoInfo);
        al::setNerve(this, &OnDemo);
    }
}

void TrampleSwitch::exeOnDemo() {
    if (al::isGreaterEqualStep(this, 150)) {
        al::endCamera(this, mDemoCamera, -1, 0);
        rs::requestEndDemoNormal(this);
        al::setNerve(this, &NrvTrampleSwitch.OnWait);
    }
}

void TrampleSwitch::exeOnWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::invalidateHitSensors(this);
        al::startAction(this, "OnWait");
    }
}

void TrampleSwitch::exeOff() {
    if (al::isFirstStep(this))
        al::startAction(this, "Off");
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvTrampleSwitch.OffWait);
}

bool TrampleSwitch::isOn() const {
    return al::isNerve(this, &NrvTrampleSwitch.OnWait);
}

// NON_MATCHING: extra register used for the last comparsion, https://decomp.me/scratch/HyCSL
bool TrampleSwitch::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (al::isNerve(this, &NrvTrampleSwitch.Off))
        return false;
    if (al::isSensorName(self, "PlayerRegard")) {
        if (!mIsFacingUp && rs::isMsgPlayerDisregardTargetMarker(message))
            return true;
        return false;
    }
    if (mIsFacingUp && !al::isSensorName(self, "PPanel"))
        return false;

    bool v10 = (rs::isMsgCapTouchWall(message) || rs::isMsgCapAttackCollide(message)) &&
               (mIsFacingUp || !al::isNearZeroOrGreater(al::getActorVelocity(other).y, 0.001f));
    bool v11 = rs::isMsgCapHipDrop(message);
    bool v12 = al::isMsgPlayerTouch(message);

    if (v12 || v10 || v11)
        return trySetNerveOn();

    return false;
}

bool TrampleSwitch::trySetNerveOn() {
    if (al::isNerve(this, &NrvTrampleSwitch.OffWait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvTrampleSwitch.On);
        return true;
    }
    return false;
}
