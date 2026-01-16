#include "Player/PlayerPushReceiver.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"

#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

PlayerPushReceiver::PlayerPushReceiver(const al::LiveActor* actor) : mActor(actor) {}

void PlayerPushReceiver::clear() {
    mPushMin = {0.0f, 0.0f, 0.0f};
    mPushMax = {0.0f, 0.0f, 0.0f};
    mCollidePushMin = {0.0f, 0.0f, 0.0f};
    mCollidePushMax = {0.0f, 0.0f, 0.0f};
}

bool PlayerPushReceiver::receivePushMsg(const al::SensorMsg* msg, const al::HitSensor* self,
                                        const al::HitSensor* other, f32 maxTrans) {
    sead::Vector3f pushTrans{0.0f, 0.0f, 0.0f};
    if (rs::tryReceiveMsgPushToPlayerAndCalcPushTrans(&pushTrans, msg, mActor, self, other,
                                                      maxTrans)) {
        receiveForceDirect(pushTrans);
        return true;
    }

    return false;
}

bool PlayerPushReceiver::receivePushMsgHacker(const al::SensorMsg* msg, const al::HitSensor* self,
                                              const al::HitSensor* other, f32 maxTrans,
                                              bool isPlayer) {
    sead::Vector3f pushTrans{0.0f, 0.0f, 0.0f};
    if (rs::tryReceiveMsgPushToHackerAndCalcPushTrans(&pushTrans, msg, mActor, self, other,
                                                      maxTrans, isPlayer)) {
        receiveForceDirect(pushTrans);
        return true;
    }

    return false;
}

bool PlayerPushReceiver::receivePushMsgYoshiNpc(const al::SensorMsg* msg, const al::HitSensor* self,
                                                const al::HitSensor* other, f32 maxTrans) {
    sead::Vector3f pushTrans{0.0f, 0.0f, 0.0f};
    if (rs::tryReceiveMsgPushToYoshiNpcAndCalcPushTrans(&pushTrans, msg, mActor, self, other,
                                                        maxTrans)) {
        receiveForceDirect(pushTrans);
        return true;
    }

    return false;
}

bool PlayerPushReceiver::receivePushMsgCap(const al::SensorMsg* msg, const al::HitSensor* self,
                                           const al::HitSensor* other, f32 maxTrans) {
    sead::Vector3f pushTrans{0.0f, 0.0f, 0.0f};
    if (rs::tryReceiveMsgPushToCapAndCalcPushTrans(&pushTrans, msg, mActor, self, other,
                                                   maxTrans)) {
        receiveForceDirect(pushTrans);
        return true;
    }

    return false;
}

bool PlayerPushReceiver::receivePushMsgGrowPlant(const al::SensorMsg* msg,
                                                 const al::HitSensor* self,
                                                 const al::HitSensor* other, f32 maxTrans) {
    sead::Vector3f pushTrans{0.0f, 0.0f, 0.0f};
    if (rs::tryReceiveMsgPushToGrowPlantAndCalcPushTrans(&pushTrans, msg, mActor, self, other,
                                                         maxTrans)) {
        receiveForceDirect(pushTrans);
        return true;
    }

    return false;
}

bool PlayerPushReceiver::receiveCollidePushMsg(const al::SensorMsg* msg) {
    sead::Vector3f pushTrans{0.0f, 0.0f, 0.0f};
    if (al::tryReceiveMsgCollidePush(&pushTrans, msg)) {
        receiveForceDirect(pushTrans);
        al::separateMinMax(&mCollidePushMin, &mCollidePushMax, pushTrans);
        return true;
    }

    return false;
}

void PlayerPushReceiver::receiveForceDirect(const sead::Vector3f& force) {
    al::separateMinMax(&mPushMin, &mPushMax, force);
}

void PlayerPushReceiver::cutPushVec(const sead::Vector3f& pushVec) {
    al::verticalizeVec(&mPushMin, pushVec, mPushMin);
    al::verticalizeVec(&mPushMax, pushVec, mPushMax);
}

void PlayerPushReceiver::calcPushVec(sead::Vector3f* outPushVec) const {
    outPushVec->setAdd(mPushMin, mPushMax);
}

void PlayerPushReceiver::calcOnlyCollidePushVec(sead::Vector3f* outCollidePushVec) const {
    outCollidePushVec->setAdd(mCollidePushMin, mCollidePushMax);
}

void PlayerPushReceiver::calcPushedVelocity(sead::Vector3f* outPushedVelocity,
                                            const sead::Vector3f& velocity) const {
    sead::Vector3f pushVec;
    calcPushVec(&pushVec);
    calcPushedVelocityCommon(outPushedVelocity, velocity, pushVec);
}

void PlayerPushReceiver::calcPushedVelocityCommon(sead::Vector3f* outPushedVelocity,
                                                  const sead::Vector3f& velocity,
                                                  const sead::Vector3f& pushVec) const {
    sead::Vector3f pushDir{0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&pushDir, pushVec)) {
        outPushedVelocity->set(velocity);
        return;
    }

    f32 pushForce = pushVec.length();
    f32 maxPushForce = pushDir.dot(velocity);

    f32 pushOffset = 1.0f;
    sead::Vector3f velocityDir{0.0f, 0.0f, 0.0f};

    if (al::tryNormalizeOrZero(&velocityDir, velocity)) {
        f32 rate = sead::Mathf::clamp(1.0f - pushDir.dot(velocityDir), 0.0f, 1.0f);
        pushOffset = sead::Mathf::clamp(-rate * maxPushForce, 1.0f, pushForce * 0.9f);
    }

    f32 force = sead::Mathf::clampMin(pushForce - pushOffset, 0.0f);
    pushForce = sead::Mathf::clampMin(maxPushForce, force);

    al::verticalizeVec(outPushedVelocity, pushDir, velocity);
    outPushedVelocity->setScaleAdd(pushForce, pushDir, *outPushedVelocity);
}

void PlayerPushReceiver::calcPushedVelocityWithCollide(sead::Vector3f* outPushedVelocity,
                                                       const sead::Vector3f& velocity,
                                                       const IUsePlayerCollision* collision,
                                                       f32 collisionRadius) const {
    sead::Vector3f pushVec;
    calcPushVec(&pushVec);

    if (rs::isCollidedWallFace(collision)) {
        const sead::Vector3f& wallNormal = rs::getCollidedWallNormal(collision);
        sead::Vector3f trans = al::getTrans(mActor);

        f32 distance = sead::Mathf::clampMin(
            collisionRadius - wallNormal.dot(trans - rs::getCollidedWallPos(collision)), 0.0f);

        if (distance >= 5.0f) {
            f32 dotPushVec = wallNormal.dot(pushVec);
            f32 dotVelocity = wallNormal.dot(velocity);

            f32 pushDistance = sead::Mathf::clampMin(
                al::sign(dotVelocity * dotPushVec) * sead::Mathf::abs(dotVelocity), 0.0f);

            f32 pushForce = sead::Mathf::abs(dotPushVec);
            f32 maxPushForce = sead::Mathf::clampMin(35.0f - distance - pushDistance, 0.0f);

            if (pushForce > maxPushForce) {
                pushVec.setScaleAdd(-sead::Mathf::clampMin(pushForce - maxPushForce, 0.0f) *
                                        al::sign(dotPushVec),
                                    wallNormal, pushVec);
            }
        }
    }

    calcPushedVelocityCommon(outPushedVelocity, velocity, pushVec);
}
