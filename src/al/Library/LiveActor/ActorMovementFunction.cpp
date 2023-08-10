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

float calcDistance(const al::LiveActor* l1, const al::LiveActor* l2) {
    return calcDistance(l1, getTrans(l2));
}

float calcDistance(const al::LiveActor* l1, const sead::Vector3f& vec) {
    sead::Vector3f distance = al::getTrans(l1) - vec;
    return distance.length();
}

float calcSpeed(const al::LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity().length();
}

float calcSpeedH(const al::LiveActor* actor) {
    sead::Vector3f verticalized;
    al::verticalizeVec(&verticalized, al::getGravity(actor), actor->getPoseKeeper()->getVelocity());
    return verticalized.length();
}

float calcSpeedV(const al::LiveActor* actor) {
    return -actor->getPoseKeeper()->getVelocity().dot(al::getGravity(actor));
}

void resetPosition(al::LiveActor* actor) {
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
        al::resetAllCollisionMtx(actor);
    if (!actor->getAudioKeeper())
        return;
    if (!actor->getAudioKeeper()->getSeKeeper())
        return;
    actor->getAudioKeeper()->getSeKeeper()->resetPosition();
}

void resetPosition(al::LiveActor* actor, const sead::Vector3f& trans) {
    al::updatePoseTrans(actor, trans);
    al::resetPosition(actor);
}

void resetRotatePosition(al::LiveActor* actor, const sead::Vector3f& rot,
                         const sead::Vector3f& trans) {
    al::updatePoseRotate(actor, rot);
    al::updatePoseTrans(actor, trans);
    al::resetPosition(actor);
}

void resetQuatPosition(al::LiveActor* actor, const sead::Quatf& quat, const sead::Vector3f& trans) {
    al::updatePoseQuat(actor, quat);
    al::updatePoseTrans(actor, trans);
    al::resetPosition(actor);
}

void resetMtxPosition(al::LiveActor* actor, const sead::Matrix34f& mtx) {
    al::updatePoseMtx(actor, &mtx);
    al::resetPosition(actor);
}

void resetActorPosition(al::LiveActor* actor, const al::LiveActor* target) {
    al::updatePoseMtx(actor, target->getBaseMtx());
    al::resetPosition(actor);
}

// trySetPosOnGround

const sead::Vector3f& getVelocity(const al::LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity();
}

sead::Vector3f* getVelocityPtr(al::LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocityPtr();
}

// separateVelocityHV

}  // namespace al
