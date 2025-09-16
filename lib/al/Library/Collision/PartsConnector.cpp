#include "Library/Collision/PartsConnector.h"

#include "Library/Collision/CollisionParts.h"

namespace al {
CollisionPartsConnector::CollisionPartsConnector() = default;

bool CollisionPartsConnector::isConnecting() const {
    if (isConnectInvalidCollision())
        return false;

    return MtxConnector::isConnecting();
}

void CollisionPartsConnector::clear() {
    MtxConnector::clear();
    mCollisionParts = nullptr;
}

void CollisionPartsConnector::init(const sead::Matrix34f* mtxA, const sead::Matrix34f& mtxB,
                                   const CollisionParts* collisionParts) {
    MtxConnector::init(mtxA, mtxB);
    mCollisionParts = collisionParts;
}

const HitSensor* CollisionPartsConnector::getConnectingSensor() const {
    return mCollisionParts->getConnectedSensor();
}

bool CollisionPartsConnector::isMoved() const {
    if (!mCollisionParts)
        return false;

    return mCollisionParts->get_15c() == 0 || mCollisionParts->isMoving();
}

bool CollisionPartsConnector::isConnectInvalidCollision() const {
    if (!mCollisionParts)
        return false;

    return !mCollisionParts->isConnectValidCollision();
}

}  // namespace al
