#include "Boss/BossMagma/BossMagmaStep.h"

#include "Library/LiveActor/ActorSensorUtil.h"

#include "Boss/BossMagma/BossMagmaStepAnimControl.h"
#include "Util/SensorMsgFunction.h"

BossMagmaStep::BossMagmaStep() : WaveSurfMapParts("溶岩ボス足場") {
    mAnimControl = nullptr;
}

void BossMagmaStep::init(const al::ActorInitInfo& info) {
    WaveSurfMapParts::init(info);
    mAnimControl = new BossMagmaStepAnimControl(this);
}

bool BossMagmaStep::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (al::isSensorName(self, "Collision")) {
        if (rs::isMsgBubbleReflectV(message) || rs::isMsgBubbleWallTouch(message)) {
            mAnimControl->requestDownImm();
            return true;
        }
    }

    if (al::isMsgExplosionCollide(message)) {
        mAnimControl->requestDown();
        return true;
    }

    return WaveSurfMapParts::receiveMsg(message, other, self);
}

void BossMagmaStep::rebirth() {
    mAnimControl->requestUp();
}

void BossMagmaStep::down() {
    mAnimControl->requestDown();
}

void BossMagmaStep::control() {
    WaveSurfMapParts::control();
    mAnimControl->updateNerve();
}
