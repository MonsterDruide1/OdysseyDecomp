#include "MapObj/ElectricSwitch.h"

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ElectricSwitch, Wait);
NERVE_IMPL(ElectricSwitch, HitReaction);
NERVE_IMPL(ElectricSwitch, ReactionEnd);
NERVES_MAKE_NOSTRUCT(ElectricSwitch, Wait, HitReaction, ReactionEnd);
}  // namespace

ElectricSwitch::ElectricSwitch(const char* name) : al::LiveActor(name) {}

void ElectricSwitch::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    al::initNerve(this, &Wait, 0);
    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
    makeActorAlive();
}

void ElectricSwitch::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    al::sendMsgPushVeryStrong(other, self);
}

bool ElectricSwitch::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isNerve(this, &Wait) && rs::isMsgConductLightning(message))
        return true;
    if (rs::isMsgHackAttack(message)) {
        al::setNerve(this, &HitReaction);
        return true;
    }
    return false;
}

void ElectricSwitch::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void ElectricSwitch::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void ElectricSwitch::exeWait() {
    if (al::isFirstStep(this)) {
        al::startMclAnim(this, "ElectricSwitch");
        al::setMclAnimFrameAndStop(this, 0.0f);
    }
}

void ElectricSwitch::exeHitReaction() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HitReaction");
        al::startMclAnim(this, "ElectricSwitch");
        al::setMclAnimFrameAndStop(this, 1.0f);
    }
    if (al::isActionEnd(this)) {
        al::tryOnStageSwitch(this, "SwitchElectricOn");
        al::setNerve(this, &ReactionEnd);
    }
}

void ElectricSwitch::exeReactionEnd() {}
