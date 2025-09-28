#include "Library/LiveActor/ActorCollisionFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/ICollisionPartsKeeper.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Project/Collision/ActorCollisionController.h"

namespace al {

Collider* getActorCollider(const LiveActor* actor) {
    return actor->getCollider();
}

bool isExistActorCollider(const LiveActor* actor) {
    return actor->getCollider() != nullptr;
}

const HitSensor* getActorCollisionPartsSensor(const LiveActor* actor) {
    return actor->getCollisionParts()->getConnectedSensor();
}

bool isExistCollisionParts(const LiveActor* actor) {
    return actor->getCollisionParts() != nullptr;
}

bool isEqualCollisionParts(const LiveActor* actor, const CollisionParts* collisionParts) {
    return collisionParts && actor->getCollisionParts() == collisionParts;
}

void validateCollisionParts(LiveActor* actor) {
    CollisionParts* collisionParts = actor->getCollisionParts();
    if (collisionParts->getSyncCollisonMtx()) {
        collisionParts->resetAllMtx();
    } else {
        sead::Matrix34f mtx;
        makeMtxSRT(&mtx, actor);
        collisionParts->resetAllMtx(mtx);
    }
    collisionParts->validateByUser();
}

void invalidateCollisionParts(LiveActor* actor) {
    actor->getCollisionParts()->invalidateByUser();
}

void validateCollisionPartsBySystem(LiveActor* actor) {
    CollisionParts* collisionParts = actor->getCollisionParts();
    collisionParts->validateBySystem();

    if (collisionParts->getSyncCollisonMtx()) {
        collisionParts->resetAllMtx();
    } else {
        sead::Matrix34f mtx;
        makeMtxSRT(&mtx, actor);
        collisionParts->resetAllMtx(mtx);
    }
}

void invalidateCollisionPartsBySystem(LiveActor* actor) {
    actor->getCollisionParts()->invalidateBySystem();
}

bool isValidCollisionParts(const LiveActor* actor) {
    return actor->getCollisionParts()->isValidCollision();
}

void setCollisionPartsSpecialPurposeName(LiveActor* actor, const char* name) {
    actor->getCollisionParts()->setSpecialPurposeName(name);
}

void resetAllCollisionMtx(LiveActor* actor) {
    CollisionParts* collisionParts = actor->getCollisionParts();

    if (collisionParts->getSyncCollisonMtx()) {
        collisionParts->resetAllMtx();
    } else {
        sead::Matrix34f mtx;
        makeMtxSRT(&mtx, actor);
        collisionParts->resetAllMtx(mtx);
    }

    alCollisionUtil::getCollisionPartsKeeper(actor)->resetToCollisionPartsList(collisionParts);
}

void syncCollisionMtx(LiveActor* actor, const sead::Matrix34f* mtx) {
    syncCollisionMtx(actor, actor->getCollisionParts(), mtx);
}

void syncCollisionMtx(LiveActor* actor, CollisionParts* collisionParts,
                      const sead::Matrix34f* mtx) {
    if (!collisionParts->isValidCollision())
        return;

    if (collisionParts->getSyncCollisonMtx()) {
        collisionParts->syncMtx();
        return;
    }

    if (mtx) {
        collisionParts->syncMtx(*mtx);
        return;
    }

    sead::Matrix34f newMtx;
    makeMtxSRT(&newMtx, actor);
    collisionParts->syncMtx(newMtx);
}

void setSyncCollisionMtxPtr(LiveActor* actor, const sead::Matrix34f* mtx) {
    actor->getCollisionParts()->setSyncCollisionMtx(mtx);
}

bool isOnGround(const LiveActor* actor, u32 offset) {
    Collider* collider = actor->getCollider();
    if (!isCollidedGround(actor) && collider->get_264() > offset)
        return false;
    return !(getVelocity(actor).dot(collider->getRecentOnGroundNormal(offset)) > 0.0f);
}

bool isOnGroundFace(const LiveActor* actor) {
    return isOnGround(actor, 0) && !isCollidedGroundEdgeOrCorner(actor);
}

bool isCollidedGroundEdgeOrCorner(const LiveActor* actor) {
    return isCollidedGround(actor) && !actor->getCollider()->getFloorHit().isCollisionAtFace();
}

bool isOnGroundNoVelocity(const LiveActor* actor, u32 offset) {
    if (!isExistActorCollider(actor))
        return getTrans(actor).y <= 0.0f;

    if (isCollidedGround(actor))
        return true;

    return actor->getCollider()->get_264() <= offset;
}

bool isOnGroundDegree(const LiveActor* actor, f32 angle, u32 offset) {
    Collider* collider = actor->getCollider();
    if (!(isCollidedGround(actor) || collider->get_264() <= offset))
        return false;

    if (sead::Mathf::abs((*collider->get_30()).dot(collider->getRecentOnGroundNormal(offset))) <
        sead::Mathf::cos(sead::Mathf::deg2rad(angle)))
        return false;

    return !(getVelocity(actor).dot(collider->getRecentOnGroundNormal(offset)) > 0.0f);
}

bool isOnGroundFaceDegree(const LiveActor* actor, f32 angle) {
    return isOnGroundDegree(actor, angle, 0) && !isCollidedGroundEdgeOrCorner(actor);
}

bool isOnGroundNoVelocityDegree(const LiveActor* actor, f32 angle, u32 offset) {
    Collider* collider = actor->getCollider();
    if (!collider)
        return getTrans(actor).y <= 0.0;

    if (!(isCollidedGround(actor) || collider->get_264() <= offset))
        return false;

    if (sead::Mathf::abs((*collider->get_30()).dot(collider->getRecentOnGroundNormal(offset))) <
        sead::Mathf::cos(sead::Mathf::deg2rad(angle)))
        return false;

    return true;
}

const sead::Vector3f& getOnGroundNormal(const LiveActor* actor, u32 offset) {
    return actor->getCollider()->getRecentOnGroundNormal(offset);
}

void setColliderRadius(LiveActor* actor, f32 radius) {
    actor->getCollider()->setRadius(radius);
}

void setColliderOffsetY(LiveActor* actor, f32 offsety) {
    actor->getCollider()->setOffsetY(offsety);
}

f32 getColliderRadius(const LiveActor* actor) {
    return actor->getCollider()->getRadius();
}

f32 getColliderOffsetY(const LiveActor* actor) {
    return actor->getCollider()->getOffsetY();
}

const sead::Vector3f& getColliderFixReaction(const LiveActor* actor) {
    return actor->getCollider()->getFixReaction();
}

void calcColliderPos(sead::Vector3f* outPos, const LiveActor* actor) {
    actor->getCollider()->calcCheckPos(outPos);
}

void validateColliderRobustCheck(LiveActor* actor) {
    actor->getCollider()->validateRobustCheck();
}

void invalidateColliderRobustCheck(LiveActor* actor) {
    actor->getCollider()->invalidateRobustCheck();
}

void setColliderReactMovePower(LiveActor* actor, bool isEnabled) {
    actor->getCollider()->setReactMovePower(isEnabled);
}

void calcColliderFloorRotatePower(sead::Quatf* outRotatePower, LiveActor* actor) {
    actor->getCollider()->getFloorHit().triangle.calcForceRotatePower(outRotatePower);
}

void calcJumpInertia(sead::Vector3f* outJumpInertia, LiveActor* actor, const sead::Vector3f& pos,
                     f32 force) {
    if (!isCollidedGround(actor)) {
        outJumpInertia->set({0.0f, 0.0f, 0.0f});
        return;
    }

    sead::Vector3f movePower = {0.0f, 0.0f, 0.0f};
    const HitInfo& floorSensor = actor->getCollider()->getFloorHit();
    if (floorSensor.triangle.getCollisionParts() && floorSensor.triangle.isHostMoved())
        floorSensor.triangle.calcForceMovePower(&movePower, getTrans(actor));

    verticalizeVec(&movePower, getGravity(actor), movePower);

    sead::Vector3f movePowerNormalized = movePower;
    tryNormalizeOrZero(&movePowerNormalized);

    sead::Vector3f movePowerVertical = {0.0f, 0.0f, 0.0f};
    verticalizeVec(&movePowerVertical, getGravity(actor), pos);
    if (!tryNormalizeOrZero(&movePowerVertical)) {
        outJumpInertia->set(movePower * force);
        return;
    }

    force *= sead::Mathf::clampMin(movePowerNormalized.dot(movePowerVertical), 0.0f);
    outJumpInertia->set(movePower * force);
}

bool isCollidedGround(const LiveActor* actor) {
    return actor->getCollider()->get_110() >= 0.0f;
}

void calcJumpInertiaWall(sead::Vector3f* outJumpInertia, LiveActor* actor, f32 force) {
    if (!isCollidedWall(actor)) {
        outJumpInertia->set({0.0f, 0.0f, 0.0f});
        return;
    }

    sead::Vector3f movePower = {0.0f, 0.0f, 0.0f};
    const sead::Vector3f& trans = getTrans(actor);
    const HitInfo& floorSensor = actor->getCollider()->getWallHit();

    if (floorSensor.triangle.getCollisionParts() &&
        alCollisionUtil::isCollisionMoving(&floorSensor))
        floorSensor.triangle.calcForceMovePower(&movePower, trans);

    verticalizeVec(&movePower, getGravity(actor), movePower);

    outJumpInertia->set(movePower * force);
}

bool isCollidedWall(const LiveActor* actor) {
    return actor->getCollider()->get_1b8() >= 0.0f;
}

inline f32 normalize2(sead::Vector3f* v, f32 scalar) {
    const f32 len = v->length();
    if (len > 0) {
        const f32 inv_len = scalar / len;
        v->x *= inv_len;
        v->y *= inv_len;
        v->z *= inv_len;
    }

    return len;
}

void scaleVelocityInertiaWallHit(LiveActor* actor, f32 velocityScale, f32 maxVelocity,
                                 f32 maxScaledVelocity) {
    if (!isCollidedWall(actor))
        return;

    sead::Vector3f faceNormal = getCollidedWallNormal(actor);
    verticalizeVec(&faceNormal, getGravity(actor), faceNormal);
    if (!tryNormalizeOrZero(&faceNormal))
        return;

    sead::Vector3f jumpInertia = {0.0f, 0.0f, 0.0f};
    calcJumpInertiaWall(&jumpInertia, actor, 1.0f);

    sead::Vector3f velocityH = {0.0f, 0.0f, 0.0f};
    sead::Vector3f velocityY = {0.0f, 0.0f, 0.0f};
    separateVelocityHV(&velocityH, &velocityY, actor);

    sead::Vector3f velocityDirH = {0.0f, 0.0f, 0.0f};
    tryNormalizeOrZero(&velocityDirH, velocityH);

    if (velocityDirH.dot(faceNormal) < -0.5f) {
        f32 velocity = velocityH.length();
        f32 scaledVelocity = velocity * velocityScale;

        velocity = sead::Mathf::min(velocity, maxVelocity);

        if (!(scaledVelocity < velocity)) {
            velocity = scaledVelocity;
            if (maxScaledVelocity < scaledVelocity)
                velocity = maxScaledVelocity;
        }

        normalize2(&velocityH, velocity);

        f32 hammer = jumpInertia.dot(velocityDirH);
        if (-hammer < faceNormal.dot(velocityDirH))
            normalize2(&velocityH, hammer);
    }

    setVelocity(actor, velocityH + velocityY);
}

const sead::Vector3f& getCollidedWallNormal(const LiveActor* actor) {
    return actor->getCollider()->getWallHit().triangle.getFaceNormal();
}

void calcCollidedNormalSum(const LiveActor* actor, sead::Vector3f* outNormal) {
    outNormal->set({0.0f, 0.0f, 0.0f});
    Collider* collider = actor->getCollider();

    if (collider->get_110() >= 0.0f)
        outNormal->add(collider->getFloorHit().triangle.getFaceNormal());

    if (collider->get_1b8() >= 0.0f)
        outNormal->add(collider->getWallHit().triangle.getFaceNormal());

    if (collider->get_260() >= 0.0f)
        outNormal->add(collider->getCeilingHit().triangle.getFaceNormal());

    *outNormal *= 1.0f / 3.0f;
}

void calcGroundNormalOrUpDir(sead::Vector3f* outDir, const LiveActor* actor) {
    if (!isCollidedGround(actor)) {
        calcUpDir(outDir, actor);
        return;
    }

    *outDir = getCollidedGroundNormal(actor);
}

const sead::Vector3f& getCollidedGroundNormal(const LiveActor* actor) {
    return actor->getCollider()->getFloorHit().triangle.getFaceNormal();
}

void calcGroundNormalOrGravityDir(sead::Vector3f* outDir, const LiveActor* actor) {
    if (!isCollidedGround(actor)) {
        *outDir = -getGravity(actor);
        return;
    }

    *outDir = getCollidedGroundNormal(actor);
}

void setColliderFilterTriangle(LiveActor* actor, const TriangleFilterBase* triangleFilter) {
    actor->getCollider()->setTriangleFilter(triangleFilter);
}

void setColliderFilterCollisionParts(LiveActor* actor,
                                     const CollisionPartsFilterBase* collisionPartsFilter) {
    actor->getCollider()->setCollisionPartsFilter(collisionPartsFilter);
}

void createAndSetColliderFilterExistActor(LiveActor* actor) {
    createAndSetColliderFilterExistActor(actor, actor);
}

void createAndSetColliderFilterExistActor(LiveActor* actor, LiveActor* filterActor) {
    CollisionPartsFilterActor* collisionPartsFilter = new CollisionPartsFilterActor(filterActor);
    setColliderFilterCollisionParts(actor, collisionPartsFilter);
}

void createAndSetColliderFilterSubActor(LiveActor* actor) {
    CollisionPartsFilterSubActor* collisionPartsFilter = new CollisionPartsFilterSubActor(actor);
    setColliderFilterCollisionParts(actor, collisionPartsFilter);
}

void createAndSetColliderSpecialPurpose(LiveActor* actor, const char* name) {
    CollisionPartsFilterSpecialPurpose* collisionPartsFilter =
        new CollisionPartsFilterSpecialPurpose(name);
    setColliderFilterCollisionParts(actor, collisionPartsFilter);
}

void createAndSetColliderSpecialPurposeForCollisionActor(LiveActor* actor, const char* name) {
    CollisionPartsFilterMergePair* collisionPartsFilter = new CollisionPartsFilterMergePair(
        new CollisionPartsFilterSpecialPurpose(name), new CollisionPartsFilterActor(actor));
    setColliderFilterCollisionParts(actor, collisionPartsFilter);
}

void createAndSetColliderIgnoreOptionalPurpose(LiveActor* actor, const char* name) {
    CollisionPartsFilterIgnoreOptionalPurpose* collisionPartsFilter =
        new CollisionPartsFilterIgnoreOptionalPurpose(name);
    setColliderFilterCollisionParts(actor, collisionPartsFilter);
}

void createAndSetColliderSpecialAndIgnoreOptionalPurpose(LiveActor* actor, const char* name,
                                                         const char* ignoreName) {
    CollisionPartsFilterMergePair* collisionPartsFilter = new CollisionPartsFilterMergePair(
        new CollisionPartsFilterSpecialPurpose(name),
        new CollisionPartsFilterIgnoreOptionalPurpose(ignoreName));
    setColliderFilterCollisionParts(actor, collisionPartsFilter);
}

ActorCollisionController* createActorCollisionController(LiveActor* actor) {
    return new ActorCollisionController(actor);
}

void setColliderRadius(ActorCollisionController* collisionController, f32 radius) {
    collisionController->setColliderRadius(radius);
}

void setColliderRadiusScale(ActorCollisionController* collisionController, f32 scale) {
    collisionController->setColliderRadiusScale(scale);
}

void setColliderOffsetY(ActorCollisionController* collisionController, f32 offsetY) {
    collisionController->setColliderOffsetY(offsetY);
}

void resetActorCollisionController(ActorCollisionController* collisionController, s32 delay) {
    collisionController->resetToOrigin(delay);
}

void updateActorCollisionController(ActorCollisionController* collisionController) {
    collisionController->update();
}

const char* getCollidedFloorMaterialCodeName(const LiveActor* actor) {
    return getCollidedFloorMaterialCodeName(actor->getCollider());
}

const char* getCollidedFloorMaterialCodeName(const Collider* collider) {
    return getMaterialCodeName(collider->getFloorHit().triangle);
}

const char* getCollidedWallMaterialCodeName(const LiveActor* actor) {
    return getMaterialCodeName(actor->getCollider()->getWallHit().triangle);
}

const char* getCollidedCeilingMaterialCodeName(const LiveActor* actor) {
    return getMaterialCodeName(actor->getCollider()->getCeilingHit().triangle);
}

bool isCollidedFloorCode(const LiveActor* actor, const char* name) {
    return isCollidedCollisionCode(actor, "FloorCode", name);
}

bool isCollidedCollisionCode(const LiveActor* actor, const char* sensorName, const char* name) {
    Collider* collider = actor->getCollider();
    if (collider->get_48() == 0) {
        if (collider->get_110() >= 0.0f) {
            if (isEqualString(name,
                              getCollisionCodeName(collider->getFloorHit().triangle, sensorName))) {
                return true;
            }
        }
        if (collider->get_1b8() >= 0.0f) {
            if (isEqualString(name,
                              getCollisionCodeName(collider->getWallHit().triangle, sensorName))) {
                return true;
            }
        }
        if (collider->get_260() >= 0.0f) {
            if (isEqualString(
                    name, getCollisionCodeName(collider->getCeilingHit().triangle, sensorName))) {
                return true;
            }
        }
        return false;
    }

    u32 size = collider->getPlaneCount();
    for (u32 i = 0; i < size; i++)
        if (isEqualString(name, getCollisionCodeName(*collider->getPlane(i), sensorName)))
            return true;

    return false;
}

bool isCollidedWallCode(const LiveActor* actor, const char* name) {
    return isCollidedCollisionCode(actor, "WallCode", name);
}

bool isCollidedCameraCode(const LiveActor* actor, const char* name) {
    return isCollidedCollisionCode(actor, "CameraCode", name);
}

bool isCollidedMaterialCode(const LiveActor* actor, const char* name) {
    return isCollidedCollisionCode(actor, "MaterialCode", name);
}

bool isCollidedGroundFloorCode(const LiveActor* actor, const char* name) {
    return isCollidedGroundCollisionCode(actor, "FloorCode", name);
}

bool isCollidedGroundCollisionCode(const LiveActor* actor, const char* sensorName,
                                   const char* name) {
    if (!isCollidedGround(actor))
        return false;

    return isEqualString(
        name, getCollisionCodeName(actor->getCollider()->getFloorHit().triangle, sensorName));
}

bool isCollided(const LiveActor* actor) {
    return isCollidedGround(actor) || isCollidedWall(actor) || isCollidedCeiling(actor);
}

bool isCollidedCeiling(const LiveActor* actor) {
    return actor->getCollider()->get_260() >= 0.0f;
}

bool isCollidedWallFace(const LiveActor* actor) {
    if (!isCollidedWall(actor))
        return false;
    return actor->getCollider()->isCollidedWallFace();
}

bool isCollidedVelocity(const LiveActor* actor) {
    if (isOnGround(actor, 0))
        return true;

    if (isCollidedWallVelocity(actor))
        return true;

    return isCollidedCeilingVelocity(actor);
}

bool isCollidedWallVelocity(const LiveActor* actor) {
    return isCollidedWall(actor) && getVelocity(actor).dot(getCollidedWallNormal(actor)) < 0.0f;
}

bool isCollidedCeilingVelocity(const LiveActor* actor) {
    return isCollidedCeiling(actor) &&
           getVelocity(actor).dot(getCollidedCeilingNormal(actor)) < 0.0f;
}

const sead::Vector3f& getCollidedCeilingNormal(const LiveActor* actor) {
    return actor->getCollider()->getCeilingHit().triangle.getFaceNormal();
}

const sead::Vector3f& getCollidedGroundPos(const LiveActor* actor) {
    return actor->getCollider()->getFloorHit().collisionHitPos;
}

const sead::Vector3f& getCollidedWallPos(const LiveActor* actor) {
    return actor->getCollider()->getWallHit().collisionHitPos;
}

const sead::Vector3f& getCollidedCeilingPos(const LiveActor* actor) {
    return actor->getCollider()->getCeilingHit().collisionHitPos;
}

f32 calcSpeedCollideWall(const LiveActor* actor) {
    if (!isCollidedWall(actor))
        return 0.0f;
    sead::Vector3f wallNormal = getCollidedWallNormal(actor);
    return -sead::Mathf::min(wallNormal.dot(getVelocity(actor)), 0.0f);
}

f32 calcSpeedCollideGround(const LiveActor* actor) {
    if (!isCollidedGround(actor))
        return 0.0f;
    sead::Vector3f groundNormal = getCollidedGroundNormal(actor);
    return -sead::Mathf::min(groundNormal.dot(getVelocity(actor)), 0.0f);
}

f32 calcSpeedCollideCeiling(const LiveActor* actor) {
    if (!isCollidedCeiling(actor))
        return 0.0f;
    sead::Vector3f ceilingNormal = getCollidedCeilingNormal(actor);
    return -sead::Mathf::min(ceilingNormal.dot(getVelocity(actor)), 0.0f);
}

const CollisionParts* getCollidedGroundCollisionParts(const LiveActor* actor) {
    return tryGetCollidedGroundCollisionParts(actor);
}

const CollisionParts* tryGetCollidedGroundCollisionParts(const LiveActor* actor) {
    if (!isCollidedGround(actor))
        return nullptr;
    return actor->getCollider()->getFloorHit().triangle.getCollisionParts();
}

const CollisionParts* getCollidedWallCollisionParts(const LiveActor* actor) {
    return tryGetCollidedWallCollisionParts(actor);
}

const CollisionParts* tryGetCollidedWallCollisionParts(const LiveActor* actor) {
    if (!isCollidedWall(actor))
        return nullptr;
    return actor->getCollider()->getWallHit().triangle.getCollisionParts();
}

const CollisionParts* getCollidedCeilingCollisionParts(const LiveActor* actor) {
    return tryGetCollidedCeilingCollisionParts(actor);
}

const CollisionParts* tryGetCollidedCeilingCollisionParts(const LiveActor* actor) {
    if (!isCollidedCeiling(actor))
        return nullptr;
    return actor->getCollider()->getCeilingHit().triangle.getCollisionParts();
}

HitSensor* getCollidedGroundSensor(const LiveActor* actor) {
    return tryGetCollidedGroundSensor(actor);
}

HitSensor* tryGetCollidedGroundSensor(const LiveActor* actor) {
    if (!isCollidedGround(actor))
        return nullptr;
    return actor->getCollider()->getFloorHit().triangle.getCollisionParts()->getConnectedSensor();
}

HitSensor* getCollidedWallSensor(const LiveActor* actor) {
    return tryGetCollidedWallSensor(actor);
}

HitSensor* tryGetCollidedWallSensor(const LiveActor* actor) {
    if (!isCollidedWall(actor))
        return nullptr;
    return actor->getCollider()->getWallHit().triangle.getCollisionParts()->getConnectedSensor();
}

HitSensor* getCollidedCeilingSensor(const LiveActor* actor) {
    return tryGetCollidedCeilingSensor(actor);
}

HitSensor* tryGetCollidedCeilingSensor(const LiveActor* actor) {
    if (!isCollidedCeiling(actor))
        return nullptr;
    return actor->getCollider()->getCeilingHit().triangle.getCollisionParts()->getConnectedSensor();
}

HitSensor* tryGetCollidedSensor(const LiveActor* actor) {
    HitSensor* sensor = tryGetCollidedWallSensor(actor);
    if (sensor)
        return sensor;

    sensor = tryGetCollidedGroundSensor(actor);
    if (sensor)
        return sensor;

    return tryGetCollidedCeilingSensor(actor);
}

bool tryGetCollidedPos(sead::Vector3f* outPos, const LiveActor* actor) {
    if (isCollidedWall(actor)) {
        outPos->set(getCollidedWallPos(actor));
        return true;
    }
    if (isCollidedGround(actor)) {
        outPos->set(getCollidedGroundPos(actor));
        return true;
    }
    if (isCollidedCeiling(actor)) {
        outPos->set(getCollidedCeilingPos(actor));
        return true;
    }
    return false;
}

void setForceCollisionScaleOne(const LiveActor* actor) {
    actor->getCollisionParts()->setForceCollisionScaleOne();
}

void followRotateFrontAxisUpGround(LiveActor* actor) {
    if (!isCollidedGround(actor))
        return;

    const CollisionParts* collisionParts =
        actor->getCollider()->getFloorHit().triangle.getCollisionParts();

    if (collisionParts)
        followRotateFrontAxisUp(actor, collisionParts);
}

void followRotateFrontAxisUp(LiveActor* actor, const CollisionParts* collisionParts) {
    sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&frontDir, actor);

    sead::Vector3f sideDir = frontDir;
    sideDir.rotate(collisionParts->getPrevBaseInvMtx());
    sideDir.rotate(collisionParts->getBaseMtx());

    sead::Vector3f upDir = {0.0f, 0.0f, 0.0f};
    calcUpDir(&upDir, actor);

    sead::Vector3f newFrontdir = {0.0f, 0.0f, 0.0f};
    turnVecToVecCosOnPlane(&newFrontdir, frontDir, sideDir, upDir, -1.0f);

    sead::Quatf quat = sead::Quatf::unit;
    makeQuatUpFront(&quat, upDir, newFrontdir);
    updatePoseQuat(actor, quat);
}

}  // namespace al
