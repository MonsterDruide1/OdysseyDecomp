#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class KCollisionServer;
struct KCPrismData;
struct KCPrismHeader;
}  // namespace al

namespace alKCollisionFunc {
void calcSphereHitPos(sead::Vector3f* outHitPos, const al::KCollisionServer* server,
                      const sead::Vector3f& position, const al::KCPrismData& data,
                      const al::KCPrismHeader* header, u8 location);

void projectToPlane(sead::Vector3f* out, const sead::Vector3f& position,
                    const sead::Vector3f& planePosition, const sead::Vector3f& planeNormal);

void calcDiskHitPos(sead::Vector3f* outHitPos, const al::KCollisionServer* server,
                    const sead::Vector3f& position, f32 halfHeight, const sead::Vector3f& normal,
                    const al::KCPrismData& data, const al::KCPrismHeader* header, u8 location);
}  // namespace alKCollisionFunc
