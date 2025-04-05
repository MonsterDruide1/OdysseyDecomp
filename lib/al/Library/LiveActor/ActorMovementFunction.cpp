#include "Library/LiveActor/ActorMovementFunction.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/HitSensor/SensorFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorParamMove.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Se/SeKeeper.h"

namespace al {

inline f32 modDegree(f32 deg) {
    return modf(deg + 360.0f, 360.0f) + 0.0f;
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
    updatePoseMtx(actor, &mtx);
    resetPosition(actor);
}

void resetActorPosition(LiveActor* actor, const LiveActor* target) {
    resetMtxPosition(actor, *target->getBaseMtx());
}

bool trySetPosOnGround(LiveActor* actor) {
    sead::Vector3f pos = getTrans(actor);
    sead::Vector3f dir = {0.0f, -500.0f, 0.0f};
    pos.y += 200.0f;
    return alCollisionUtil::getFirstPolyOnArrow(actor, getTransPtr(actor), nullptr, pos, dir,
                                                nullptr, nullptr);
}

const sead::Vector3f& getVelocity(const LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocity();
}

sead::Vector3f* getVelocityPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getVelocityPtr();
}

void separateVelocityHV(sead::Vector3f* horizontal, sead::Vector3f* vertical,
                        const LiveActor* actor) {
    separateVelocityDirHV(horizontal, vertical, actor, getGravity(actor));
}

void separateVelocityDirHV(sead::Vector3f* horizontal, sead::Vector3f* vertical,
                           const LiveActor* actor, const sead::Vector3f& dir) {
    separateVectorHV(horizontal, vertical, dir, getVelocity(actor));
}

void separateVelocityParallelVertical(sead::Vector3f* parallel, sead::Vector3f* vertical,
                                      const LiveActor* actor, const sead::Vector3f& dir) {
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
    getVelocityPtr(actor)->setScale(getGravity(actor), -speed);
}

void setVelocityToFront(LiveActor* actor, f32 speed) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&front, actor);
    getVelocityPtr(actor)->setScale(front, speed);
}

void setVelocityToUp(LiveActor* actor, f32 speed) {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    calcUpDir(&up, actor);
    getVelocityPtr(actor)->setScale(up, speed);
}

void setVelocityToSide(LiveActor* actor, f32 speed) {
    sead::Vector3f side = {0.0f, 0.0f, 0.0f};
    calcSideDir(&side, actor);
    getVelocityPtr(actor)->setScale(side, speed);
}

void setVelocityToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 speed) {
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dir);
    getVelocityPtr(actor)->setScale(normDir, speed);
}

void setVelocityToGravity(LiveActor* actor, f32 speed) {
    getVelocityPtr(actor)->setScale(getGravity(actor), speed);
}

void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, const sead::Vector3f& v,
                           f32 speedH, f32 speedV) {
    calcVectorSeparateHV(getVelocityPtr(actor), h, v, speedH, speedV);
}

void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV) {
    calcVelocitySeparateHV(getVelocityPtr(actor), actor, h, speedH, speedV);
}

void calcVelocitySeparateHV(sead::Vector3f* velocity, const LiveActor* actor,
                            const sead::Vector3f& h, f32 speedH, f32 speedV) {
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
    getVelocityPtr(actor)->add(normDir * (speed - normDir.dot(getVelocity(actor))));
}

void setVelocityOnlyV(LiveActor* actor, f32 speed) {
    setVelocityOnlyDir(actor, getGravity(actor), -speed);
}

void setVelocityOnlyGravity(LiveActor* actor, f32 speed) {
    setVelocityOnlyDir(actor, getGravity(actor), speed);
}

void addVelocity(LiveActor* actor, const sead::Vector3f& vel) {
    getVelocityPtr(actor)->add(vel);
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
    addVelocity(actor, getGravity(actor) * -force);
}

inline void addVelocityInline(LiveActor* actor, const sead::Vector3f& vel, f32 force) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    velocity->setScaleAdd(force, vel, *velocity);
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
    velocity->setScaleAdd(force, getGravity(actor), *velocity);
}

void addVelocityToGravityLimit(LiveActor* actor, f32 force, f32 limit) {
    addVectorLimit(getVelocityPtr(actor), getGravity(actor) * force, limit);
}

void addVelocityToGravityFittedGround(LiveActor* actor, f32 force, u32 maxAirTime) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    const sead::Vector3f& normal = getOnGroundNormal(actor, maxAirTime);
    // TODO maybe this should be a Vector3::subScale() method?
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

void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 minForce, f32 maxForce,
                         f32 minDistance, f32 maxDistance) {
    sead::Vector3f diff = target;
    diff -= getTrans(actor);
    f32 distance;
    separateScalarAndDirection(&distance, &diff, diff);
    f32 normDistance = normalize(distance, minDistance, maxDistance);
    sead::Vector3f* velocity = getVelocityPtr(actor);
    f32 force = lerpValue(minForce, maxForce, normDistance);
    velocity->setScaleAdd(force, diff, *velocity);
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
    addVelocityDampToTarget(actor, target, forceDamp, forceDamp);
}

void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force, f32 damp) {
    sead::Vector3f diff = (target - getTrans(actor)) * force;
    sead::Vector3f* velocity = getVelocityPtr(actor);
    lerpVec(velocity, *velocity, diff, damp);
}

bool addVelocityToPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityToTarget(actor, playerPos + offset, force);
    return true;
}

bool addVelocityToPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
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

void addVelocityFromTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH,
                             f32 forceV) {
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
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityFromTarget(actor, playerPos + offset, force);
    return true;
}

bool addVelocityFromPlayerHV(LiveActor* actor, f32 forceH, f32 forceV,
                             const sead::Vector3f& offset) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;

    addVelocityFromTargetHV(actor, playerPos + offset, forceH, forceV);
    return true;
}

void addVelocityClockwiseToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force) {
    sead::Vector3f dirVelocity;
    if (!calcVelocityClockwiseToDirection(actor, &dirVelocity, dir))
        return;
    sead::Vector3f normDir;
    tryNormalizeOrZero(&normDir, dirVelocity);
    addVelocityInline(actor, normDir, force);
}

