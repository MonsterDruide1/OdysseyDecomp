#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {

enum DirIndex : s32 {
    DirIndex_NegZ = -3,
    DirIndex_NegY = -2,
    DirIndex_NegX = -1,
    DirIndex_None = 0,
    DirIndex_X = 1,
    DirIndex_Y = 2,
    DirIndex_Z = 3,
};

void calcWorldPosFromGridIndex(sead::Vector3f* outPos, const sead::Matrix34f& mtx,
                               const sead::Vector3i& gridIndex);
void calcGridIndexFromWorldPos(sead::Vector3i* outGridIndex, const sead::Matrix34f& mtx,
                               const sead::Vector3f& pos);
sead::Vector3i calcGridIndexNext(const sead::Vector3i& gridIndex, s32 dirIndex);
f32 calcDegreeDirIndexSignedX(s32 dirIndex);
f32 calcDegreeDirIndexSignedY(s32 dirIndex);
f32 calcDegreeDirIndexSignedZ(s32 dirIndex);
s32 calcDirIndexNearXY(const sead::Vector3i& startGridIndex, const sead::Vector3i& endGridIndex);
s32 calcDirIndexNearYZ(const sead::Vector3i& startGridIndex, const sead::Vector3i& endGridIndex);
s32 calcDirIndexNearZX(const sead::Vector3i& startGridIndex, const sead::Vector3i& endGridIndex);
s32 signDirIndexX(s32 dirIndex);
s32 signDirIndexY(s32 dirIndex);
s32 signDirIndexZ(s32 dirIndex);
void calcGridMinMaxFromOBB(sead::Vector3i* outGridIndexMin, sead::Vector3i* outGridIndexMax,
                           const sead::Matrix34f& gridMtx, const sead::Matrix34f& boxMtx,
                           const sead::BoundBox3f& boundBox);
void expandGridFromOBB(sead::Vector3i* outGridIndexMin, sead::Vector3i* outGridIndexMax,
                       const sead::Matrix34f& gridMtx, const sead::Matrix34f& boxMtx,
                       const sead::BoundBox3f& boundBox);
}  // namespace al
