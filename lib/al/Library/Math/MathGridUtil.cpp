#include "Library/Math/MathGridUtil.h"

#include "Library/Math/MathUtil.h"

namespace al {
const f32 gIndexToDegreeX[7] = {270.0f, 180.0f, 0.0f, 0.0f, 0.0f, 0.0f, 90.0f};
const f32 gIndexToDegreeY[7] = {180.0f, 0.0f, 270.0f, 0.0f, 90.0f, 0.0f, 0.0f};
const f32 gIndexToDegreeZ[5] = {270.0f, 180.0f, 0.0f, 0.0f, 90.0f};

void calcWorldPosFromGridIndex(sead::Vector3f* outPos, const sead::Matrix34f& mtx,
                               const sead::Vector3i& gridIndex) {
    sead::Vector3f grid = {(f32)gridIndex.x, (f32)gridIndex.y, (f32)gridIndex.z};
    outPos->setMul(mtx, grid);
}

// TODO: might be a sead function?
inline s32 round(f32 val) {
    return val >= 0.0f ? val + 0.5f : val - 0.5f;
}

void calcGridIndexFromWorldPos(sead::Vector3i* outGridIndex, const sead::Matrix34f& mtx,
                               const sead::Vector3f& pos) {
    sead::Vector3f posOrigin = pos - mtx.getTranslation();

    outGridIndex->x = round(mtx.getBase(0).dot(posOrigin) / mtx.getBase(0).squaredLength());
    outGridIndex->y = round(mtx.getBase(1).dot(posOrigin) / mtx.getBase(1).squaredLength());
    outGridIndex->z = round(mtx.getBase(2).dot(posOrigin) / mtx.getBase(2).squaredLength());
}

sead::Vector3i calcGridIndexNext(const sead::Vector3i& gridIndex, s32 dirIndex) {
    sead::Vector3i nextGridIndex = gridIndex;
    s32 axis = sead::Mathi::abs(dirIndex);
    s32 dir = 0 < dirIndex ? 1 : -1;

    s32* selectedAxis;
    switch (axis) {
    case DirIndex_X:
        selectedAxis = &nextGridIndex.x;
        break;
    case DirIndex_Y:
        selectedAxis = &nextGridIndex.y;
        break;
    case DirIndex_Z:
        selectedAxis = &nextGridIndex.z;
        break;
    default:
        selectedAxis = &nextGridIndex.z;
        break;
    }

    *selectedAxis += dir;
    return nextGridIndex;
}

f32 calcDegreeDirIndexSignedX(s32 dirIndex) {
    if ((u32)(dirIndex + 3) < 7U)
        return gIndexToDegreeX[dirIndex + 3];
    return 0;
}

f32 calcDegreeDirIndexSignedY(s32 dirIndex) {
    if ((u32)(dirIndex + 3) < 7U)
        return gIndexToDegreeY[dirIndex + 3];
    return 0;
}

f32 calcDegreeDirIndexSignedZ(s32 dirIndex) {
    if ((u32)(dirIndex + 2) < 5U)
        return gIndexToDegreeZ[dirIndex + 2];
    return 0;
}

s32 calcDirIndexNearXY(const sead::Vector3i& startGridIndex, const sead::Vector3i& endGridIndex) {
    s32 x = endGridIndex.x - startGridIndex.x;
    s32 y = endGridIndex.y - startGridIndex.y;

    if (y == 0 && x == 0)
        return DirIndex_None;

    if (sead::Mathi::abs(x) >= sead::Mathi::abs(y))
        return x > 0 ? DirIndex_X : DirIndex_NegX;
    else
        return y > 0 ? DirIndex_Y : DirIndex_NegY;
}

s32 calcDirIndexNearYZ(const sead::Vector3i& startGridIndex, const sead::Vector3i& endGridIndex) {
    s32 y = endGridIndex.y - startGridIndex.y;
    s32 z = endGridIndex.z - startGridIndex.z;

    if (z == 0 && y == 0)
        return DirIndex_None;

    if (sead::Mathi::abs(y) >= sead::Mathi::abs(z))
        return 0 < y ? DirIndex_Y : DirIndex_NegY;
    else
        return 0 < z ? DirIndex_Z : DirIndex_NegZ;
}

s32 calcDirIndexNearZX(const sead::Vector3i& startGridIndex, const sead::Vector3i& endGridIndex) {
    s32 z = endGridIndex.z - startGridIndex.z;
    s32 x = endGridIndex.x - startGridIndex.x;

    if (x == 0 && z == 0)
        return DirIndex_None;

    if (sead::Mathi::abs(z) >= sead::Mathi::abs(x))
        return 0 < z ? DirIndex_Z : DirIndex_NegZ;
    else
        return 0 < x ? DirIndex_X : DirIndex_NegX;
}

s32 signDirIndexX(s32 dirIndex) {
    if (sead::Mathi::abs(dirIndex) != DirIndex_X)
        return 0;
    return sign(dirIndex);
}

s32 signDirIndexY(s32 dirIndex) {
    if (sead::Mathi::abs(dirIndex) != DirIndex_Y)
        return 0;
    return sign(dirIndex);
}

s32 signDirIndexZ(s32 dirIndex) {
    if (sead::Mathi::abs(dirIndex) != DirIndex_Z)
        return 0;
    return sign(dirIndex);
}

void calcGridMinMaxFromOBB(sead::Vector3i* outGridIndexMin, sead::Vector3i* outGridIndexMax,
                           const sead::Matrix34f& gridMtx, const sead::Matrix34f& boxMtx,
                           const sead::BoundBox3f& boundBox) {
    outGridIndexMin->x = sead::Mathi::maxNumber();
    outGridIndexMin->y = sead::Mathi::maxNumber();
    outGridIndexMin->z = sead::Mathi::maxNumber();
    outGridIndexMax->x = sead::Mathi::minNumber();
    outGridIndexMax->y = sead::Mathi::minNumber();
    outGridIndexMax->z = sead::Mathi::minNumber();
    expandGridFromOBB(outGridIndexMin, outGridIndexMax, gridMtx, boxMtx, boundBox);
}

void expandGridFromOBB(sead::Vector3i* outGridIndexMin, sead::Vector3i* outGridIndexMax,
                       const sead::Matrix34f& gridMtx, const sead::Matrix34f& boxMtx,
                       const sead::BoundBox3f& boundBox) {
    sead::Vector3f points[8];
    points[0] = {boundBox.getMin().x, boundBox.getMin().y, boundBox.getMin().z};
    points[1] = {boundBox.getMax().x, boundBox.getMin().y, boundBox.getMin().z};
    points[2] = {boundBox.getMax().x, boundBox.getMin().y, boundBox.getMax().z};
    points[3] = {boundBox.getMin().x, boundBox.getMin().y, boundBox.getMax().z};
    points[4] = {boundBox.getMin().x, boundBox.getMax().y, boundBox.getMin().z};
    points[5] = {boundBox.getMax().x, boundBox.getMax().y, boundBox.getMin().z};
    points[6] = {boundBox.getMax().x, boundBox.getMax().y, boundBox.getMax().z};
    points[7] = {boundBox.getMin().x, boundBox.getMax().y, boundBox.getMax().z};

    for (s32 i = 0; i < 8; i++) {
        sead::Vector3f gridPos;
        gridPos.setMul(boxMtx, points[i]);

        sead::Vector3i gridIndex;
        calcGridIndexFromWorldPos(&gridIndex, gridMtx, gridPos);

        if (outGridIndexMin->x > gridIndex.x)
            outGridIndexMin->x = gridIndex.x;
        if (outGridIndexMin->y > gridIndex.y)
            outGridIndexMin->y = gridIndex.y;
        if (outGridIndexMin->z > gridIndex.z)
            outGridIndexMin->z = gridIndex.z;

        if (outGridIndexMax->x < gridIndex.x)
            outGridIndexMax->x = gridIndex.x;
        if (outGridIndexMax->y < gridIndex.y)
            outGridIndexMax->y = gridIndex.y;
        if (outGridIndexMax->z < gridIndex.z)
            outGridIndexMax->z = gridIndex.z;
    }
}

}  // namespace al