bool calcVelocityClockwiseToDirection(LiveActor* actor, sead::Vector3f* dirVelocity,
                                      const sead::Vector3f& dir) {
    sead::Vector3f normDir;
    if (!dirVelocity || !tryNormalizeOrZero(&normDir, dir))
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

void subVelocityExceptDirectionLimit(LiveActor* actor, const sead::Vector3f& direction, f32 subVel,
                                     f32 limit) {
    sead::Vector3f horizontal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityDirHV(&horizontal, &vertical, actor, direction);

    f32 length = horizontal.length();
    f32 newLen = sead::Mathf::clampMin(length - subVel, limit);
    // TODO sead function to scale a vector to a certain length?
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
    if (separateScalarAndDirection(&scalar, &direction, getVelocity(actor)))
        return;

    // enforces a *lower* limit = minimum speed!
    if (scalar < limit)
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

void scaleVelocityHV(LiveActor* actor, f32 factorH, f32 factorV) {
    scaleVelocityParallelVertical(actor, getGravity(actor), factorV, factorH);
}

void scaleVelocityDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    scaleVectorDirection(velocity, direction, *velocity, factor);
}

void scaleVelocityExceptDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor) {
    sead::Vector3f* velocity = getVelocityPtr(actor);
    scaleVectorExceptDirection(velocity, direction, *velocity, factor);
}

void scaleVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& direction, f32 parallel,
                                   f32 vertical) {
    const sead::Vector3f& velocity = getVelocity(actor);

    f32 speedV = direction.dot(velocity);
    sead::Vector3f parallelVec = direction * (speedV * parallel);
    sead::Vector3f verticalVec = velocity;
    // TODO sead subScale ?
    verticalVec.x -= direction.x * speedV;
    verticalVec.y -= direction.y * speedV;
    verticalVec.z -= direction.z * speedV;

    sead::Vector3f* newVelocity = getVelocityPtr(actor);
    *newVelocity = parallelVec;
    newVelocity->setScaleAdd(vertical, verticalVec, parallelVec);
}

void limitVelocity(LiveActor* actor, f32 limit) {
    if (calcSpeed(actor) > limit) {
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
        if (length > 0.0f)
            horizontal *= limitH / length;
    }
    setVelocity(actor, vertical + horizontal);
}

void limitVelocityHV(LiveActor* actor, f32 limitH, f32 limitV) {
    sead::Vector3f horizontal, vertical;
    separateVelocityHV(&horizontal, &vertical, actor);
    if (horizontal.squaredLength() > sead::Mathf::square(limitH)) {
        f32 length = horizontal.length();
        if (length > 0.0f)
            horizontal *= limitH / length;
    }
    if (vertical.squaredLength() > sead::Mathf::square(limitV)) {
        f32 length = vertical.length();
        if (length > 0.0f)
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
        if (length > 0.0f)
            horizontal *= limitH / length;
    }

    f32 speedV = vertical.dot(getGravity(actor));
    if (speedV > limitUp)
        vertical = getGravity(actor) * limitUp;
    f32 negLimitDown = -limitDown;
    if (speedV < negLimitDown)
        vertical = getGravity(actor) * negLimitDown;
    setVelocity(actor, vertical + horizontal);
}

void limitVelocityDir(LiveActor* actor, const sead::Vector3f& dir, f32 limit) {
    sead::Vector3f parallel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertical = {0.0f, 0.0f, 0.0f};
    separateVelocityParallelVertical(&parallel, &vertical, actor, dir);
    if (parallel.squaredLength() > sead::Mathf::square(limit)) {
        f32 length = parallel.length();
        if (length > 0.0f)
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
        if (length > 0.0f)
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
        if (length > 0.0f)
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
        if (length > 0.0f)
            vertical *= ((length1 - limit) * rate + limit) / length;
    }
    setVelocity(actor, parallel + vertical);
}

void limitVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& dir, f32 parallel,
                                   f32 vertical) {
    limitVectorParallelVertical(getVelocityPtr(actor), dir, parallel, vertical);
}

void limitVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& dir, f32 horizontal,
                             f32 vertical) {
    limitVectorSeparateHV(getVelocityPtr(actor), dir, horizontal, vertical);
}

u32 reboundVelocityPart(LiveActor* actor, f32 rebound, f32 threshold) {
    return reboundVelocityPart(actor, rebound, rebound, rebound, threshold);
}

u32 reboundVelocityPart(LiveActor* actor, f32 ground, f32 wall, f32 ceiling, f32 threshold) {
    u32 result = 0;
    if (isCollidedGround(actor)) {
        sead::Vector3f normal = getCollidedGroundNormal(actor);
        f32 dot = normal.dot(getVelocity(actor));
        if (ground < 0.0f || dot < -threshold) {
            addVelocity(actor, -(normal * ((ground + 1.0f) * dot)));
            if (ground >= 0.0f)
                result |= 1;
        } else if (dot < 0.0f) {
            addVelocity(actor, -(normal * dot));
        }
    }

    if (isCollidedWall(actor)) {
        sead::Vector3f normal = getCollidedWallNormal(actor);
        f32 dot = normal.dot(getVelocity(actor));
        if (wall < 0.0f || dot < -threshold) {
            addVelocity(actor, -(normal * ((wall + 1.0f) * dot)));
            if (wall >= 0.0f)
                result |= 2;
        } else if (dot < 0.0f) {
            addVelocity(actor, -(normal * dot));
        }
    }

    if (isCollidedCeiling(actor)) {
        sead::Vector3f normal = getCollidedCeilingNormal(actor);
        f32 dot = normal.dot(getVelocity(actor));
        if (ceiling < 0.0f || dot < -threshold) {
            addVelocity(actor, -(normal * ((ceiling + 1.0f) * dot)));
            if (ceiling >= 0.0f)
                result |= 4;
        } else if (dot < 0.0f) {
            addVelocity(actor, -(normal * dot));
        }
    }

    return result;
}

bool reboundVelocityFromEachCollision(LiveActor* actor, f32 ground, f32 wall, f32 ceiling,
                                      f32 threshold) {
    if (!isCollided(actor))
        return false;

    sead::Vector3f normalSum;
    calcCollidedNormalSum(actor, &normalSum);
    if (isNearZero(normalSum, 0.001f))
        return false;

    normalize(&normalSum);
    const sead::Vector3f& gravity = getGravity(actor);
    f32 rebound;
    if (isFloorPolygon(normalSum, gravity))
        rebound = ground;
    else if (isWallPolygon(normalSum, gravity))
        rebound = wall;
    else if (isCeilingPolygon(normalSum, gravity))
        rebound = ceiling;
    else
        rebound = 0.0f;

    f32 dot = normalSum.dot(getVelocity(actor));
    if (dot < -threshold) {
        sead::Vector3f* velocity = getVelocityPtr(actor);
        f32 mul = (rebound + 1.0f) * dot;
        velocity->setScaleAdd(-mul, normalSum, *velocity);
        return true;
    } else if (dot < 0.0f) {
        sead::Vector3f* velocity = getVelocityPtr(actor);
        velocity->setScaleAdd(-dot, normalSum, *velocity);
    }
    return false;
}

