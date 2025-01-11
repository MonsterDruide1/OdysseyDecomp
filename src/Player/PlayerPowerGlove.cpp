#include "Player/PlayerPowerGlove.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/Sensor.h"

PlayerPowerGlove::PlayerPowerGlove() : al::LiveActor("パワーグローブ") {}

void PlayerPowerGlove::initPartsMtx(al::LiveActor* other, const al::ActorInitInfo& info,
                                    const sead::Matrix34f* mtx) {
    mOther = other;
    mBodySensor = al::getHitSensor(other, "Body");
    mMtx = mtx;

    al::initChildActorWithArchiveNameNoPlacementInfo(this, info, "PowerGrove", nullptr);

    al::setHitSensorMtxPtr(this, "Attack", other->getBaseMtx());
    al::invalidateClipping(this);
    al::invalidateHitSensors(this);
    makeActorAlive();
}

void PlayerPowerGlove::makeActorAlive() {
    updatePose();
    al::LiveActor::makeActorAlive();
    field_120 = false;
}

void PlayerPowerGlove::updatePose() {
    // ???
    sead::Matrix34f t;
    t.makeR(sead::Vector3f(sead::Mathf::piHalf(), 0, 0));
    t.makeR(sead::Vector3f(0, 0, 0));

    sead::Matrix34f newPose = *mMtx;
    al::normalize(&newPose);

    sead::Matrix34f poseMtx = {0, 0, 1.0f, 0, 0, -0.001f, 0.7f, 0, 1.0f, 0.001f, 0.3f, 0};

    newPose.setMul(newPose, poseMtx);

    return al::updatePoseMtx(this, &newPose);
}

void PlayerPowerGlove::control() {
    if (al::updateSyncHostVisible(&field_120, this, mOther, 0)) {
        al::showModelIfHide(this);
        updatePose();
    } else
        al::hideModelIfShow(this);
}

void PlayerPowerGlove::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::sendMsgCapAttack(other, mBodySensor);
}

void PlayerPowerGlove::init(const al::ActorInitInfo& info) {}
