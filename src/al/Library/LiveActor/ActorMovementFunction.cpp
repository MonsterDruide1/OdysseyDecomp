#include <math/seadVector.h>
#include "al/Library/Audio/AudioKeeper.h"
#include "al/Library/Collision/Collider.h"
#include "al/Library/HitSensor/SensorFunction.h"
#include "al/Library/LiveActor/ActorCollisionFunction.h"
#include "al/Library/LiveActor/ActorMovementFunction.h"
#include "al/Library/LiveActor/ActorPoseKeeper.h"
#include "al/Library/LiveActor/LiveActorUtil.h"
#include "al/Library/Math/MathUtil.h"
#include "al/Library/Screen/ScreenPointKeeper.h"
#include "al/Library/Se/SeKeeper.h"

namespace al {

float calcDistance(const LiveActor* l1, const LiveActor* l2) {
    return calcDistance(l1, getTrans(l2));
}

float calcDistance(const LiveActor* l1, const sead::Vector3f& vec) {
    sead::Vector3f distance = getTrans(l1) - vec;
    return distance.length();
}

float calcSpeed(const LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity().length();
}

float calcSpeedH(const LiveActor* actor) {
    sead::Vector3f verticalized;
    verticalizeVec(&verticalized, getGravity(actor), actor->getPoseKeeper()->getVelocity());
    return verticalized.length();
}

float calcSpeedV(const LiveActor* actor) {
    return -actor->getPoseKeeper()->getVelocity().dot(getGravity(actor));
}

void resetPosition(LiveActor* actor) {
    if (actor->getPoseKeeper())
        actor->calcAnim();
    if (actor->getHitSensorKeeper()) {
        alSensorFunction::clearHitSensors(actor);
        alSensorFunction::updateHitSensorsAll(actor);
    }
    if (actor->getScreenPointKeeper()) {
        alScreenPointFunction::updateScreenPointAll(actor);
    }
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

void resetRotatePosition(LiveActor* actor, const sead::Vector3f& rot,
                         const sead::Vector3f& trans) {
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

}  // namespace al