bool reboundVelocityFromCollision(LiveActor* actor, f32 reboundStrength, f32 reboundMin,
                                  f32 friction) {
    if (!isCollided(actor))
        return false;

    sead::Vector3f normalSum;
    calcCollidedNormalSum(actor, &normalSum);
    if (isNearZero(normalSum, 0.001f))
        return false;

    normalize(&normalSum);
    f32 dot = normalSum.dot(getVelocity(actor));
    if (dot < -reboundMin) {
        *getVelocityPtr(actor) -= normalSum * dot;
        scaleVelocity(actor, friction);
        *getVelocityPtr(actor) -= normalSum * dot * reboundStrength;
        return true;
    } else if (dot < 0.0f) {
        *getVelocityPtr(actor) -= normalSum * dot;
    }
    return false;
}

bool reboundVelocityFromTriangles(LiveActor* actor, f32 reboundStrength, f32 reboundMin) {
    Collider* collider = getActorCollider(actor);
    s32 _4c = collider->get_4c();
    if (collider->get_48() == 0)
        return false;

    bool isRebound = false;
    for (s32 i = 0; i != _4c; i++) {
        sead::Vector3f normal = collider->getPlane(i)->getNormal(0);
        f32 dot = normal.dot(getVelocity(actor));
        if (reboundStrength < 0.0) {
            addVelocity(actor, -((reboundStrength + 1.0f) * dot * normal));
            isRebound = true;
        } else if (dot < -reboundMin) {
            addVelocity(actor, -((reboundStrength + 1.0f) * dot * normal));
            isRebound = true;
        } else if (dot < 0.0)
            addVelocity(actor, -(dot * normal));
    }

    return isRebound;
}

bool reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, f32 reboundStrength) {
    return reboundVelocityFromActor(actor, target, getVelocity(target), reboundStrength);
}

__attribute__((always_inline)) bool
reboundVelocityFromActorInline(LiveActor* actor, const LiveActor* target,
                               const sead::Vector3f& targetVelocity, f32 reboundStrength) {
    sead::Vector3f direction;
    calcDirToActor(&direction, actor, target);
    f32 dot = (getVelocity(actor) - targetVelocity).dot(direction);
    if (dot <= 0.0f)  // moving away from each other
        return false;

    addVelocity(actor, direction * dot * -(reboundStrength + 1.0f));
    return true;
}

bool reboundVelocityFromActor(LiveActor* actor, const LiveActor* target,
                              const sead::Vector3f& targetVelocity, f32 reboundStrength) {
    return reboundVelocityFromActorInline(actor, target, targetVelocity, reboundStrength);
}

bool reboundVelocityFromSensor(LiveActor* actor, const HitSensor* sensor, f32 reboundStrength) {
    return reboundVelocityFromSensor(actor, sensor, getActorVelocity(sensor), reboundStrength);
}

bool reboundVelocityFromSensor(LiveActor* actor, const HitSensor* sensor,
                               const sead::Vector3f& targetVelocity, f32 reboundStrength) {
    return reboundVelocityFromActorInline(actor, getSensorHost(sensor), targetVelocity,
                                          reboundStrength);
}

bool calcDirToActor(sead::Vector3f* dir, const LiveActor* actor, const LiveActor* target) {
    dir->setSub(getTrans(target), getTrans(actor));
    return !tryNormalizeOrZero(dir);
}

bool reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target,
                                 const sead::Vector3f& direction, f32 reboundStrength);

bool reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target, f32 reboundStrength) {
    sead::Vector3f direction;
    calcDirToActor(&direction, actor, target);
    return reboundVelocityBetweenActor(actor, target, direction, reboundStrength);
}

bool reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target,
                                 const sead::Vector3f& direction, f32 reboundStrength) {
    f32 dot = (getVelocity(actor) - getVelocity(target)).dot(direction);
    if (dot <= 0.0f)  // moving away from each other
        return false;
    sead::Vector3f vel = direction * dot * ((reboundStrength + 1.0f) * 0.5f);
    addVelocity(target, vel);
    addVelocity(actor, -vel);
    return true;
}

bool reboundVelocityBetweenSensor(HitSensor* sensorA, HitSensor* sensorB, f32 reboundStrength) {
    sead::Vector3f direction;
    calcDirBetweenSensors(&direction, sensorA, sensorB);
    return reboundVelocityBetweenActor(getSensorHost(sensorA), getSensorHost(sensorB), direction,
                                       reboundStrength);
}

void calcVelocityKeepLengthBetweenActor(sead::Vector3f* vel, const LiveActor* actor,
                                        const LiveActor* target, f32 targetLength, f32 force) {
    sead::Vector3f dir;
    dir.setSub(getTrans(target), getTrans(actor));
    f32 len = dir.length();
    tryNormalizeOrDirZ(&dir);
    *vel = (targetLength - len) * dir * force * 0.5f;
}

void addVelocityKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32 targetLength,
                                       f32 force) {
    sead::Vector3f vel;
    calcVelocityKeepLengthBetweenActor(&vel, actor, target, targetLength, force);
    addVelocity(actor, -vel);
    addVelocity(target, vel);
}

void addVelocityDumpKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32 targetLength,
                                           f32 lenForce, f32 dumpForce) {
    sead::Vector3f vel;
    calcVelocityKeepLengthBetweenActor(&vel, actor, target, targetLength, lenForce);
    addVelocityDump(actor, -vel, dumpForce);
    addVelocityDump(target, vel, dumpForce);
}

void calcVelocityBlowAttack(sead::Vector3f* velocity, const LiveActor* actor,
                            const sead::Vector3f& trans, f32 speedH, f32 speedV) {
    calcVelocitySeparateHV(velocity, actor, getTrans(actor) - trans, speedH, speedV);
}

void addVelocityBlowAttack(LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV) {
    sead::Vector3f velocity;
    calcVelocityBlowAttack(&velocity, actor, trans, speedH, speedV);
    addVelocity(actor, velocity);
}

void addVelocityBlowAttack(LiveActor* actor, const HitSensor* sensor, f32 speedH, f32 speedV) {
    addVelocityBlowAttack(actor, getSensorPos(sensor), speedH, speedV);
}

void setVelocityBlowAttack(LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV) {
    setVelocitySeparateHV(actor, getTrans(actor) - trans, speedH, speedV);
}

void setVelocityBlowAttack(LiveActor* actor, const HitSensor* sensor, f32 speedH, f32 speedV) {
    setVelocitySeparateHV(actor, getTrans(actor) - getSensorPos(sensor), speedH, speedV);
}

