#pragma once

#include <math/seadBoundBox.h>

#include "Library/Collision/CollisionParts.h"

namespace al {

class CollisionPartsFilterBase;
class ICollisionPartsKeeper;

struct CollisionCheckInfoBase {
    CollisionCheckInfoBase(const sead::Vector3f& position) : position(position) {}

    const sead::Vector3f& position;

    const CollisionPartsFilterBase* partsFilter = nullptr;
    const TriangleFilterBase* triFilter = nullptr;
};

struct SphereCheckInfo : public CollisionCheckInfoBase {
    SphereCheckInfo(const sead::Vector3f& position, f32 radius)
        : CollisionCheckInfoBase(position), radius(radius) {}

    f32 radius;
};

struct ArrowCheckInfo : public CollisionCheckInfoBase {
    ArrowCheckInfo(const sead::Vector3f& position, const sead::Vector3f& direction)
        : CollisionCheckInfoBase(position), direction(direction) {
        target.setAdd(position, direction);
        bounds.addPoint(position);
        bounds.addPoint(target);
    }

    const sead::Vector3f& direction;

    sead::Vector3f target;
    sead::BoundBox3f bounds;
};

struct DiskCheckInfo : public CollisionCheckInfoBase {
    DiskCheckInfo(const sead::Vector3f& position, f32 radius, f32 halfHeight,
                  const sead::Vector3f& normal);

    f32 radius;
    f32 halfHeight;
    const sead::Vector3f& normal;

    f32 boundingRange = 0.0f;
};

u32 checkStrikePointCore(HitInfo* outHit, const CollisionPartsList& partsList,
                         const CollisionCheckInfoBase& checkInfo);
u32 checkStrikeSphereCore(SphereHitResultBuffer* resultBuffer, const CollisionPartsList& partsList,
                          const SphereCheckInfo& checkInfo, bool checkMoving,
                          const sead::Vector3f& moveDirecrion);
u32 checkStrikeArrowCore(ArrowHitResultBuffer* resultBuffer, const CollisionPartsList& partsList,
                         const ArrowCheckInfo& checkInfo);
u32 checkStrikeSphereForPlayerCore(SphereHitResultBuffer* resultBuffer,
                                   const CollisionPartsList& partsList,
                                   const SphereCheckInfo& checkInfo, bool checkMoving,
                                   const sead::Vector3f& moveDirection);
u32 checkStrikeDiskCore(DiskHitResultBuffer* resultBuffer, const CollisionPartsList& partsList,
                        const DiskCheckInfo& checkInfo);

void updateCollisionParts(CollisionParts* parts);
void updateCollisionPartsList(CollisionPartsList* partsList);
void pushBackCollisionParts(CollisionPartsList* partsList, CollisionParts* parts);

ICollisionPartsKeeper* createSimpleCollisionPartsKeeper();

}  // namespace al
