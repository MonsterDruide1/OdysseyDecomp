#include "Library/LiveActor/ActorMovementFunction.h"

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/HitSensor/SensorFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Se/SeKeeper.h"

namespace al {

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
    if (actor->getAudioKeeper() && actor->getAudioKeeper()->getSeKeeper())
        actor->getAudioKeeper()->getSeKeeper()->resetPosition();
}

void resetPosition(LiveActor* actor, const sead::Vector3f& trans) {
    updatePoseTrans(actor, trans);
    resetPosition(actor);
}

void resetRotatePosition(LiveActor* actor, const sead::Vector3f& rot, const sead::Vector3f& trans) {
    updatePoseRotate(actor, rot);
    resetPosition(actor, trans);
}

void resetQuatPosition(LiveActor* actor, const sead::Quatf& quat, const sead::Vector3f& trans) {
    updatePoseQuat(actor, quat);
    resetPosition(actor, trans);
}

void resetMtxPosition(LiveActor* actor, const sead::Matrix34f& mtx) {
    updatePoseMtx(actor, mtx);
    resetPosition(actor);
}

void resetActorPosition(LiveActor* actor, const LiveActor* target) {
    resetMtxPosition(actor, *target->getBaseMtx());
}

bool trySetPosOnGround(LiveActor* actor) {
    sead::Vector3f pos = getTrans(actor);
    sead::Vector3f dir = {0.0f, -500.0f, 0.0f};
    pos.y += 200.0f;
    return alCollisionUtil::getFirstPolyOnArrow(actor, getTransPtr(actor), nullptr, pos, dir, nullptr, nullptr);
}

const sead::Vector3f& getVelocity(const LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity();
}

sead::Vector3f* getVelocityPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocityPtr();
}

void separateVelocityHV(sead::Vector3f* horizontal, sead::Vector3f* vertical, const LiveActor* actor) {
    separateVelocityDirHV(horizontal, vertical, actor, getGravity(actor));
}

void separateVelocityDirHV(sead::Vector3f* horizontal, sead::Vector3f* vertical, const LiveActor* actor, const sead::Vector3f& dir) {
    separateVectorHV(horizontal, vertical, dir, getVelocity(actor));
}

void separateVelocityParallelVertical(sead::Vector3f* parallel, sead::Vector3f* vertical, const LiveActor* actor,  const sead::Vector3f& dir) {
    separateVectorParallelVertical(parallel, vertical, dir, getVelocity(actor));
}

void setVelocity(LiveActor* actor, const sead::Vector3f& vel) {
    getVelocityPtr(actor)->set(vel);
}

void setVelocity(LiveActor* actor, f32 x, f32 y, f32 z) {
    getVelocityPtr(actor)->set(x, y, z);
}

void setVelocityX(LiveActor* actor, f32 x) {
    getVelocityPtr(actor)->x = x;
}

void setVelocityY(LiveActor* actor, f32 y) {
    getVelocityPtr(actor)->y = y;
}

void setVelocityZ(LiveActor* actor, f32 z) {
    getVelocityPtr(actor)->z = z;
}

void setVelocityZero(LiveActor* actor) {
    setVelocity(actor, 0.0f, 0.0f, 0.0f);
}

void setVelocityZeroX(LiveActor* actor) {
    setVelocityX(actor, 0.0f);
}

void setVelocityZeroY(LiveActor* actor) {
    setVelocityY(actor, 0.0f);
}
void setVelocityZeroZ(LiveActor* actor) {
    setVelocityZ(actor, 0.0f);
}

void setVelocityZeroH(LiveActor* actor) {
    setVelocityZeroH(actor, getGravity(actor));
}

void setVelocityZeroH(LiveActor* actor, const sead::Vector3f& gravity) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    parallelizeVec(velocity, gravity, *velocity);
}

void setVelocityZeroV(LiveActor* actor) {
    setVelocityZeroV(actor, getGravity(actor));
}

void setVelocityZeroV(LiveActor* actor, const sead::Vector3f& gravity) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    verticalizeVec(velocity, gravity, *velocity);
}

