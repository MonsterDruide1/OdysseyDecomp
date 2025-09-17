#pragma once

#include <math/seadMatrix.h>

#include "Library/Collision/PartsMtxConnector.h"

namespace al {
class CollisionParts;
class HitSensor;

class CollisionPartsConnector : public MtxConnector {
public:
    CollisionPartsConnector();

    bool isConnecting() const override;
    void clear() override;

    // TODO: Rename parameters
    void init(const sead::Matrix34f* mtxA, const sead::Matrix34f& mtxB,
              const CollisionParts* collisionParts);
    const HitSensor* getConnectingSensor() const;
    bool isMoved() const;
    bool isConnectInvalidCollision() const;

private:
    const CollisionParts* mCollisionParts = nullptr;
};

static_assert(sizeof(CollisionPartsConnector) == 0x68);

}  // namespace al
