#pragma once

#include <math/seadVector.h>
#include <basis/seadTypes.h>

namespace al {
struct KCPrismData { // triangle
    float mLength;
    u16 mPosIndex;
    u16 mFaceNormalIndex;
    u16 mEdgeNormalIndex[3];
    u16 mCollisionType;
    u32 mTriIndex;
};

struct KCPrismHeader { // model
    u32 mPositionsOffset;
    u32 mNormalsOffset;
    u32 mTrianglesOffset;
    u32 mOctreeOffset;
    float mThickness;
    sead::Vector3f mOctreeOrigin;
    u32 mXMask;
    u32 mYMask;
    u32 mZMask;
    u32 mCoordShift;
    u32 mYShift;
    u32 mZShift;
    float mHitboxRadiusCap;
};
}
