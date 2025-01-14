#include "Player/PlayerPowerGlove.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
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
    // These are created but not used
    sead::Matrix34f t;
    sead::Matrix34f tt;
    t.makeR(sead::Vector3f(sead::Mathf::piHalf(), 0, 0));
    tt.makeR(sead::Vector3f(0, 0, 0));

    sead::Matrix34f newPoseMtx = *mPlayerBaseMtx;
    al::normalize(&newPoseMtx);

    newPoseMtx.setMul(newPoseMtx, {0, 0, 1.0f, 0, 0, -0.001f, 0.7f, 0, 1.0f, 0.001f, 0.3f, 0});

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