void setVelocityBlowAttackAndTurnToTarget(LiveActor* actor, const sead::Vector3f& target,
                                          f32 speedH, f32 speedV) {
    sead::Vector3f dir = getTrans(actor) - target;
    bool isValidDir = tryNormalizeOrZero(&dir);
    setVelocitySeparateHV(actor, dir, speedH, speedV);
    if (!isValidDir)
        return;
    sead::Quatf quat;
    makeQuatUpFront(&quat, -getGravity(actor), -dir);
    updatePoseQuat(actor, quat);
}

bool isVelocityFast(const LiveActor* actor, f32 threshold) {
    return getVelocity(actor).squaredLength() > sead::Mathf::square(threshold);
}

bool isVelocityFastH(const LiveActor* actor, f32 threshold) {
    sead::Vector3f velocity = getVelocity(actor);
    verticalizeVec(&velocity, getGravity(actor), velocity);
    return sead::Mathf::square(threshold) < velocity.squaredLength();
}

bool isVelocitySlow(const LiveActor* actor, f32 threshold) {
    return getVelocity(actor).squaredLength() < sead::Mathf::square(threshold);
}

bool isVelocitySlowH(const LiveActor* actor, f32 threshold) {
    sead::Vector3f velocity = getVelocity(actor);
    verticalizeVec(&velocity, getGravity(actor), velocity);
    return velocity.squaredLength() < sead::Mathf::square(threshold);
}

f32 calcSpeedH(const LiveActor* actor) {
    sead::Vector3f velocityH;
    verticalizeVec(&velocityH, getGravity(actor), getVelocity(actor));
    return velocityH.length();
}

f32 calcSpeedV(const LiveActor* actor) {
    return -getVelocity(actor).dot(getGravity(actor));
}

f32 calcSpeedDirection(const LiveActor* actor, const sead::Vector3f& dir) {
    return sead::Mathf::abs(getVelocity(actor).dot(dir));
}

f32 calcSpeedExceptDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f velocityExceptDir = {0.0f, 0.0f, 0.0f};
    verticalizeVec(&velocityExceptDir, dir, getVelocity(actor));
    return velocityExceptDir.length();
}

bool isNear(const LiveActor* actor, const LiveActor* target, f32 threshold) {
    return isNear(actor, getTrans(target), threshold);
}

bool isNear(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold) {
    return (getTrans(actor) - trans).squaredLength() < sead::Mathf::square(threshold);
}

bool isNearXZ(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold) {
    const sead::Vector3f& actorTrans = getTrans(actor);
    return sead::Mathf::square(actorTrans.x - trans.x) +
               sead::Mathf::square(actorTrans.z - trans.z) <
           sead::Mathf::square(threshold);
}

bool isNearH(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold) {
    return calcDistanceH(actor, trans) < threshold;
}

f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f& trans) {
    sead::Vector3f dist;
    verticalizeVec(&dist, getGravity(actor), trans - getTrans(actor));
    return dist.length();
}

bool isNearV(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold) {
    return calcDistanceV(actor, trans) < threshold;
}

f32 calcDistanceV(const LiveActor* actor, const sead::Vector3f& trans) {
    const sead::Vector3f& gravity = getGravity(actor);
    return sead::Mathf::abs((trans - getTrans(actor)).dot(gravity));
}

bool isNearHV(const LiveActor* actor, const sead::Vector3f& trans, f32 threshH, f32 threshV) {
    if (calcDistanceV(actor, trans) > threshV)
        return false;
    return isNearH(actor, trans, threshH);
}

bool isNearHV(const LiveActor* actor, const sead::Vector3f& trans, f32 threshH, f32 minV,
              f32 maxV) {
    f32 height = calcHeight(actor, trans);
    if (height < minV || height > maxV)
        return false;
    return isNearH(actor, trans, threshH);
}

f32 calcHeight(const LiveActor* actor, const sead::Vector3f& trans) {
    const sead::Vector3f& gravity = getGravity(actor);
    return -(trans - getTrans(actor)).dot(gravity);
}

bool isFar(const LiveActor* actor, const LiveActor* target, f32 threshold) {
    return isFar(actor, getTrans(target), threshold);
}

bool isFar(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold) {
    return (getTrans(actor) - trans).squaredLength() > sead::Mathf::square(threshold);
}

f32 calcDistance(const LiveActor* actor, const LiveActor* target) {
    return calcDistance(actor, getTrans(target));
}

f32 calcDistance(const LiveActor* actor, const sead::Vector3f& trans) {
    return (getTrans(actor) - trans).length();
}

f32 calcDistanceV(const LiveActor* actor, const LiveActor* target) {
    return calcDistanceV(actor, getTrans(target));
}

f32 calcDistanceH(const LiveActor* actor, const LiveActor* target) {
    return calcDistanceH(actor, getTrans(target));
}

f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f& trans1,
                  const sead::Vector3f& trans2) {
    sead::Vector3f dist;
    verticalizeVec(&dist, getGravity(actor), trans2 - trans1);
    return dist.length();
}

f32 calcHeight(const LiveActor* actor, const LiveActor* target) {
    return calcHeight(actor, getTrans(target));
}

f32 calcDistanceFront(const LiveActor* actor, const sead::Vector3f& trans) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return (trans - getTrans(actor)).dot(front);
}

f32 calcDistanceFront(const LiveActor* actor, const LiveActor* target) {
    return calcDistanceFront(actor, getTrans(target));
}

void addRotateAndRepeatX(LiveActor* actor, f32 deg) {
    setRotateX(actor, modDegree(getRotate(actor).x + deg));
}

void addRotateAndRepeatY(LiveActor* actor, f32 deg) {
    setRotateY(actor, modDegree(getRotate(actor).y + deg));
}

void addRotateAndRepeatZ(LiveActor* actor, f32 deg) {
    setRotateZ(actor, modDegree(getRotate(actor).z + deg));
}

void addRandomRotateY(LiveActor* actor) {
    addRotateAndRepeatY(actor, getRandomDegree());
}

void calcQuatSide(sead::Vector3f* side, const LiveActor* actor) {
    calcQuatSide(side, getQuat(actor));
}

void calcQuatUp(sead::Vector3f* up, const LiveActor* actor) {
    calcQuatUp(up, getQuat(actor));
}

void calcQuatFront(sead::Vector3f* front, const LiveActor* actor) {
    calcQuatFront(front, getQuat(actor));
}

void calcQuatLocalAxis(sead::Vector3f* local, const LiveActor* actor, s32 axis) {
    calcQuatLocalAxis(local, getQuat(actor), axis);
}

void calcTransOffsetFront(sead::Vector3f* offset, const LiveActor* actor, f32 len) {
    multVecPose(offset, actor, {0.0f, 0.0f, len});
}

