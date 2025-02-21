#include "Library/LiveActor/ActorMovementFunction.h"

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/HitSensor/SensorFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Se/SeKeeper.h"
#include "basis/seadTypes.h"

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
    sead::Vector3f* velocity = getVelocityPtr(actor);
    *velocity += vel;
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

// exists to avoid doing this dimension-wise addition in every function
inline void addVelocityInline(LiveActor* actor, const sead::Vector3f& vel, f32 force) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->x = vel.x * force + velocity->x;
    velocity->y = vel.y * force + velocity->y;
    velocity->z = vel.z * force + velocity->z;
}

void addVelocityToFront(LiveActor* actor, f32 force) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    addVelocityInline(actor, front, force);
}

void addVelocityToUp(LiveActor* actor, f32 force) {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    calcUpDir(&up, actor);
    addVelocityInline(actor, up, force);
}

void addVelocityToSide(LiveActor* actor, f32 force) {
    sead::Vector3f side = {0.0f, 0.0f, 0.0f};
    calcSideDir(&side, actor);
    addVelocityInline(actor, side, force);
}

void addVelocityToDown(LiveActor* actor, f32 force) {
    sead::Vector3f down = {0.0f, 0.0f, 0.0f};
    calcDownDir(&down, actor);
    addVelocityInline(actor, down, force);
}
void addVelocityToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force) {
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dir);
    addVelocityInline(actor, normDir, force);
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
    addVelocityInline(actor, gravity, force);
}

void calcGravityDir(sead::Vector3f* gravity, const LiveActor* actor) {
    if (isCollidedGround(actor))
        *gravity = -getOnGroundNormal(actor, 0);
    else
        gravity->set(getGravity(actor));
    tryNormalizeOrZero(gravity);
}

void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force) {
    sead::Vector3f diff = target;
    diff -= getTrans(actor);
    tryNormalizeOrZero(&diff);
    addVelocityInline(actor, diff, force);
}

void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 minForce, f32 maxForce, f32 minDistance, f32 maxDistance) {
    sead::Vector3f diff = target;
    diff -= getTrans(actor);
    f32 distance;
    separateScalarAndDirection(&distance, &diff, diff);
    f32 normDistance = normalize(distance, minDistance, maxDistance);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    f32 force = lerpValue(minForce, maxForce, normDistance);
    velocity->x = diff.x * force + velocity->x;
    velocity->y = diff.y * force + velocity->y;
    velocity->z = diff.z * force + velocity->z;
}

void addVelocityToTargetH(LiveActor* actor, const sead::Vector3f& target, f32 force) {
    sead::Vector3f diff = target;
    diff -= getTrans(actor);
    verticalizeVec(&diff, getGravity(actor), diff);
    tryNormalizeOrZero(&diff);
    addVelocityInline(actor, diff, force);
}

void addVelocityToTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV) {
    sead::Vector3f diff = target;
    diff -= getTrans(actor);
    f32 diffDown = diff.dot(getGravity(actor));
    sead::Vector3f diffV = diffDown * getGravity(actor);
    sead::Vector3f diffH = diff - diffV;
    tryNormalizeOrZero(&diffH);
    tryNormalizeOrZero(&diffV);
    addVelocityInline(actor, diffH, forceH);
    addVelocityInline(actor, diffV, forceV);
}

void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceDamp) {
    sead::Vector3f diff = (target - getTrans(actor)) * forceDamp;
    sead::Vector3f* velocity = getVelocityPtr(actor);
    lerpVec(velocity, *velocity, diff, forceDamp);
}

void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force, f32 damp) {
    sead::Vector3f diff = (target - getTrans(actor)) * force;
    sead::Vector3f* velocity = getVelocityPtr(actor);
    lerpVec(velocity, *velocity, diff, damp);
}

bool addVelocityToPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if(!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityToTarget(actor, playerPos + offset, force);
    return true;
}

bool addVelocityToPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if(!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityToTargetHV(actor, playerPos + offset, forceH, forceV);
    return true;
}

void addVelocityFromTarget(LiveActor* actor, const sead::Vector3f& target, f32 force) {
    sead::Vector3f diff = getTrans(actor);
    diff -= target;
    tryNormalizeOrZero(&diff);
    addVelocityInline(actor, diff, force);
}

void addVelocityFromTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV) {
    sead::Vector3f diff = getTrans(actor);
    diff -= target;
    f32 diffDown = diff.dot(getGravity(actor));
    sead::Vector3f diffV = diffDown * getGravity(actor);
    sead::Vector3f diffH = diff - diffV;
    tryNormalizeOrZero(&diffH);
    tryNormalizeOrZero(&diffV);
    addVelocityInline(actor, diffH, forceH);
    addVelocityInline(actor, diffV, forceV);
}

