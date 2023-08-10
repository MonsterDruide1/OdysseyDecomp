#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
void calcWorldPosFromGridIndex(sead::Vector3f* out, const sead::Matrix34f& matrix,
                               const sead::Vector3i& in);
void calcGridIndexFromWorldPos(sead::Vector3i* out, const sead::Matrix34f& matrix,
                               const sead::Vector3f& in);
sead::Vector3f calcGridIndexNext(const sead::Vector3f&, s32);
f32 calcDegreeDirIndexSignedX(s32 index);
f32 calcDegreeDirIndexSignedY(s32 index);
f32 calcDegreeDirIndexSignedZ(s32 index);
f32 calcDirIndexNearXY(const sead::Vector3i&, const sead::Vector3i&);
f32 calcDirIndexNearXY(const sead::Vector3i&, const sead::Vector3i&);
f32 calcDirIndexNearXY(const sead::Vector3i&, const sead::Vector3i&);
f32 signDirIndexX(s32);
f32 signDirIndexY(s32);
f32 signDirIndexZ(s32);
void calcGridMinMaxFromOBB(sead::Vector3i*, sead::Vector3i*, const sead::Matrix34f&,
                           const sead::Matrix34f&, const sead::BoundBox3f&);
void expandGridFromOBB(sead::Vector3i*, sead::Vector3i*, const sead::Matrix34f&,
                       const sead::Matrix34f&, const sead::BoundBox3f&);
}  // namespace al
