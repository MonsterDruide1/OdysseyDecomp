#include "Player/PlayerSword.h"

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

// BUG: second entry should have been `PowerGlove`. Unused, so does not matter
static const char* sWeaponTypes[] = {"Sword", "PowerGrove"};

PlayerSword::PlayerSword(const char* name) : al::LiveActor(name) {}

void PlayerSword::initPartsMtx(al::LiveActor* player, const al::ActorInitInfo& info,
                               const sead::Matrix34f* playerBaseMtx, const char* name) {
    mPlayer = player;
    mPlayerBodySensor = al::getHitSensor(player, "Body");
    mPlayerBaseMtx = playerBaseMtx;

    if (name)
        al::initChildActorWithArchiveNameNoPlacementInfo(this, info, name, nullptr);
    else {
        s32 type = 0;
        al::tryGetArg(&type, info, "WeaponType");
        al::initChildActorWithArchiveNameNoPlacementInfo(this, info, sWeaponTypes[type], nullptr);
    }

    al::setHitSensorMtxPtr(this, "Attack", player->getBaseMtx());
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
    sead::Matrix34f t;
    sead::Matrix34f tt;
    t.makeR({sead::Mathf::piHalf(), 0.0f, 0.0f});
    tt.makeR({0.0f, 0.0f, 0.0f});

    sead::Matrix34f newPoseMtx = *mPlayerBaseMtx;
    al::normalize(&newPoseMtx);

    newPoseMtx *= (t * tt);

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
