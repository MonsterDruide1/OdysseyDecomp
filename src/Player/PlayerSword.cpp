#include "Player/PlayerSword.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/SensorMsgFunction.h"

static const char* sWeaponTypes[] = {"Sword", "PowerGrove"};

PlayerSword::PlayerSword(const char* name) : al::LiveActor(name) {}

void PlayerSword::initPartsMtx(al::LiveActor* other, const al::ActorInitInfo& info,
                               const sead::Matrix34f* mtx, const char* name) {
    mPlayer = other;
    mPlayerBodySensor = al::getHitSensor(other, "Body");
    mPlayerBaseMtx = mtx;

    if (name)
        al::initChildActorWithArchiveNameNoPlacementInfo(this, info, name, nullptr);
    else {
        s32 type = 0;
        al::tryGetArg(&type, info, "WeaponType");
        al::initChildActorWithArchiveNameNoPlacementInfo(this, info, sWeaponTypes[type], nullptr);
    }

    al::setHitSensorMtxPtr(this, "Attack", other->getBaseMtx());
    al::invalidateClipping(this);
    al::invalidateHitSensors(this);
    makeActorAlive();
}

void PlayerSword::makeActorAlive() {
    updatePose();
    al::LiveActor::makeActorAlive();
    mIsInvisible = false;
}

void PlayerSword::updatePose() {
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

void PlayerSword::control() {
    if (al::updateSyncHostVisible(&mIsInvisible, this, mPlayer, false)) {
        al::showModelIfHide(this);
        updatePose();
    } else
        al::hideModelIfShow(this);
}

void PlayerSword::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::sendMsgPlayerSwordAttack(other, mPlayerBodySensor);
}