bool addVelocityFromPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if(!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityFromTarget(actor, playerPos + offset, force);
    return true;
}

bool addVelocityFromPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if(!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityFromTargetHV(actor, playerPos + offset, forceH, forceV);
    return true;
}

void addVelocityClockwiseToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force) {
    sead::Vector3f dirVelocity;
    if(!calcVelocityClockwiseToDirection(actor, &dirVelocity, dir))
        return;
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dirVelocity);
    addVelocityInline(actor, normDir, force);
}

bool calcVelocityClockwiseToDirection(LiveActor* actor, sead::Vector3f* dirVelocity, const sead::Vector3f& dir) {
    sead::Vector3f normDir;
    if(!dirVelocity || !tryNormalizeOrZero(&normDir, dir))
        return false;

    dirVelocity->setCross(getGravity(actor), normDir);
    return true;
}

void addVelocityClockwiseToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force) {
    addVelocityClockwiseToDirection(actor, target - getTrans(actor), force);
}

void addVelocityJumpGroundInertia(LiveActor* actor, const sead::Vector3f& velocity, f32 force) {
    sead::Vector3f inertia = {0.0f, 0.0f, 0.0f};
    calcJumpInertia(&inertia, actor, velocity, force);
    addVelocity(actor, inertia);
}

void tryAddVelocityLimit(LiveActor* actor, const sead::Vector3f& velocity, f32 limit) {
    sead::Vector3f newVelocity = getVelocity(actor);
    addVectorLimit(&newVelocity, velocity, limit);
    setVelocity(actor, newVelocity);
}

void subVelocityExceptDirectionLimit(LiveActor* actor, const sead::Vector3f& direction, f32 subVel, f32 limit) {
    sead::Vector3f horizontal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityDirHV(&horizontal, &vertical, actor, direction);

    f32 length = horizontal.length();
    f32 newLen = sead::Mathf::clampMin(length - subVel, limit);
    f32 lenAgain = horizontal.length();
    if (lenAgain > 0.0f)
        horizontal *= newLen / lenAgain;

    setVelocity(actor, horizontal + vertical);
}

void scaleVelocity(LiveActor* actor, f32 factor) {
    *getVelocityPtr(actor) *= factor;
}

void scaleVelocityLimit(LiveActor* actor, f32 factor, f32 limit) {
    sead::Vector3f direction = {0.0f, 0.0f, 0.0f};
    f32 scalar = 0.0f;
    if(separateScalarAndDirection(&scalar, &direction, getVelocity(actor)))
        return;

    // enforces a *lower* limit = minimum speed!
    if(scalar < limit)
        return;
    scalar = sead::Mathf::clampMin(scalar * factor, limit);
    setVelocity(actor, scalar * direction);
}

void scaleVelocityX(LiveActor* actor, f32 factorX) {
    getVelocityPtr(actor)->x *= factorX;
}

void scaleVelocityY(LiveActor* actor, f32 factorY) {
    getVelocityPtr(actor)->y *= factorY;
}

void scaleVelocityZ(LiveActor* actor, f32 factorZ) {
    getVelocityPtr(actor)->z *= factorZ;
}

// NON_MATCHING: mismatch in inlined function
void scaleVelocityHV(LiveActor* actor, f32 factorH, f32 factorV) {
    //scaleVelocityParallelVertical(actor, getGravity(actor), factorV, factorH);
    const sead::Vector3f& direction = getGravity(actor);
    const sead::Vector3f& velocity = getVelocity(actor);

    f32 speedV = direction.dot(velocity);
    sead::Vector3f parallelVec = direction * (speedV * factorV);
    sead::Vector3f verticalVec = velocity;
    verticalVec.x -= direction.x*speedV;
    verticalVec.y -= direction.y*speedV;
    verticalVec.z -= direction.z*speedV;

    sead::Vector3f* newVelocity = getVelocityPtr(actor);
    newVelocity->x = parallelVec.x + verticalVec.x * factorH;
    newVelocity->y = parallelVec.y + verticalVec.y * factorH;
    newVelocity->z = parallelVec.z + verticalVec.z * factorH;
}

void scaleVelocityDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    scaleVectorDirection(velocity, direction, *velocity, factor);
}

void scaleVelocityExceptDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    scaleVectorExceptDirection(velocity, direction, *velocity, factor);
}

void scaleVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& direction, f32 parallel, f32 vertical) {
    const sead::Vector3f& velocity = getVelocity(actor);

    f32 speedV = direction.dot(velocity);
    sead::Vector3f parallelVec = direction * (speedV * parallel);
    sead::Vector3f verticalVec = velocity;
    verticalVec.x -= direction.x*speedV;
    verticalVec.y -= direction.y*speedV;
    verticalVec.z -= direction.z*speedV;

    sead::Vector3f* newVelocity = getVelocityPtr(actor);
    newVelocity->x = parallelVec.x + verticalVec.x * vertical;
    newVelocity->y = parallelVec.y + verticalVec.y * vertical;
    newVelocity->z = parallelVec.z + verticalVec.z * vertical;
}

void limitVelocity(LiveActor* actor, f32 limit) {
    if(calcSpeed(actor) > limit) {
        tryNormalizeOrZero(getVelocityPtr(actor));
        scaleVelocity(actor, limit);
    }
}

f32 calcSpeed(const LiveActor* actor) {
    return getVelocity(actor).length();
}

void limitVelocityX(LiveActor* actor, f32 limitX) {
    if (getVelocity(actor).x > limitX)
        getVelocityPtr(actor)->x = limitX;
    else if (getVelocity(actor).x < -limitX)
        getVelocityPtr(actor)->x = -limitX;
}

void limitVelocityY(LiveActor* actor, f32 limitY) {
    if (getVelocity(actor).y > limitY)
        getVelocityPtr(actor)->y = limitY;
    else if (getVelocity(actor).y < -limitY)
        getVelocityPtr(actor)->y = -limitY;
}

void limitVelocityZ(LiveActor* actor, f32 limitZ) {
    if (getVelocity(actor).z > limitZ)
        getVelocityPtr(actor)->z = limitZ;
    else if (getVelocity(actor).z < -limitZ)
        getVelocityPtr(actor)->z = -limitZ;
}

void limitVelocityH(LiveActor* actor, f32 limitH) {
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    sead::Vector3f horizontal = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&vertical, &horizontal, actor, getGravity(actor));
    if (horizontal.squaredLength() > sead::Mathf::square(limitH)) {
        f32 length = horizontal.length();
        if(length > 0.0f)
            horizontal *= limitH / length;
    }
    setVelocity(actor, vertical + horizontal);
}

void limitVelocityHV(LiveActor* actor, f32 limitH, f32 limitV) {
    sead::Vector3f horizontal, vertical;
    separateVelocityHV(&horizontal, &vertical, actor);
    if (horizontal.squaredLength() > sead::Mathf::square(limitH)) {
        f32 length = horizontal.length();
        if(length > 0.0f)
            horizontal *= limitH / length;
    }
    if (vertical.squaredLength() > sead::Mathf::square(limitV)) {
        f32 length = vertical.length();
        if(length > 0.0f)
            vertical *= limitV / length;
    }
    setVelocity(actor, horizontal + vertical);
}

void limitVelocityUpGravityH(LiveActor* actor, f32 limitDown, f32 limitUp, f32 limitH) {
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    sead::Vector3f horizontal = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&vertical, &horizontal, actor, getGravity(actor));
    if (horizontal.squaredLength() > sead::Mathf::square(limitH)) {
        f32 length = horizontal.length();
        if(length > 0.0f)
            horizontal *= limitH / length;
    }

    f32 speedV = vertical.dot(getGravity(actor));
    if (speedV > limitUp) {
        vertical = getGravity(actor) * limitUp;
    }
    f32 x = -limitDown;
    if(speedV < x) {
        vertical = getGravity(actor) * x;
    }
    setVelocity(actor, vertical + horizontal);
}

void limitVelocityDir(LiveActor* actor, const sead::Vector3f& dir, f32 limit) {
    sead::Vector3f parallel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&parallel, &vertical, actor, dir);
    if (parallel.squaredLength() > sead::Mathf::square(limit)) {
        f32 length = parallel.length();
        if(length > 0.0f)
            parallel *= limit / length;
    }
    setVelocity(actor, parallel + vertical);
}

void limitVelocityDirSign(LiveActor* actor, const sead::Vector3f& dir, f32 limit) {
    sead::Vector3f parallel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&parallel, &vertical, actor, dir);
    if (dir.dot(parallel) < 0.0f)
        return;

    if (parallel.squaredLength() > sead::Mathf::square(limit)) {
        f32 length = parallel.length();
        if(length > 0.0f)
            parallel *= limit / length;
    }
    setVelocity(actor, parallel + vertical);
}

void limitVelocityDirV(LiveActor* actor, const sead::Vector3f& dir, f32 limit) {
    sead::Vector3f parallel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&parallel, &vertical, actor, dir);
    if (vertical.squaredLength() > sead::Mathf::square(limit)) {
        f32 length = vertical.length();
        if(length > 0.0f)
            vertical *= limit / length;
    }
    setVelocity(actor, parallel + vertical);
}