void setVelocityJump(LiveActor* actor, f32 speed) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    const sead::Vector3f& gravity = getGravity(actor);
    speed = -speed;
    velocity->x = gravity.x * speed;
    velocity->y = gravity.y * speed;
    velocity->z = gravity.z * speed;
}

void setVelocityToFront(LiveActor* actor, f32 speed) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&front, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = front.x * speed;
    velocity->y = front.y * speed;
    velocity->z = front.z * speed;
}

void setVelocityToUp(LiveActor* actor, f32 speed) {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    calcUpDir(&up, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = up.x * speed;
    velocity->y = up.y * speed;
    velocity->z = up.z * speed;
}

void setVelocityToSide(LiveActor* actor, f32 speed) {
    sead::Vector3f side = {0.0f, 0.0f, 0.0f};
    calcSideDir(&side, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = side.x * speed;
    velocity->y = side.y * speed;
    velocity->z = side.z * speed;
}

void setVelocityToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 speed) {
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dir);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = normDir.x * speed;
    velocity->y = normDir.y * speed;
    velocity->z = normDir.z * speed;
}

void setVelocityToGravity(LiveActor* actor, f32 speed) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    const sead::Vector3f& gravity = getGravity(actor);
    velocity->x = gravity.x * speed;
    velocity->y = gravity.y * speed;
    velocity->z = gravity.z * speed;
}
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, const sead::Vector3f& v, f32 speedH, f32 speedV) {
    calcVectorSeparateHV(getVelocityPtr(actor), h, v, speedH, speedV);
}

void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV) {
    calcVelocitySeparateHV(getVelocityPtr(actor), actor, h, speedH, speedV);
}

void calcVelocitySeparateHV(sead::Vector3f* velocity, const LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV) {
    calcVectorSeparateHV(velocity, h, -getGravity(actor), speedH, speedV);
}

void setVelocitySeparateUp(LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV) {
    sead::Vector3f up;
    calcUpDir(&up, actor);
    setVelocitySeparateHV(actor, h, up, speedH, speedV);
}

void setVelocityOnlyDir(LiveActor* actor, const sead::Vector3f& dir, f32 speed) {
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dir);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    *velocity += normDir * (speed - normDir.dot(getVelocity(actor)));
}

void setVelocityOnlyV(LiveActor* actor, f32 speed) {
    setVelocityOnlyDir(actor, getGravity(actor), -speed);
}

void setVelocityOnlyGravity(LiveActor* actor, f32 speed) {
    setVelocityOnlyDir(actor, getGravity(actor), speed);
}

void addVelocity(LiveActor* actor, const sead::Vector3f& vel) {
    *getVelocityPtr(actor) += vel;
}

void addVelocity(LiveActor* actor, f32 x, f32 y, f32 z) {
    addVelocity(actor, {x, y, z});
}

void addVelocityX(LiveActor* actor, f32 x) {
    getVelocityPtr(actor)->x += x;
}

void addVelocityY(LiveActor* actor, f32 y) {
    getVelocityPtr(actor)->y += y;
}

void addVelocityZ(LiveActor* actor, f32 z) {
    getVelocityPtr(actor)->z += z;
}

void addVelocityDump(LiveActor* actor, const sead::Vector3f& dir, f32 force) {
    getVelocity(actor);  // unused
    sead::Vector3f accel = dir - getVelocity(actor);
    sead::Vector3f add;
    lerpVec(&add, dir, accel, force);
    addVelocity(actor, add);
}

void addVelocityJump(LiveActor* actor, f32 force) {
    addVelocity(actor, -(getGravity(actor) * force));
}

void addVelocityToFront(LiveActor* actor, f32 force) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = front.x * force + velocity->x;
    velocity->y = front.y * force + velocity->y;
    velocity->z = front.z * force + velocity->z;
}

