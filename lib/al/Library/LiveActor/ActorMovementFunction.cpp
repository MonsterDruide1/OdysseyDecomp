#include "Library/LiveActor/ActorMovementFunction.h"

#include <math/seadVector.h>

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Collision/Collider.h"
#include "Library/HitSensor/SensorFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Se/SeKeeper.h"

namespace al {

f32 calcDistance(const LiveActor* l1, const LiveActor* l2) {
    return calcDistance(l1, getTrans(l2));
}

f32 calcDistance(const LiveActor* l1, const sead::Vector3f& vec) {
    sead::Vector3f distance = getTrans(l1) - vec;
    return distance.length();
}

f32 calcSpeed(const LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity().length();
}

f32 calcSpeedH(const LiveActor* actor) {
    sead::Vector3f verticalized;
    verticalizeVec(&verticalized, getGravity(actor), actor->getPoseKeeper()->getVelocity());
    return verticalized.length();
}

f32 calcSpeedV(const LiveActor* actor) {
    return -actor->getPoseKeeper()->getVelocity().dot(getGravity(actor));
}

void resetPosition(LiveActor* actor) {
    if (actor->getPoseKeeper())
        actor->calcAnim();
    if (actor->getHitSensorKeeper()) {
        alSensorFunction::clearHitSensors(actor);
        alSensorFunction::updateHitSensorsAll(actor);
    }
    if (actor->getScreenPointKeeper())
        alScreenPointFunction::updateScreenPointAll(actor);
    if (actor->getCollider())
        actor->getCollider()->onInvalidate();
    if (actor->getCollisionParts())
        resetAllCollisionMtx(actor);
    if (!actor->getAudioKeeper())
        return;
    if (!actor->getAudioKeeper()->getSeKeeper())
        return;
    actor->getAudioKeeper()->getSeKeeper()->resetPosition();
}

void resetPosition(LiveActor* actor, const sead::Vector3f& trans) {
    updatePoseTrans(actor, trans);
    resetPosition(actor);
}

void resetRotatePosition(LiveActor* actor, const sead::Vector3f& rot, const sead::Vector3f& trans) {
    updatePoseRotate(actor, rot);
    updatePoseTrans(actor, trans);
    resetPosition(actor);
}

void resetQuatPosition(LiveActor* actor, const sead::Quatf& quat, const sead::Vector3f& trans) {
    updatePoseQuat(actor, quat);
    updatePoseTrans(actor, trans);
    resetPosition(actor);
}

void resetMtxPosition(LiveActor* actor, const sead::Matrix34f& mtx) {
    updatePoseMtx(actor, &mtx);
    resetPosition(actor);
}

void resetActorPosition(LiveActor* actor, const LiveActor* target) {
    updatePoseMtx(actor, target->getBaseMtx());
    resetPosition(actor);
}

// trySetPosOnGround

const sead::Vector3f& getVelocity(const LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity();
}

sead::Vector3f* getVelocityPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocityPtr();
}

// separateVelocityHV

void addVelocity(LiveActor* actor, const sead::Vector3f& vel) {
    sead::Vector3f* currentVelocity = actor->getPoseKeeper()->getVelocityPtr();
    *currentVelocity += vel;
}

void addVelocity(LiveActor* actor, f32 x, f32 y, f32 z) {
    sead::Vector3f* currentVelocity = actor->getPoseKeeper()->getVelocityPtr();
    currentVelocity->x += x;
    currentVelocity->y += y;
    currentVelocity->z += z;
}

void addVelocityX(LiveActor* actor, f32 x) {
    sead::Vector3f* currentVelocity = actor->getPoseKeeper()->getVelocityPtr();
    currentVelocity->x += x;
}

void addVelocityY(LiveActor* actor, f32 y) {
    sead::Vector3f* currentVelocity = actor->getPoseKeeper()->getVelocityPtr();
    currentVelocity->y += y;
}

void addVelocityZ(LiveActor* actor, f32 z) {
    sead::Vector3f* currentVelocity = actor->getPoseKeeper()->getVelocityPtr();
    currentVelocity->z += z;
}

}  // namespace al
