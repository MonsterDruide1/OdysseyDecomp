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

    void init(const sead::Matrix34f*, const sead::Matrix34f&, const CollisionParts*);
    HitSensor* getConnectingSensor() const;
    bool isMoved() const;
    bool isConnectInvalidCollision() const;

private:
    void* _0[1];
};

static_assert(sizeof(CollisionPartsConnector) == 0x68);

}  // namespace al
