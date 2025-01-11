#include "Player/PlayerSword.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/Sensor.h"

static const char* weaponTypes[] = {"Sword", "PowerGrove"};

PlayerSword::PlayerSword(const char* name) : al::LiveActor(name) {}

void PlayerSword::initPartsMtx(al::LiveActor* other, const al::ActorInitInfo& info,
                               const sead::Matrix34f* mtx, const char* name) {
    mOther = other;
    mBodySensor = al::getHitSensor(other, "Body");
    mMtx = mtx;

    if (name)
        al::initChildActorWithArchiveNameNoPlacementInfo(this, info, name, nullptr);
    else {
        s32 type = 0;
        al::tryGetArg(&type, info, "WeaponType");
        al::initChildActorWithArchiveNameNoPlacementInfo(this, info, weaponTypes[type], nullptr);
    }

    al::setHitSensorMtxPtr(this, "Attack", other->getBaseMtx());
    al::invalidateClipping(this);
    al::invalidateHitSensors(this);
    makeActorAlive();
}

void PlayerSword::makeActorAlive() {
    updatePose();
    al::LiveActor::makeActorAlive();
    field_120 = false;
}

void PlayerSword::updatePose() {
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

void PlayerSword::control() {
    if (al::updateSyncHostVisible(&field_120, this, mOther, 0)) {
        al::showModelIfHide(this);
        updatePose();
    } else
        al::hideModelIfShow(this);
}

void PlayerSword::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::sendMsgPlayerSwordAttack(other, mBodySensor);
}
