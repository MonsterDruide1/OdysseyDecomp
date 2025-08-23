#include "Player/PlayerPowerGlove.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/SensorMsgFunction.h"

PlayerPowerGlove::PlayerPowerGlove() : al::LiveActor("パワーグローブ") {}

void PlayerPowerGlove::initPartsMtx(al::LiveActor* other, const al::ActorInitInfo& info,
                                    const sead::Matrix34f* mtx) {
    mPlayer = other;
    mPlayerBodySensor = al::getHitSensor(other, "Body");
    mPlayerBaseMtx = mtx;

    al::initChildActorWithArchiveNameNoPlacementInfo(this, info, "PowerGrove", nullptr);

    al::setHitSensorMtxPtr(this, "Attack", other->getBaseMtx());
    al::invalidateClipping(this);
    al::invalidateHitSensors(this);
    makeActorAlive();
}

void PlayerPowerGlove::makeActorAlive() {
    updatePose();
    al::LiveActor::makeActorAlive();
    mIsInvisible = false;
}

void PlayerPowerGlove::updatePose() {
    sead::Matrix34f t;
    sead::Matrix34f tt;
    t.makeR(sead::Vector3f(sead::Mathf::piHalf(), 0, 0));
    tt.makeR(sead::Vector3f(0, 0, 0));

    sead::Matrix34f newPoseMtx = *mPlayerBaseMtx;
    al::normalize(&newPoseMtx);

    newPoseMtx = newPoseMtx * (t * tt);

    return al::updatePoseMtx(this, &newPoseMtx);
}

void PlayerPowerGlove::control() {
    if (al::updateSyncHostVisible(&mIsInvisible, this, mPlayer, false)) {
        al::showModelIfHide(this);
        updatePose();
    } else
        al::hideModelIfShow(this);
}

void PlayerPowerGlove::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::sendMsgCapAttack(other, mPlayerBodySensor);
}

void PlayerPowerGlove::init(const al::ActorInitInfo& info) {}