void calcTransOffsetUp(sead::Vector3f* offset, const LiveActor* actor, f32 len) {
    multVecPose(offset, actor, {0.0f, len, 0.0f});
}

void calcTransOffsetSide(sead::Vector3f* offset, const LiveActor* actor, f32 len) {
    multVecPose(offset, actor, {len, 0.0f, 0.0f});
}

void setTransOffsetLocalDir(LiveActor* actor, const sead::Quatf& quat,
                            const sead::Vector3f& globalOffset, f32 localOffset, s32 axis) {
    sead::Vector3f offset;
    calcQuatLocalAxis(&offset, quat, axis);
    getTransPtr(actor)->setScaleAdd(localOffset, offset, globalOffset);
}

void addTransOffsetLocal(LiveActor* actor, const sead::Vector3f& localOffset) {
    sead::Quatf quat = sead::Quatf::unit;
    calcQuat(&quat, actor);
    sead::Vector3f offset;
    offset.setRotated(quat, localOffset);
    *getTransPtr(actor) += offset;
}

void addTransOffsetLocalDir(LiveActor* actor, f32 localOffset, s32 axis) {
    setTransOffsetLocalDir(actor, getQuat(actor), getTrans(actor), localOffset, axis);
}

void rotateQuatXDirDegree(LiveActor* actor, f32 deg) {
    sead::Quatf* quat = getQuatPtr(actor);
    rotateQuatXDirDegree(quat, *quat, deg);
}

void rotateQuatXDirDegree(LiveActor* actor, const sead::Quatf& quat, f32 deg) {
    rotateQuatXDirDegree(getQuatPtr(actor), quat, deg);
}

void rotateQuatYDirDegree(LiveActor* actor, f32 deg) {
    sead::Quatf* quat = getQuatPtr(actor);
    rotateQuatYDirDegree(quat, *quat, deg);
}

void rotateQuatYDirDegree(LiveActor* actor, const sead::Quatf& quat, f32 deg) {
    rotateQuatYDirDegree(getQuatPtr(actor), quat, deg);
}

void rotateQuatZDirDegree(LiveActor* actor, f32 deg) {
    sead::Quatf* quat = getQuatPtr(actor);
    rotateQuatZDirDegree(quat, *quat, deg);
}

void rotateQuatZDirDegree(LiveActor* actor, const sead::Quatf& quat, f32 deg) {
    rotateQuatZDirDegree(getQuatPtr(actor), quat, deg);
}

void rotateQuatLocalDirDegree(LiveActor* actor, s32 axis, f32 deg) {
    sead::Quatf* quat = getQuatPtr(actor);
    rotateQuatLocalDirDegree(quat, *quat, axis, deg);
}

void rotateQuatLocalDirDegree(LiveActor* actor, const sead::Quatf& quat, s32 axis, f32 deg) {
    rotateQuatLocalDirDegree(getQuatPtr(actor), quat, axis, deg);
}

void rotateQuatYDirRandomDegree(LiveActor* actor) {
    sead::Quatf* quat = getQuatPtr(actor);
    rotateQuatYDirDegree(quat, *quat, getRandomDegree());
}

void rotateQuatYDirRandomDegree(LiveActor* actor, const sead::Quatf& quat) {
    rotateQuatYDirDegree(getQuatPtr(actor), quat, getRandomDegree());
}

bool turnQuatFrontToDirDegreeH(LiveActor* actor, const sead::Vector3f& dir, f32 deg) {
    return turnQuatFrontToDirDegreeH(getQuatPtr(actor), dir, deg);
}

bool turnQuatFrontToPosDegreeH(LiveActor* actor, const sead::Vector3f& pos, f32 deg) {
    sead::Vector3f dir;
    dir.setSub(pos, getTrans(actor));
    return turnQuatFrontToDirDegreeH(actor, dir, deg);
}

bool turnQuatFrontFromPosDegreeH(LiveActor* actor, const sead::Vector3f& pos, f32 deg) {
    sead::Vector3f dir;
    dir.setSub(getTrans(actor), pos);
    return turnQuatFrontToDirDegreeH(actor, dir, deg);
}

void turnFront(LiveActor* actor, f32 deg) {
    sead::Vector3f up = sead::Vector3f::ey;
    calcUpDir(&up, actor);
    turnFront(actor, up, deg);
}

void turnFront(LiveActor* actor, const sead::Vector3f& up, f32 deg) {
    sead::Vector3f* front = getFrontPtr(actor);
    rotateVectorDegree(front, *front, up, deg);
    normalize(getFrontPtr(actor));
}

void turnFrontToPos(LiveActor* actor, const sead::Vector3f& pos, f32 deg) {
    turnFrontToDir(actor, pos - getTrans(actor), deg);
}

void turnFrontToDir(LiveActor* actor, const sead::Vector3f& dir, f32 deg) {
    turnFrontToDirGetIsFinished(actor, dir, deg);
}

bool turnFrontToDirGetIsFinished(LiveActor* actor, const sead::Vector3f& dir, f32 deg) {
    sead::Vector3f up;
    calcFrontDir(&up, actor);
    bool finished = turnDirectionDegree(actor, &up, dir, deg);
    rotateVectorDegree(getFrontPtr(actor), getFront(actor), up, deg);
    normalize(getFrontPtr(actor));
    return finished;
}

bool turnDirectionDegree(const LiveActor* actor, sead::Vector3f* vec, const sead::Vector3f& dir,
                         f32 deg) {
    f32 cos = sead::Mathf::cos(sead::Mathf::deg2rad(deg));
    return turnDirection(actor, vec, dir, cos);
}

void turnFrontToTarget(LiveActor* actor, const LiveActor* target, f32 deg) {
    turnFrontToDir(actor, getTrans(target) - getTrans(actor), deg);
}

void turnFrontFromTarget(LiveActor* actor, const LiveActor* target, f32 deg) {
    turnFrontToDir(actor, getTrans(actor) - getTrans(target), deg);
}

bool turnFrontToPlayer(LiveActor* actor, f32 deg) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    turnFrontToDir(actor, playerPos - getTrans(actor), deg);
    return true;
}

bool turnFrontFromPlayer(LiveActor* actor, f32 deg) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    turnFrontToDir(actor, getTrans(actor) - playerPos, deg);
    return true;
}

bool turnDirection(const LiveActor* actor, sead::Vector3f* vec, const sead::Vector3f& dir,
                   f32 cos) {
    return turnVecToVecCosOnPlane(vec, dir, getGravity(actor), cos);
}

bool turnDirectionToTarget(const LiveActor* actor, sead::Vector3f* vec,
                           const sead::Vector3f& target, f32 cos) {
    return turnDirection(actor, vec, target - getTrans(actor), cos);
}

