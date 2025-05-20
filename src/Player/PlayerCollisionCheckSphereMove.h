#pragma once

#include <math/seadVector.h>

#include "Library/Collision/IUseCollision.h"

namespace al {
class CollisionDirector;
class CollisionParts;
class CollisionPartsFilterBase;
}

class PlayerCollisionCheckSphereMove : public al::IUseCollision {
public:
    PlayerCollisionCheckSphereMove(al::CollisionDirector*, int);
    
    al::CollisionDirector* getCollisionDirector() const override;

    bool checkSphereMove(const sead::Vector3f&, const sead::Vector3f&, f32);
    void gatherCollisionParts(al::CollisionParts*);
    u32 getNum() const;
    f32 getTValue(u32) const;
    const sead::Vector3f& getPos(u32) const;
    const sead::Vector3f& getNormal(u32) const;
    const al::CollisionParts* getCollisionParts(u32) const;
    const char* getMapCodeName(u32) const;
    const char* getWallCodeName(u32) const;
    const char* getMaterialCodeName(u32) const;
    
    void setCollisionPartsFilter(const al::CollisionPartsFilterBase* filter) {
        mCollisionPartsFilter = filter;
    }

private:
    void* _8[0x50/8];
    const al::CollisionPartsFilterBase* mCollisionPartsFilter;
};

static_assert(sizeof(PlayerCollisionCheckSphereMove) == 0x60);