void limitVelocityDirVRate(LiveActor* actor, const sead::Vector3f& dir, f32 limit, f32 rate) {
    sead::Vector3f parallel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&parallel, &vertical, actor, dir);
    if (vertical.squaredLength() > sead::Mathf::square(limit)) {
        f32 length1 = vertical.length();
        f32 length = vertical.length();
        if(length > 0.0f)
            vertical *= ((length1 - limit) * rate + limit) / length;
    }
    setVelocity(actor, parallel + vertical);
}

void limitVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& dir, f32 parallel, f32 vertical) {
    limitVectorParallelVertical(getVelocityPtr(actor), dir, parallel, vertical);
}

void limitVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& dir, f32 horizontal, f32 vertical) {
    limitVectorSeparateHV(getVelocityPtr(actor), dir, horizontal, vertical);
}

u32 reboundVelocityPart(LiveActor* actor, f32 rebound, f32 threshold) {
    return reboundVelocityPart(actor, rebound, rebound, rebound, threshold);
}

u32 reboundVelocityPart(LiveActor* actor, f32 ground, f32 wall, f32 ceiling, f32 threshold) {
    u32 result = 0;
    if(isCollidedGround(actor)) {
        sead::Vector3f normal = getCollidedGroundNormal(actor);
        f32 dot = normal.dot(getVelocity(actor));
        if (ground < 0.0f || dot < -threshold) {
            addVelocity(actor, -(normal * ((ground + 1.0f) * dot)));
            if (ground >= 0.0f)
                result |= 1;
        } else if(dot < 0.0f) {
            addVelocity(actor, -(normal * dot));
        }
    }

    if(isCollidedWall(actor)) {
        sead::Vector3f normal = getCollidedWallNormal(actor);
        f32 dot = normal.dot(getVelocity(actor));
        if (wall < 0.0f || dot < -threshold) {
            addVelocity(actor, -(normal * ((wall + 1.0f) * dot)));
            if (wall >= 0.0f)
                result |= 2;
        } else if(dot < 0.0f) {
            addVelocity(actor, -(normal * dot));
        }
    }

    if(isCollidedCeiling(actor)) {
        sead::Vector3f normal = getCollidedCeilingNormal(actor);
        f32 dot = normal.dot(getVelocity(actor));
        if (ceiling < 0.0f || dot < -threshold) {
            addVelocity(actor, -(normal * ((ceiling + 1.0f) * dot)));
            if (ceiling >= 0.0f)
                result |= 4;
        } else if(dot < 0.0f) {
            addVelocity(actor, -(normal * dot));
        }
    }

    return result;
}

bool reboundVelocityFromEachCollision(LiveActor* actor, f32 ground, f32 wall, f32 ceiling, f32 threshold) {
    if(!isCollided(actor))
        return false;

    sead::Vector3f normalSum;
    calcCollidedNormalSum(actor, &normalSum);
    if(isNearZero(normalSum, 0.001f))
        return false;

    normalize(&normalSum);
    const sead::Vector3f& gravity = getGravity(actor);
    f32 rebound;
    if(isFloorPolygon(normalSum, gravity))
        rebound = ground;
    else if(isWallPolygon(normalSum, gravity))
        rebound = wall;
    else if(isCeilingPolygon(normalSum, gravity))
        rebound = ceiling;
    else
        rebound = 0.0f;

    f32 dot = normalSum.dot(getVelocity(actor));
    if (dot < -threshold) {
        sead::Vector3f* velocity = getVelocityPtr(actor);
        f32 mul = (rebound + 1.0f) * dot;
        velocity->x -= normalSum.x * mul;
        velocity->y -= normalSum.y * mul;
        velocity->z -= normalSum.z * mul;
        return true;
    } else if(dot < 0.0f) {
        sead::Vector3f* velocity = getVelocityPtr(actor);
        velocity->x -= normalSum.x * dot;
        velocity->y -= normalSum.y * dot;
        velocity->z -= normalSum.z * dot;
    }
    return false;
}

bool reboundVelocityFromCollision(LiveActor* actor, f32 a, f32 b, f32 c) {
    if(!isCollided(actor))
        return false;

    sead::Vector3f normalSum;
    calcCollidedNormalSum(actor, &normalSum);
    if(isNearZero(normalSum, 0.001f))
        return false;

    normalize(&normalSum);
    f32 dot = normalSum.dot(getVelocity(actor));
    if (dot < -b) {
        *getVelocityPtr(actor) -= normalSum * dot;
        *getVelocityPtr(actor) *= c;
        *getVelocityPtr(actor) -= normalSum * dot * a;
        return true;
    } else if(dot < 0.0f) {
        *getVelocityPtr(actor) -= normalSum * dot;
    }
    return false;
}
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
