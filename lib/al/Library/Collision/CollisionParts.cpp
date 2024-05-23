#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

#include <cstdio>

namespace al {

CollisionParts::CollisionParts(void* kclData, const void* bymlData) {
    mKCollisionServer = new KCollisionServer();
    mKCollisionServer->initKCollisionServer(kclData, bymlData);

    mPrevBaseMtx = sead::Matrix34f::ident;
    mBaseMtx = sead::Matrix34f::ident;
    mSyncMtx = sead::Matrix34f::ident;
    mPrevBaseInvMtx = sead::Matrix34f::ident;
    mBaseInvMtx = sead::Matrix34f::ident;

    calcMtxScale(&mMtxScaleVec, mBaseInvMtx);
    mMtxScale = (mMtxScaleVec.x + mMtxScaleVec.y + mMtxScaleVec.z) / 3.0f;
    mInvMtxScale = 1.0f / mMtxScale;
}

void CollisionParts::initParts(const sead::Matrix34f& mtx) {
    resetAllMtx(mtx);
    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    calcMtxScale(&scale, mtx);
    mKCollisionServer->calcFarthestVertexDistance();
    mBaseMtxScale = (scale.x + scale.y + scale.z) / 3.0f;
    mBoundingSphereRange = mBaseMtxScale * mKCollisionServer->mFarthestVertexDistance;
}

void CollisionParts::resetAllMtx(const sead::Matrix34f& mtx_param) {
    if(!bVar4 && !bVar5)
        return;

    sead::Matrix34f mtx = mtx_param;
    makeEqualScale(&mtx);

    mPrevBaseMtx = mtx;
    mBaseMtx = mtx;
    mSyncMtx = mtx;
    mPrevBaseInvMtx.setInverse(mPrevBaseMtx);
    mBaseInvMtx = mPrevBaseInvMtx;
    
    calcMtxScale(&mMtxScaleVec, mBaseInvMtx);
    mMtxScale = (mMtxScaleVec.x + mMtxScaleVec.y + mMtxScaleVec.z) / 3.0f;
    mInvMtxScale = 1.0f / mMtxScale;

    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    calcMtxScale(&scale, mBaseMtx);
    mBaseMtxScale = (scale.x + scale.y + scale.z) / 3.0f;
    mBoundingSphereRange = mBaseMtxScale * mKCollisionServer->mFarthestVertexDistance;
}

f32 CollisionParts::makeEqualScale(sead::Matrix34f* mtx) {
    sead::Vector3f scale;
    calcMtxScale(&scale, *mtx);
    sead::Vector3f v17 = {scale.z-scale.x, scale.x-scale.y, scale.y-scale.z};
    if(isNearZero(v17, 0.001f)) return scale.x;

    f32 factor = 1.0f;
    sead::Vector3f vec;
    if(something == 1) {
        factor = (scale.x + scale.y + scale.z) / 3.0f;
        vec = {factor / scale.x, factor / scale.y, factor / scale.z};
    } else if (something == 2) {
        vec = {1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z};
    }

    mtx->scaleBases(vec.x, vec.y, vec.z);
}

}
