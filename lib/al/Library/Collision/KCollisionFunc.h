#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class KCollisionServer;
struct KCPrismData;
struct KCPrismHeader;
}  // namespace al

namespace alKCollisionFunc {
void calcSphereHitPos(sead::Vector3f* pHitPos, const al::KCollisionServer* pServer,
                      const sead::Vector3f& rPos, const al::KCPrismData& rData,
                      const al::KCPrismHeader* pHeader, u8 location);
void projectToPlane(sead::Vector3f* pOut, const sead::Vector3f& rPos,
                    const sead::Vector3f& rPlanePos, const sead::Vector3f& rPlaneNormal);
void calcDiskHitPos(sead::Vector3f* pHitPos, const al::KCollisionServer* pServer,
                    const sead::Vector3f& rPos, f32 radius, const sead::Vector3f& rDir,
                    const al::KCPrismData& rData, const al::KCPrismHeader* pHeader, u8 location);
}  // namespace alKCollisionFunc