void addVelocityToUp(LiveActor* actor, f32 force) {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    calcUpDir(&up, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = up.x * force + velocity->x;
    velocity->y = up.y * force + velocity->y;
    velocity->z = up.z * force + velocity->z;
}

void addVelocityToSide(LiveActor* actor, f32 force) {
    sead::Vector3f side = {0.0f, 0.0f, 0.0f};
    calcSideDir(&side, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = side.x * force + velocity->x;
    velocity->y = side.y * force + velocity->y;
    velocity->z = side.z * force + velocity->z;
}

void addVelocityToDown(LiveActor* actor, f32 force) {
    sead::Vector3f down = {0.0f, 0.0f, 0.0f};
    calcDownDir(&down, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = down.x * force + velocity->x;
    velocity->y = down.y * force + velocity->y;
    velocity->z = down.z * force + velocity->z;
}
void addVelocityToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force) {
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dir);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = normDir.x * force + velocity->x;
    velocity->y = normDir.y * force + velocity->y;
    velocity->z = normDir.z * force + velocity->z;
}
void addVelocityToGravity(LiveActor* actor, f32 force) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    const sead::Vector3f& gravity = getGravity(actor);
    velocity->x = gravity.x * force + velocity->x;
    velocity->y = gravity.y * force + velocity->y;
    velocity->z = gravity.z * force + velocity->z;
}

void addVelocityToGravityLimit(LiveActor* actor, f32 force, f32 limit) {
    addVectorLimit(getVelocityPtr(actor), getGravity(actor) * force, limit);
}

void addVelocityToGravityFittedGround(LiveActor* actor, f32 force, u32 maxAirTime) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    const sead::Vector3f& normal = getOnGroundNormal(actor, maxAirTime);
    velocity->x -= normal.x * force;
    velocity->y -= normal.y * force;
    velocity->z -= normal.z * force;
}

void addVelocityToGravityNaturalOrFittedGround(LiveActor* actor, f32 force) {
    sead::Vector3f gravity = sead::Vector3f::zero;
    calcGravityDir(&gravity, actor);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = gravity.x * force + velocity->x;
    velocity->y = gravity.y * force + velocity->y;
    velocity->z = gravity.z * force + velocity->z;
}