bool turnDirectionToTargetDegree(const LiveActor* actor, sead::Vector3f* vec,
                                 const sead::Vector3f& target, f32 deg) {
    return turnDirectionDegree(actor, vec, target - getTrans(actor), deg);
}

bool turnDirectionFromTargetDegree(const LiveActor* actor, sead::Vector3f* vec,
                                   const sead::Vector3f& target, f32 deg) {
    return turnDirectionDegree(actor, vec, getTrans(actor) - target, deg);
}

void turnDirectionAlongGround(const LiveActor* actor, sead::Vector3f* dir) {
    sead::Vector3f down;
    if (isCollidedGround(actor))
        down = -getOnGroundNormal(actor, 0);
    else
        down.set(getGravity(actor));

    verticalizeVec(dir, down, *dir);
    normalize(dir);
}

void turnDirectionAlongGround(LiveActor* actor) {
    if (tryGetQuatPtr(actor)) {
        sead::Vector3f ground;
        calcFrontDir(&ground, actor);

        sead::Vector3f down;
        if (isCollidedGround(actor))
            down = -getOnGroundNormal(actor, 0);
        else
            down.set(getGravity(actor));

        verticalizeVec(&ground, down, ground);
        if (!tryNormalizeOrZero(&ground))
            return;

        turnToDirectionAxis(actor, ground, -down, 180.0f);
    } else if (getFrontPtr(actor)) {
        turnDirectionAlongGround(actor, getFrontPtr(actor));
    }
}

bool turnToDirectionAxis(LiveActor* actor, const sead::Vector3f& horizontal,
                         const sead::Vector3f& vertical, f32 deg) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&front, actor);
    bool result = turnVecToVecCosOnPlane(&front, horizontal, vertical,
                                         sead::Mathf::cos(sead::Mathf::deg2rad(deg)));

    sead::Quatf quat = sead::Quatf::unit;
    makeQuatUpFront(&quat, vertical, front);
    updatePoseQuat(actor, quat);
    return result;
}

bool turnFrontSpherical(const LiveActor* actor, sead::Vector3f* vec,
                        const sead::Vector3f& targetFront, f32 cos) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    sead::Vector3f axis;
    axis.setCross(front, targetFront);
    if (isNearZero(axis, 0.001f))
        calcUpDir(&axis, actor);
    normalize(&axis);
    normalize(&front);
    return turnVecToVecCosOnPlane(vec, front, targetFront, axis, cos);
}

bool turnFrontSphericalToTarget(const LiveActor* actor, sead::Vector3f* vec,
                                const sead::Vector3f& target, f32 cos) {
    sead::Vector3f targetFront = target - getTrans(actor);
    if (!tryNormalizeOrZero(&targetFront)) {
        calcFrontDir(vec, actor);
        return true;
    }
    return turnFrontSpherical(actor, vec, targetFront, cos);
}

bool turnFrontSphericalToTargetDegree(const LiveActor* actor, sead::Vector3f* vec,
                                      const sead::Vector3f& target, f32 deg) {
    return turnFrontSphericalToTarget(actor, vec, target,
                                      sead::Mathf::cos(sead::Mathf::deg2rad(deg)));
}

bool turnToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 deg) {
    sead::Vector3f vec;
    calcFrontDir(&vec, actor);
    bool result = turnDirectionDegree(actor, &vec, dir, deg);

    sead::Quatf quat;
    makeQuatUpFront(&quat, -getGravity(actor), vec);
    updatePoseQuat(actor, quat);
    return result;
}

void turnLocalDirToDirection(LiveActor* actor, const sead::Vector3f& localDir,
                             const sead::Vector3f& targetDir, f32 deg) {
    sead::Quatf quat;
    calcQuat(&quat, actor);
    sead::Vector3f globalDir = localDir;
    rotateVectorQuat(&globalDir, quat);

    sead::Quatf quat2;
    makeQuatRotationLimit(&quat2, globalDir, targetDir, sead::Mathf::deg2rad(deg));

    quat2 *= quat;
    updatePoseQuat(actor, quat2);
}

bool turnToTarget(LiveActor* actor, const sead::Vector3f& target, f32 deg) {
    sead::Vector3f dir = target - getTrans(actor);
    if (!tryNormalizeOrZero(&dir))
        return false;
    return turnToDirection(actor, dir, deg);
}

bool turnToTarget(LiveActor* actor, const LiveActor* target, f32 deg) {
    return turnToTarget(actor, getTrans(target), deg);
}

void faceToDirection(LiveActor* actor, const sead::Vector3f& dir) {
    if (isParallelDirection(dir, getGravity(actor), 0.01f))
        return;

    sead::Quatf quat;
    makeQuatUpFront(&quat, -getGravity(actor), dir);
    updatePoseQuat(actor, quat);
}

void faceToDirectionSupportUp(LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    calcUpDir(&up, actor);
    if (isParallelDirection(dir, up, 0.01f))
        return;

    sead::Quatf quat;
    makeQuatFrontUp(&quat, dir, up);
    updatePoseQuat(actor, quat);
}

void faceToTarget(LiveActor* actor, const sead::Vector3f& target) {
    sead::Vector3f direction = target - getTrans(actor);
    if (!tryNormalizeOrZero(&direction))
        return;
    faceToDirection(actor, direction);
}

void faceToTarget(LiveActor* actor, const LiveActor* target) {
    faceToTarget(actor, getTrans(target));
}

void faceToSensor(LiveActor* actor, const HitSensor* sensor) {
    faceToTarget(actor, getSensorPos(sensor));
}

void faceToVelocity(LiveActor* actor) {
    sead::Vector3f direction = getVelocity(actor);
    if (!tryNormalizeOrZero(&direction))
        return;
    faceToDirection(actor, direction);
}

void calcDirClockwiseToDir(sead::Vector3f* out, const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f result;
    result.setCross(getGravity(actor), dir);
    tryNormalizeOrZero(out, result);
}

void calcDirClockwiseToPos(sead::Vector3f* out, const LiveActor* actor,
                           const sead::Vector3f& target) {
    calcDirClockwiseToDir(out, actor, target - getTrans(actor));
}

void calcDirToActorH(sead::Vector3f* out, const LiveActor* actor, const LiveActor* target) {
    calcDirToActorH(out, actor, getTrans(target));
}

void calcDirToActorH(sead::Vector3f* out, const LiveActor* actor, const sead::Vector3f& target) {
    out->setSub(target, getTrans(actor));
    verticalizeVec(out, getGravity(actor), *out);
    tryNormalizeOrZero(out);
}

