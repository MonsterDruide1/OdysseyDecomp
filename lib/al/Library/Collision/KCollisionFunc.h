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

void projectToPlane(sead::Vector3f* pOut, const sead::Vector3f& rPos,
                    const sead::Vector3f& rPlanePos, const sead::Vector3f& rPlaneNormal);

void calcDiskHitPos(sead::Vector3f* outHitPos, const al::KCollisionServer* server,
                    const sead::Vector3f& position, f32 radius, const sead::Vector3f& direction,
                    const al::KCPrismData& data, const al::KCPrismHeader* header, u8 location);
}  // namespace alKCollisionFunc