void calcGravityDir(sead::Vector3f* gravity, const LiveActor* actor) {
    if (isCollidedGround(actor))
        *gravity = -getOnGroundNormal(actor, 0);
    else
        gravity->set(getGravity(actor));
    tryNormalizeOrZero(gravity);
}
// void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
// void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 minForce, f32 maxForce, f32 minDistance, f32 maxDistance);
// void addVelocityToTargetH(LiveActor* actor, const sead::Vector3f& target, f32 force);
// void addVelocityToTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV);
// void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceDamp);
// void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force, f32 damp);
// void addVelocityToPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset);
// void addVelocityToPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset);
// void addVelocityFromTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
// void addVelocityFromTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV);
// void addVelocityFromPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset);
// void addVelocityFromPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset);
// void addVelocityClockwiseToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force);
// void calcVelocityClockwiseToDirection(LiveActor* actor, sead::Vector3f* dirVelocity, const sead::Vector3f& dir);
// void addVelocityClockwiseToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
// void addVelocityJumpGroundInertia(LiveActor* actor, const sead::Vector3f& velocity, f32 force);
// bool tryAddVelocityLimit(LiveActor* actor, const sead::Vector3f& velocity, f32 limit);
// void subVelocityExceptDirectionLimit(LiveActor* actor, const sead::Vector3f& direction, f32 subVel, f32 limit);
// void scaleVelocity(LiveActor* actor, f32 factor);
// void scaleVelocityLimit(LiveActor* actor, f32 factor, f32 limit);
// void scaleVelocityX(LiveActor* actor, f32 factorX);
// void scaleVelocityY(LiveActor* actor, f32 factorY);
// void scaleVelocityZ(LiveActor* actor, f32 factorZ);
// void scaleVelocityHV(LiveActor* actor, f32 factorH, f32 factorV);
// void scaleVelocityDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor);
// void scaleVelocityExceptDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor);
// void scaleVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& direction, f32 parallel, f32 vertical);
// void limitVelocity(LiveActor* actor, f32 limit);
// f32 calcSpeed(const LiveActor* actor);
// void limitVelocityX(LiveActor* actor, f32 limitX);
// void limitVelocityY(LiveActor* actor, f32 limitY);
// void limitVelocityZ(LiveActor* actor, f32 limitZ);
// void limitVelocityH(LiveActor* actor, f32 limitH);
// void limitVelocityHV(LiveActor* actor, f32 limitH, f32 limitV);
// void limitVelocityUpGravityH(LiveActor* actor, f32, f32, f32);
// void limitVelocityDir(LiveActor* actor, const sead::Vector3f&, f32);
// void limitVelocityDirSign(LiveActor* actor, const sead::Vector3f&, f32);
// void limitVelocityDirV(LiveActor* actor, const sead::Vector3f&, f32);
// void limitVelocityDirVRate(LiveActor* actor, const sead::Vector3f&, f32, f32);
// void limitVelocityParallelVertical(LiveActor* actor, const sead::Vector3f&, f32, f32);
// void limitVelocitySeparateHV(LiveActor* actor, const sead::Vector3f&, f32, f32);
// void reboundVelocityPart(LiveActor* actor, f32, f32);
// void reboundVelocityPart(LiveActor* actor, f32, f32, f32, f32);
// void reboundVelocityFromEachCollision(LiveActor* actor, f32, f32, f32, f32);
// void reboundVelocityFromCollision(LiveActor* actor, f32, f32, f32);
// void reboundVelocityFromTriangles(LiveActor* actor, f32, f32);
// void reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, f32);
// void reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, const sead::Vector3f&,                              f32);
// void reboundVelocityFromSensor(LiveActor* actor, const HitSensor*, f32);
// void reboundVelocityFromSensor(LiveActor* actor, const HitSensor*, const sead::Vector3f&, f32);
// void calcDirToActor(sead::Vector3f*, const LiveActor* actor, const LiveActor* target);
// void reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target, f32);
// void reboundVelocityBetweenSensor(HitSensor*, HitSensor*, f32);
// void calcVelocityKeepLengthBetweenActor(sead::Vector3f*, const LiveActor* actor,                                        const LiveActor* target, f32, f32);
// void addVelocityKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32, f32);
// void addVelocityDumpKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32, f32, f32);
// void calcVelocityBlowAttack(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&, f32,                            f32);
// void addVelocityBlowAttack(LiveActor* actor, const sead::Vector3f&, f32, f32);
// void addVelocityBlowAttack(LiveActor* actor, const HitSensor*, f32, f32);
// void setVelocityBlowAttack(LiveActor* actor, const sead::Vector3f&, f32, f32);
// void setVelocityBlowAttack(LiveActor* actor, const HitSensor*, f32, f32);
// void setVelocityBlowAttackAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32);
// bool isVelocityFast(const LiveActor* actor, f32);
// bool isVelocityFastH(const LiveActor* actor, f32);
// bool isVelocitySlow(const LiveActor* actor, f32);
// bool isVelocitySlowH(const LiveActor* actor, f32);
// f32 calcSpeedH(const LiveActor* actor);
// f32 calcSpeedV(const LiveActor* actor);
// void calcSpeedDirection(const LiveActor* actor, const sead::Vector3f&);
// void calcSpeedExceptDir(const LiveActor* actor, const sead::Vector3f&);
// bool isNear(const LiveActor* actor, const LiveActor* target, f32);
// bool isNear(const LiveActor* actor, const sead::Vector3f&, f32);
// bool isNearXZ(const LiveActor* actor, const sead::Vector3f&, f32);
// bool isNearH(const LiveActor* actor, const sead::Vector3f&, f32);
// f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f&);
// bool isNearV(const LiveActor* actor, const sead::Vector3f&, f32);
// f32 calcDistanceV(const LiveActor* actor, const sead::Vector3f&);
// bool isNearHV(const LiveActor* actor, const sead::Vector3f&, f32, f32);
// bool isNearHV(const LiveActor* actor, const sead::Vector3f&, f32, f32, f32);
// void calcHeight(const LiveActor* actor, const sead::Vector3f&);
// bool isFar(const LiveActor* actor, const LiveActor* target, f32);
// bool isFar(const LiveActor* actor, const sead::Vector3f&, f32);
// f32 calcDistance(const LiveActor* actor, const LiveActor* target);
// f32 calcDistance(const LiveActor* actor, const sead::Vector3f&);
// f32 calcDistanceV(const LiveActor* actor, const LiveActor* target);
// f32 calcDistanceH(const LiveActor* actor, const LiveActor* target);
// f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&);
// f32 calcHeight(const LiveActor* actor, const LiveActor* target);
// void calcDistanceFront(const LiveActor* actor, const sead::Vector3f&);
// void calcDistanceFront(const LiveActor* actor, const LiveActor* target);
// void addRotateAndRepeatX(LiveActor* actor, f32);
// void addRotateAndRepeatY(LiveActor* actor, f32);
// void addRotateAndRepeatZ(LiveActor* actor, f32);
// void addRandomRotateY(LiveActor* actor);
// void calcQuatSide(sead::Vector3f*, const LiveActor* actor);
// void calcQuatUp(sead::Vector3f*, const LiveActor* actor);
// void calcQuatUp(sead::Vector3f*, const sead::Quatf& quat);
// void calcQuatFront(sead::Vector3f*, const LiveActor* actor);
// void calcQuatLocalAxis(sead::Vector3f*, const LiveActor* actor, s32);
// void calcQuatLocalAxis(sead::Vector3f*, const sead::Quatf&, s32);
// void calcTransOffsetFront(sead::Vector3f*, const LiveActor* actor, f32);
// void calcTransOffsetUp(sead::Vector3f*, const LiveActor* actor, f32);
// void calcTransOffsetSide(sead::Vector3f*, const LiveActor* actor, f32);
// void setTransOffsetLocalDir(LiveActor* actor, const sead::Quatf&, const sead::Vector3f&, f32, s32);
// void addTransOffsetLocal(LiveActor* actor, const sead::Vector3f&);
// void addTransOffsetLocalDir(LiveActor* actor, f32, s32);
// void rotateQuatXDirDegree(LiveActor* actor, f32);
// void rotateQuatXDirDegree(LiveActor* actor, const sead::Quatf&, f32);
// void rotateQuatYDirDegree(LiveActor* actor, f32);
// void rotateQuatYDirDegree(LiveActor* actor, const sead::Quatf&, f32);
// void rotateQuatZDirDegree(LiveActor* actor, f32);
// void rotateQuatZDirDegree(LiveActor* actor, const sead::Quatf&, f32);
// void rotateQuatLocalDirDegree(LiveActor* actor, s32, f32);
// void rotateQuatLocalDirDegree(LiveActor* actor, const sead::Quatf&, s32, f32);
// void rotateQuatYDirRandomDegree(LiveActor* actor);
// void rotateQuatYDirRandomDegree(LiveActor* actor, const sead::Quatf&);
// void turnQuatFrontToDirDegreeH(LiveActor* actor, const sead::Vector3f&, f32);
// void turnQuatFrontToPosDegreeH(LiveActor* actor, const sead::Vector3f&, f32);
// void turnQuatFrontFromPosDegreeH(LiveActor* actor, const sead::Vector3f&, f32);
// void turnFront(LiveActor* actor, f32);
// void turnFront(LiveActor* actor, const sead::Vector3f&, f32);
// void turnFrontToPos(LiveActor* actor, const sead::Vector3f&, f32);
// void turnFrontToDir(LiveActor* actor, const sead::Vector3f&, f32);
// void turnFrontToDirGetIsFinished(LiveActor* actor, const sead::Vector3f&, f32);
// void turnDirectionDegree(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
// void turnFrontToTarget(LiveActor* actor, const LiveActor* target, f32);
// void turnFrontFromTarget(LiveActor* actor, const LiveActor* target, f32);
// void turnFrontToPlayer(LiveActor* actor, f32);
// void turnFrontFromPlayer(LiveActor* actor, f32);
// void turnDirection(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
// void turnDirectionToTarget(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
// void turnDirectionToTargetDegree(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&,                                 f32);
// void turnDirectionFromTargetDegree(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&,                                   f32);
// void turnDirectionAlongGround(const LiveActor* actor, sead::Vector3f*);
// void turnDirectionAlongGround(LiveActor* actor);
// void turnToDirectionAxis(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32);
// void turnFrontSpherical(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
// void turnFrontSphericalToTarget(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&,                                f32);
// void turnFrontSphericalToTargetDegree(const LiveActor* actor, sead::Vector3f*,                                      const sead::Vector3f&, f32);
// void turnToDirection(LiveActor* actor, const sead::Vector3f&, f32);
// void turnLocalDirToDirection(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32);
// void turnToTarget(LiveActor* actor, const sead::Vector3f&, f32);
// void turnToTarget(LiveActor* actor, const LiveActor* target, f32);
// void faceToDirection(LiveActor* actor, const sead::Vector3f&);
// void faceToDirectionSupportUp(LiveActor* actor, const sead::Vector3f&);
// void faceToTarget(LiveActor* actor, const sead::Vector3f&);
// void faceToTarget(LiveActor* actor, const LiveActor* target);
// void faceToSensor(LiveActor* actor, const HitSensor*);
// void faceToVelocity(LiveActor* actor);
// void calcDirClockwiseToDir(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
// void calcDirClockwiseToPos(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
// void calcDirToActorH(sead::Vector3f*, const LiveActor* actor, const LiveActor* target);
// void calcDirToActorH(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
// void calcAngleToTargetH(const LiveActor* actor, const sead::Vector3f&);
// void calcAngleToTargetV(const LiveActor* actor, const sead::Vector3f&);
// bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,                          f32);
// bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f&, f32);
// bool isFaceToTargetDegreeHV(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,                            f32, f32);
// bool isFaceToTargetDegreeH(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,                           f32);
// bool isInSightCone(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
// bool isInSightConeTarget(const LiveActor* actor, const LiveActor* target, f32, f32);
// bool isInSightConePlayer(const LiveActor* actor, f32, f32);
// bool isInSightFan(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32, f32,                  f32);
// bool isInSightFanTarget(const LiveActor* actor, const LiveActor* target, f32, f32, f32);
// bool isInSightBox(const LiveActor* actor, const sead::Vector3f&, const sead::BoundBox3f&);
// void walkAndTurnToDirection(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32, bool);
// void walkAndTurnToDirection(LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32, f32, f32,                            f32, bool);
// void walkAndTurnPoseToDirection(LiveActor* actor, const sead::Vector3f&, const ActorParamMove&,                                bool);
// void walkAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32, bool);
// void flyAndTurnToDirection(LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32, f32, f32,                           f32);
// void flyAndTurnToDirection(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32);
// void flyAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32);
// void walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, sead::Vector3f*,                                               const sead::Vector3f&, f32, f32, f32, f32, bool);
// void walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, const sead::Vector3f&, f32, f32,                                               f32, f32, bool);
// void walkAndTurnToTargetFittedGroundGravity(LiveActor* actor, const sead::Vector3f&, f32, f32, f32,                                            f32, bool);
// bool tryKillByDeathArea(LiveActor* actor);
// void rotateAndKeepColliderPosRate(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,                                  f32);
// void calcSpringMovement(LiveActor* actor, const sead::Vector3f&, f32, f32, const sead::Vector3f&,                        f32, f32);
// void addVelocityClockwiseToPlayer(LiveActor* actor, f32);
// void calcDirClockwiseToPlayer(sead::Vector3f*, const LiveActor* actor);
// void flyAndTurnToPlayer(LiveActor* actor, const ActorParamMove&);
// void escapeFromPlayer(LiveActor* actor, f32, f32, f32, f32);
// void escapeFromPlayer(LiveActor* actor, sead::Vector3f*, f32, f32, f32, f32);
// void walkAndTurnToPlayer(LiveActor* actor, f32, f32, f32, f32, bool);
// bool isPlayerInSightFan(const LiveActor* actor, f32, f32, f32);
// bool isFallOrDamageCodeNextMove(const LiveActor* actor, const sead::Vector3f&, f32, f32);
// bool isFallNextMove(const LiveActor* actor, const sead::Vector3f&, f32, f32);

}  // namespace al