f32 calcAngleToTargetH(const LiveActor* actor, const sead::Vector3f& target) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    sead::Vector3f dir = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&front, actor);
    calcUpDir(&up, actor);
    dir.setSub(target, getTrans(actor));
    if (!tryNormalizeOrZero(&dir))
        return 0.0f;
    return calcAngleOnPlaneDegree(front, dir, up);
}

f32 calcAngleToTargetV(const LiveActor* actor, const sead::Vector3f& target) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    sead::Vector3f side = {0.0f, 0.0f, 0.0f};
    sead::Vector3f dir = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&front, actor);
    calcSideDir(&side, actor);
    dir.setSub(target, getTrans(actor));
    if (!tryNormalizeOrZero(&dir))
        return 0.0f;
    return calcAngleOnPlaneDegree(front, dir, side);
}

bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f& target,
                          const sead::Vector3f& face, f32 threshDeg) {
    return isNearAngleDegree(target - getTrans(actor), face, threshDeg);
}

bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f& target, f32 threshDeg) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return isNearAngleDegree(target - getTrans(actor), front, threshDeg);
}

bool isFaceToTargetDegreeHV(const LiveActor* actor, const sead::Vector3f& target,
                            const sead::Vector3f& face, f32 degH, f32 degV) {
    return isNearAngleDegreeHV(target - getTrans(actor), face, getGravity(actor), degH, degV);
}

bool isFaceToTargetDegreeH(const LiveActor* actor, const sead::Vector3f& target,
                           const sead::Vector3f& face, f32 degH) {
    sead::Vector3f diff = target;
    diff -= getTrans(actor);
    verticalizeVec(&diff, getGravity(actor), diff);
    sead::Vector3f alignedFace;
    verticalizeVec(&alignedFace, getGravity(actor), face);
    return isNearAngleDegree(diff, alignedFace, degH);
}

bool isInSightCone(const LiveActor* actor, const sead::Vector3f& target, const sead::Vector3f& face,
                   f32 maxDist, f32 threshDeg) {
    return (getTrans(actor) - target).squaredLength() < sead::Mathf::square(maxDist) &&
           isFaceToTargetDegree(actor, target, face, threshDeg);
}

bool isInSightConeTarget(const LiveActor* actor, const LiveActor* target, f32 maxDist,
                         f32 threshDeg) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return isInSightCone(actor, getTrans(target), front, maxDist, threshDeg);
}

bool isInSightConePlayer(const LiveActor* actor, f32 maxDist, f32 threshDeg) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return isInSightCone(actor, playerPos, front, maxDist, threshDeg);
}

bool isInSightFan(const LiveActor* actor, const sead::Vector3f& target, const sead::Vector3f& face,
                  f32 maxDist, f32 angleH, f32 angleV) {
    return (getTrans(actor) - target).squaredLength() < sead::Mathf::square(maxDist) &&
           isFaceToTargetDegreeHV(actor, target, face, angleH, angleV);
}

bool isInSightFanTarget(const LiveActor* actor, const LiveActor* target, f32 maxDist, f32 angleH,
                        f32 angleV) {
    sead::Vector3f front = sead::Vector3f::ez;
    calcFrontDir(&front, actor);
    return isInSightFan(actor, getTrans(target), front, maxDist, angleH, angleV);
}

bool isInSightBox(const LiveActor* actor, const sead::Vector3f& pos, const sead::BoundBox3f& box) {
    sead::Vector3f localPos;
    multVecInvPose(&localPos, actor, pos);
    return box.isInside(localPos);
}

void walkAndTurnToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 forceFront,
                            f32 forceGravity, f32 decay, f32 deg, bool turnAlongGround) {
    walkAndTurnToDirection(actor, getFrontPtr(actor), dir, forceFront, forceGravity, decay, deg,
                           turnAlongGround);
}

void walkAndTurnToDirection(LiveActor* actor, sead::Vector3f* front, const sead::Vector3f& dir,
                            f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                            bool turnAlongGround) {
    turnDirection(actor, front, dir, sead::Mathf::cos(sead::Mathf::deg2rad(deg)));
    if (turnAlongGround)
        turnDirectionAlongGround(actor);

    sead::Vector3f velFront;
    tryNormalizeOrZero(&velFront, *front);
    addVelocityInline(actor, velFront, forceFront);

    if (!isOnGround(actor, 3))
        addVelocityToGravity(actor, forceGravity);

    scaleVelocity(actor, decay);
}

void walkAndTurnPoseToDirection(LiveActor* actor, const sead::Vector3f& dir,
                                const ActorParamMove& param, bool turnAlongGround) {
    sead::Vector3f* frontPtr;
    sead::Vector3f frontForQuat;
    if (getQuatPtr(actor)) {
        frontPtr = &frontForQuat;
        calcFrontDir(&frontForQuat, actor);
        turnToDirection(actor, dir, param.turnDegrees);
    } else if (getFrontPtr(actor)) {
        frontPtr = getFrontPtr(actor);
        turnDirectionDegree(actor, frontPtr, dir, param.turnDegrees);
    } else
        return;

    if (turnAlongGround)
        turnDirectionAlongGround(actor);

    f32 forceFront = param.forceFront;
    sead::Vector3f velFront;
    tryNormalizeOrZero(&velFront, *frontPtr);
    addVelocityInline(actor, velFront, forceFront);

    if (!isOnGround(actor, 3))
        addVelocityToGravity(actor, param.forceGravity);

    // BUG: should have been param.decay (_8)
    scaleVelocity(actor, param.forceFront);
}

void walkAndTurnToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceFront,
                         f32 forceGravity, f32 decay, f32 deg, bool turnAlongGround) {
    sead::Vector3f dir = target - getTrans(actor);
    walkAndTurnToDirection(actor, dir, forceFront, forceGravity, decay, deg, turnAlongGround);
}

void flyAndTurnToDirection(LiveActor* actor, sead::Vector3f* front, const sead::Vector3f& dir,
                           f32 forceFront, f32 forceGravity, f32 decay, f32 deg) {
    sead::Vector3f normFrontH, actorFront, frontH;

    calcFrontDir(&actorFront, actor);
    turnDirectionDegree(actor, &actorFront, dir, deg);
    turnVecToVecDegree(front, *front, actorFront, deg);

    calcFrontDir(&frontH, actor);
    verticalizeVec(&frontH, getGravity(actor), frontH);
    normalize(&frontH);

    tryNormalizeOrZero(&normFrontH, frontH);
    addVelocityInline(actor, normFrontH, forceFront);
    addVelocityToGravity(actor, forceGravity);
    scaleVelocity(actor, decay);
}

void flyAndTurnToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 forceFront,
                           f32 forceGravity, f32 decay, f32 deg) {
    flyAndTurnToDirection(actor, getFrontPtr(actor), dir, forceFront, forceGravity, decay, deg);
}

void flyAndTurnToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceFront,
                        f32 forceGravity, f32 decay, f32 deg) {
    flyAndTurnToDirection(actor, target - getTrans(actor), forceFront, forceGravity, decay, deg);
}

bool walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, sead::Vector3f* front,
                                               const sead::Vector3f& dir, f32 forceFront,
                                               f32 forceGravity, f32 decay, f32 deg,
                                               bool turnAlongGround) {
    turnDirection(actor, front, dir, sead::Mathf::cos(sead::Mathf::deg2rad(deg)));
    if (turnAlongGround)
        turnDirectionAlongGround(actor, front);

    sead::Vector3f velFront;
    tryNormalizeOrZero(&velFront, *front);
    addVelocityInline(actor, velFront, forceFront);

    bool isOnGround = isOnGroundNoVelocity(actor, 3);
    if (isOnGround)
        addVelocityToGravityFittedGround(actor, forceGravity, 3);
    else
        addVelocityToGravity(actor, forceGravity);

    scaleVelocity(actor, decay);
    return isOnGround;
}

bool walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, const sead::Vector3f& dir,
                                               f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                                               bool turnAlongGround) {
    return walkAndTurnToDirectionFittedGroundGravity(actor, getFrontPtr(actor), dir, forceFront,
                                                     forceGravity, decay, deg, turnAlongGround);
}

bool walkAndTurnToTargetFittedGroundGravity(LiveActor* actor, const sead::Vector3f& target,
                                            f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                                            bool turnAlongGround) {
    return walkAndTurnToDirectionFittedGroundGravity(actor, target - getTrans(actor), forceFront,
                                                     forceGravity, decay, deg, turnAlongGround);
}

bool tryKillByDeathArea(LiveActor* actor) {
    if (!isInDeathArea(actor, getTrans(actor)))
        return false;
    actor->kill();
    return true;
}

void rotateAndKeepColliderPosRate(LiveActor* actor, const sead::Vector3f& up,
                                  const sead::Vector3f& front, f32 rate) {
    sead::Vector3f colliderPos = {0.0f, 0.0f, 0.0f};
    calcColliderPos(&colliderPos, actor);

    sead::Vector3f localColliderPos;
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    calcMtxLocalTrans(&localColliderPos, mtx, colliderPos);

    sead::Quatf quat;
    makeQuatUpFront(&quat, up, front);
    slerpQuat(getQuatPtr(actor), getQuat(actor), quat, rate);

    sead::Matrix34f mtx2;
    makeMtxRT(&mtx2, actor);

    sead::Vector3f newColliderPos = {0.0f, 0.0f, 0.0f};
    calcTransLocalOffsetByMtx(&newColliderPos, mtx2, localColliderPos);
    sead::Vector3f colliderOffset = colliderPos - newColliderPos;

    *getTransPtr(actor) += colliderOffset;
    resetPosition(actor);
}

void calcSpringMovement(LiveActor* actor, const sead::Vector3f& pos, f32 springPos, f32 sinStrength,
                        const sead::Vector3f& offset, f32 constStrength, f32 sinAmpl) {
    f32 sinPart = sead::Mathf::sin(sead::Mathf::clamp(springPos, 0.0f, 1.0f) * 2 *
                                   sead::Mathf::pi() * sinAmpl) *
                  (1.0f - springPos) * sinStrength;
    f32 constPart = (1.0f - springPos) * constStrength;
    getTransPtr(actor)->setScaleAdd(sinPart + constPart, offset, pos);
}

bool addVelocityClockwiseToPlayer(LiveActor* actor, f32 force) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    addVelocityClockwiseToDirection(actor, playerPos - getTrans(actor), force);
    return true;
}

bool calcDirClockwiseToPlayer(sead::Vector3f* dir, const LiveActor* actor) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    // BUG: should have been calcDirClockwiseToPos
    calcDirClockwiseToDir(dir, actor, playerPos);
    return true;
}

bool flyAndTurnToPlayer(LiveActor* actor, const ActorParamMove& param) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    flyAndTurnToTarget(actor, playerPos, param.forceFront, param.forceGravity, param.decay,
                       param.turnDegrees);
    return true;
}

bool escapeFromPlayer(LiveActor* actor, f32 forceFront, f32 forceGravity, f32 decay, f32 deg) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    walkAndTurnToDirection(actor, getTrans(actor) - playerPos, forceFront, forceGravity, decay, deg,
                           true);
    return true;
}

bool escapeFromPlayer(LiveActor* actor, sead::Vector3f* front, f32 forceFront, f32 forceGravity,
                      f32 decay, f32 deg) {
    sead::Vector3f playerPos = {0.0f, 0.0f, 0.0f};
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    walkAndTurnToDirection(actor, front, getTrans(actor) - playerPos, forceFront, forceGravity,
                           decay, deg, true);
    return true;
}

bool walkAndTurnToPlayer(LiveActor* actor, f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                         bool turnAlongGround) {
    sead::Vector3f playerPos;
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    return walkAndTurnToTargetFittedGroundGravity(actor, playerPos, forceFront, forceGravity, decay,
                                                  deg, turnAlongGround);
}

bool isPlayerInSightFan(const LiveActor* actor, f32 maxDist, f32 angleH, f32 angleV) {
    sead::Vector3f playerPos;
    if (!tryFindNearestPlayerPos(&playerPos, actor))
        return false;
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return isInSightFan(actor, playerPos, front, maxDist, angleH, angleV);
}

inline bool nextMove(Triangle* triangle, const LiveActor* actor, const sead::Vector3f& velocity,
                     f32 gravity, f32 searchDist) {
    sead::Vector3f hitPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f pos = getTrans(actor) + velocity;
    pos.y += gravity;

    return alCollisionUtil::getFirstPolyOnArrow(actor, &hitPos, triangle, pos,
                                                -sead::Vector3f::ey * searchDist, nullptr, nullptr);
}

bool isFallOrDamageCodeNextMove(const LiveActor* actor, const sead::Vector3f& velocity, f32 gravity,
                                f32 searchDist) {
    Triangle triangle;
    if (!nextMove(&triangle, actor, velocity, gravity, searchDist))
        return true;
    return isFloorCode(triangle, "DamageFire") || isFloorCode(triangle, "Poison");
}

bool isFallNextMove(const LiveActor* actor, const sead::Vector3f& velocity, f32 gravity,
                    f32 searchDist) {
    Triangle triangle;
    return !nextMove(&triangle, actor, velocity, gravity, searchDist);
}

}  // namespace al
